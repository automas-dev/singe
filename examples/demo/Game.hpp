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

#include <glm/glm.hpp>
using namespace glm;

#include <memory>

class Game : public GameBase {
    ResourceManager res;
    std::shared_ptr<singe::Shader> shader;
    Grid grid;
    Scene scene;
    Scene * otherScene;
    bool showGrid;

    enum DisplayMode {
        Point = GL_POINT,
        Line = GL_LINE,
        Fill = GL_FILL
    };

    DisplayMode wireframe;

public:
    Game(Window & window);
    virtual ~Game();

    void onKeyPressed(const sf::Event::KeyEvent & event) override;
    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
