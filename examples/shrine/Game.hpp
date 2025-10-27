#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/ResourceManager.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Graphics/Scene.hpp>
#include <singe/Support/log.hpp>
using namespace singe;

#include <glpp/extra/Grid.hpp>
using glpp::extra::Grid;

#include <memory>

class Game : public GameBase {
    ResourceManager        res;
    FPSDisplay             fps;
    singe::MVPShader::Ptr  shader;
    Grid                   grid;
    Scene                  scene;
    std::shared_ptr<Scene> pillar;
    float                  tPillar;

public:
    Game(Window::Ptr & window);
    virtual ~Game();

    void onMouseDown(const sf::Event::MouseButtonEvent & event) override;
    void onMouseUp(const sf::Event::MouseButtonEvent & event) override;
    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
