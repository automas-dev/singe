#pragma once

#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Support/log.hpp>
using namespace singe;


class Game : public GameBase {

public:
    Game(Window & window);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
