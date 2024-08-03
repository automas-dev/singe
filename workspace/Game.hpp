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

#include "Diamond.hpp"
#include "Collision.hpp"

struct Box {
    AABB aabb;
    singe::Scene::Ptr scene;
};

struct Ball {
    Sphere sphere;
    singe::Scene::Ptr scene;
};

class Game : public GameBase {
    ResourceManager res;
    FPSDisplay fps;
    std::shared_ptr<singe::MVPShader> shader;
    std::shared_ptr<singe::Shader> circle_shader;
    Scene scene;
    shared_ptr<Diamond> marker;

    bool drawGrid;
    bool drawMarker;
    bool moveBall;
    bool moveBox;

    Box box;
    Box box2;
    Ball ball;

    float step;

    void reset();

public:
    Game(Window::Ptr & window);
    virtual ~Game();

    void onKeyReleased(const sf::Event::KeyEvent & event) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
