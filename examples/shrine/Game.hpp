#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Graphics/ResourceManager.hpp>
#include <singe/Graphics/Scene.hpp>
#include <singe/Support/log.hpp>
using namespace singe;

#include <glpp/extra/Grid.hpp>
using glpp::extra::Grid;

#include <memory>

class Game : public GameBase {
    ResourceManager res;
    FPSDisplay fps;
    std::shared_ptr<singe::Shader> shader;
    std::shared_ptr<singe::Shader> gridShader;
    Grid grid;
    Scene scene;
    shared_ptr<Scene> pillar;
    float tPillar;

public:
    Game(Window & window);
    virtual ~Game();

    void onMouseDown(const sf::Event::MouseButtonEvent & event) override;
    void onMouseUp(const sf::Event::MouseButtonEvent & event) override;
    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
