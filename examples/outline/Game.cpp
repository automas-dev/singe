#include "Game.hpp"

#include <spdlog/spdlog.h>

static const char * white_frag = R"(
#version 330 core
out vec4 FragColor;
uniform sampler2D gTexture;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
void main() {
    // FragColor = texture(gTexture, FragTex);
    FragColor = vec4(1.0);
}
)";

static const char * blur_frag = R"(
#version 330 core
out vec4 FragColor;
uniform sampler2D gTexture;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
uniform bool horizontal;
uniform float weight[10] = float[] (0.227027, 0.227027, 0.227027, 0.227027, 0.227027, 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
void main() {
    vec2 tex_offset = 1.0 / textureSize(gTexture, 0); // gets size of single texel
    vec3 result = texture(gTexture, FragTex).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 10; ++i)
        {
            result += texture(gTexture, FragTex + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(gTexture, FragTex - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 10; ++i)
        {
            result += texture(gTexture, FragTex + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(gTexture, FragTex - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
)";

static const char * mask_frag = R"(
#version 330 core
out vec4 FragColor;
uniform sampler2D gMask;
uniform sampler2D gBlur;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
void main() {
    float a = 1.0 - texture(gMask, FragTex).r; // should be 1.0 or 0.0
    FragColor = vec4(texture(gBlur, FragTex).rgb, a);
}
)";

Game::Game(const sf::String & resPath) : GameBase(), resManager(resPath) {}

Game::~Game() {}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar * message,
                                const void * userParam) {
    SPDLOG_ERROR("GL CALLBACK: {} type = 0x{:x}, severity = 0x{:x}, message = {}",
                 (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
                 severity, message);
}

bool Game::onCreate() {
    // defautFont loaded from memory by GameBase
    fps = std::make_shared<FPSDisplay>();
    fps->setFont(uiFont);
    fps->setRate(0.1f);

#ifdef DEBUG
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif

    // Add menu buttons
    menu->addMenuItem("Resume", [&]() {
        menu->hide();
        SetMouseGrab(true);
    });
    menu->addMenuItem("Exit", [&]() {
        window->close();
    });

    camera->move({5, 2, 5});
    camera->rotateEuler({0, -1, 0});
    camera->setFov(70);

    whiteShader = Shader::fromFragmentSource(white_frag);
    if (!whiteShader)
        return false;

    blurShader = Shader::fromFragmentSource(blur_frag);
    if (!blurShader)
        return false;

    maskShader = Shader::fromFragmentSource(mask_frag);
    if (!maskShader)
        return false;

    const std::vector<FrameBufferAttachment> attachments {
        FrameBufferAttachment(GL_COLOR_ATTACHMENT0, GL_RGBA, GL_RGBA, GL_FLOAT),
    };

    auto winSize = window->getSize();
    fbo = std::make_shared<FrameBuffer>(winSize, attachments);
    fbo2 = std::make_shared<FrameBuffer>(winSize, attachments);
    fbo3 = std::make_shared<FrameBuffer>(winSize, attachments);

    scene = std::make_shared<Scene>("Root");

    auto floorScene = resManager.loadScene("model/cube_plane.obj");
    if (!floorScene)
        return false;
    scene->children.push_back(floorScene);
    scene->send();

    auto sphereScene = resManager.loadScene("model/sphere.obj");
    if (!sphereScene)
        return false;
    sphereScene->scale({0.1, 0.1, 0.1});
    sphereScene->move({1, 2, 3});

    auto cubeScene = resManager.loadScene("model/cube.obj");
    if (!cubeScene)
        return false;
    cubeScene->move({3, 0, 0});

    scene->children.push_back(sphereScene);
    scene->children.push_back(cubeScene);
    scene->send();

    SetMouseGrab(true);
    return true;
}

void Game::onDestroy() {}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();
    fps->update(delta);
}

void Game::onDraw() const {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

    defaultShader->bind();
    defaultShader->setMat4("mvp", vp);
    scene->draw(defaultShader);

    fbo->bind();
    whiteShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    whiteShader->setMat4("mvp", vp);
    scene->draw(whiteShader);
    whiteShader->unbind();
    fbo->unbind();

    fbo2->bind();
    blurShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    blurShader->setMat4("mvp", glm::mat4(1));
    blurShader->setMat4("model", glm::mat4(1));
    blurShader->setBool("horizontal", true);
    fbo->getTextures()[0]->bind();
    draw_quad({-1, -1}, {2, 2});
    blurShader->unbind();
    fbo2->unbind();

    fbo3->bind();
    blurShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    blurShader->setMat4("mvp", glm::mat4(1));
    blurShader->setMat4("model", glm::mat4(1));
    blurShader->setBool("horizontal", false);
    fbo2->getTextures()[0]->bind();
    draw_quad({-1, -1}, {2, 2});
    blurShader->unbind();
    fbo3->unbind();

    maskShader->bind();
    glDisable(GL_DEPTH_TEST);
    maskShader->setMat4("mvp", glm::mat4(1));
    maskShader->setMat4("model", glm::mat4(1));
    glActiveTexture(GL_TEXTURE0);
    fbo->getTextures()[0]->bind();
    maskShader->setInt("gMask", 0);
    glActiveTexture(GL_TEXTURE1);
    fbo3->getTextures()[0]->bind();
    maskShader->setInt("gBlur", 1);
    draw_quad({-1, -1}, {2, 2});
    maskShader->unbind();

    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}
