#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <memory>
#include <s3e/Core.hpp>
#include <s3e/Graphics.hpp>
#include <s3e/Support/log.hpp>
#include <vector>
using namespace Tom::s3e;

class Game : public GameBase {
    ResourceManager resManager;
    FPSDisplay::Ptr fps;
    Scene::Ptr scene;
    Shader::Ptr whiteShader, blurShader, maskShader;
    FrameBuffer::Ptr fbo, fbo2, fbo3;

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    void onCreate(void) override;
    void onDestroy(void) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
