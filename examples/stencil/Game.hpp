#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <memory>
#include <s3e.hpp>
#include <vector>
using namespace Tom::s3e;

class Game : public GameBase {
    ResourceManager resManager;
    FPSDisplay::Ptr fps;
    Scene::Ptr floorScene;
    Scene::Ptr scene;

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    void onCreate(void) override;
    void onDestroy(void) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
