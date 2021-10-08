#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <s3e/Core.hpp>
#include <s3e/Graphics.hpp>
#include <s3e/Support/Keymap.hpp>
#include <s3e/Support/log.hpp>
#include <vector>
using namespace Tom::s3e;

struct Light {
    size_t index;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 position;
    glm::vec3 direction;

    enum Type { DIRECTIONAL = 0, POINT, SPOT };
    Type type;

    // Point
    float constant;
    float linear;
    float quadratic;

    // Spot
    float cutOff;
    float outerCutOff;

    void uniform(Shader::Ptr shader) const {
        auto light = fmt::format("lights[{}]", index);

        shader->setVec3(light + ".ambient", ambient);
        shader->setVec3(light + ".diffuse", diffuse);
        shader->setVec3(light + ".specular", specular);
        shader->setVec3(light + ".position", position);
        shader->setVec3(light + ".direction", direction);
        shader->setUInt(light + ".type", type);

        shader->setFloat(light + ".constant", constant);
        shader->setFloat(light + ".linear", linear);
        shader->setFloat(light + ".quadratic", quadratic);

        shader->setFloat(light + ".cutOff", cutOff);
        shader->setFloat(light + ".outerCutOff", outerCutOff);
    }
};

class Game : public GameBase {
    std::vector<glm::vec3> gridVerts;
    std::vector<glm::vec3> gridCols;

    ResourceManager resManager;

    sf::Font font;

    FPSDisplay::Ptr fps;

    Shader::Ptr defaultShader;
    Shader::Ptr debugShader;
    Shader::Ptr geometryShader;
    Shader::Ptr lightingShader;
    Shader::Ptr monoShader;

    Texture::Ptr texture;

    bool doDrawGrid = false;
    bool drawGridOver = false;
    bool doDrawDebug = true;
    bool doBlitFbuff = true;

    Scene::Ptr cubeModel;
    Scene::Ptr sphereModel;
    Scene::Ptr hallModel;
    Scene::Ptr rootScene;

    Light light0;
    Light light1;
    Light light2;

    FrameBuffer::Ptr fbuff;
    FrameBuffer::Ptr gbuffMulti;

    Keymap keymap;

    float time = 0.0;
    float tps = 20.0;
    float timeSinceLastTick = 0.0;
    sf::Clock tickTimer;

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    void onCreate(void) override;
    void onDestroy(void) override;

    void onKeyPressed(const sf::Event::KeyEvent & e) override;
    void onKeyReleased(const sf::Event::KeyEvent & e) override;
    void onMouseMove(const sf::Event::MouseMoveEvent & e) override;
    void onMouseDown(const sf::Event::MouseButtonEvent & e) override;
    void onMouseUp(const sf::Event::MouseButtonEvent & e) override;
    void onResized(const sf::Event::SizeEvent & e) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw() const override;

private:
    void tick(void);
};
