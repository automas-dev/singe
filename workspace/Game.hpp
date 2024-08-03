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

#include "Collision.hpp"
#include "Diamond.hpp"
#include "Markers.hpp"

struct Box {
    AABB aabb;
    singe::Scene::Ptr scene;

    void updateTransform() {
        scene->transform.setPosition(aabb.pos - aabb.size / vec3(2));
        scene->transform.setScale(aabb.size);
    }
};

struct Ball {
    Sphere sphere;
    singe::Scene::Ptr scene;

    void updateTransform() {
        scene->transform.setPosition(sphere.p);
        scene->transform.setScale(glm::vec3(sphere.r));
    }
};

class Game : public GameBase {
    ResourceManager res;
    FPSDisplay fps;
    std::shared_ptr<singe::MVPShader> shader;
    std::shared_ptr<singe::Shader> circle_shader;
    Scene scene;
    shared_ptr<Diamond> marker;
    Markers::Ptr markers;

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
