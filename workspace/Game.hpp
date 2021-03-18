#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <array>
#include <s3e.hpp>
using namespace Tom::s3e;

struct Quad {
    Vertex p1, p2, p3, p4;

    Quad(const Vertex & p1, const Vertex & p2, const Vertex & p3) : p1(p1), p2(p2), p3(p3) {
        p4.pos = p1.pos + p3.pos - p2.pos;
        p4.norm = p1.norm + p3.norm - p2.norm;
        p4.uv = p1.uv + p3.uv - p2.uv;
    }

    std::vector<Vertex> toPoints() {
        std::vector<Vertex> p;
        p.push_back(p1);
        p.push_back(p2);
        p.push_back(p3);

        p.push_back(p3);
        p.push_back(p4);
        p.push_back(p1);
        return p;
    }
};

class Game : public GameBase {
    DefaultResourceManager resManager;
    Shader::Ptr shader;
    FPSDisplay::Ptr fps;
    Texture::Ptr devTexture;
    Model::Ptr model;

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    bool onCreate(void) override;
    void onDestroy(void) override;

    void onKeyPressed(const sf::Event::KeyEvent & e) override;
    void onKeyReleased(const sf::Event::KeyEvent & e) override;
    void onMouseMove(const sf::Event::MouseMoveEvent & e) override;
    void onMouseDown(const sf::Event::MouseButtonEvent & e) override;
    void onMouseUp(const sf::Event::MouseButtonEvent & e) override;
    void onResized(const sf::Event::SizeEvent & e) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
