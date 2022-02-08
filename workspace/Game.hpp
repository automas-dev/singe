#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
// #include <singe/Graphics/Grid.hpp>
// #include <singe/Graphics/ResourceManager.hpp>
#include <singe/Support/log.hpp>
#include <vector>
using namespace singe;


class Game : public GameBase {
    // ResourceManager resManager;
    // FPSDisplay::Ptr fps;
    // Scene::Ptr scene;
    // Grid::Ptr grid;

public:
    Game(Window & window, const sf::String & resPath);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
