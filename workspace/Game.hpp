#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
// #include <singe/Graphics/Grid.hpp>
#include <singe/Support/log.hpp>
using namespace singe;


class Game : public GameBase {
    FPSDisplay fps;
    // Grid::Ptr grid;
    Uniform mvp;

public:
    Game(Window & window);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
