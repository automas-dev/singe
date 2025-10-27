#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/ResourceManager.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Graphics/Model.hpp>
#include <singe/Graphics/Scene.hpp>
#include <singe/Graphics/Shader.hpp>
#include <singe/Support/log.hpp>
using namespace singe;

#include <glpp/extra/Line.hpp>
#include <memory>

class Game : public GameBase {
    ResourceManager       res;
    FPSDisplay            fps;
    singe::MVPShader::Ptr shader;
    singe::Shader::Ptr    circle_shader;
    Scene                 scene;

    bool drawGrid;

public:
    Game(Window::Ptr & window);
    virtual ~Game();

    void onKeyReleased(const sf::Event::KeyEvent & event) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
