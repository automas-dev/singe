#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Graphics/Model.hpp>
#include <singe/Support/log.hpp>
using namespace singe;

#include <memory>

class Game : public GameBase {
    FPSDisplay fps;
    Uniform mvp;
    Model model;
    Shader shader;

public:
    Game(Window & window);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
