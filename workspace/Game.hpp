#pragma once

#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Support/log.hpp>
#include <singe/Core/FPSDisplay.hpp>
using namespace singe;


class Game : public GameBase {
    FPSDisplay fps;

public:
    Game(Window & window);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
