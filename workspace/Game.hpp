#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <singe/Core.hpp>
#include <singe/Graphics/Grid.hpp>
#include <singe/Support/log.hpp>
#include <vector>
using namespace singe;


class Game : public GameBase {
    ResourceManager resManager;
    FPSDisplay::Ptr fps;
    Scene::Ptr scene;
    Grid::Ptr grid;

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    void onCreate(void) override;
    void onDestroy(void) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
