#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <s3e.hpp>
using namespace Tom::s3e;

struct Quad {
    Vertex p1, p2, p3, p4;

    Quad() { }

    Quad(const Quad & other) {
        p1 = other.p1;
        p2 = other.p2;
        p3 = other.p3;
        p4 = other.p4;
    }

    Quad(Quad && other) {
        std::swap(p1, other.p1);
        std::swap(p2, other.p2);
        std::swap(p3, other.p3);
        std::swap(p4, other.p4);
    }

    Quad(const Vertex & p1, const Vertex & p2, const Vertex & p3) : p1(p1), p2(p2), p3(p3) {
        p4 = p1 + p3 - p2;
    }

    Quad(const Vertex & p1, const Vertex & p2, const Vertex & p3, const Vertex & p4) : p1(p1), p2(p2), p3(p3), p4(p4) { }

    Quad & operator=(const Quad & other) {
        p1 = other.p1;
        p2 = other.p2;
        p3 = other.p3;
        p4 = other.p4;
        return *this;
    }

    std::vector<Vertex> toPoints() {
        std::vector<Vertex> points;
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        points.push_back(p3);
        points.push_back(p4);
        points.push_back(p1);
        return points;
    }
};

struct Cube {
    Quad f1;
    Quad f2;
    Quad f3;
    Quad f4;
    Quad f5;
    Quad f6;

    /// p1 = origin, p2 = +x, p3 = +y, p4 = +z
    Cube(const glm::vec3 & origin, const glm::vec3 & x, const glm::vec3 & y, const glm::vec3 & z) {
        Vertex p1(origin, {0, 0, 0}, {0, 0});
        Vertex p2(x, {0, 0, 0}, {1, 0});
        Vertex p3(y, {0, 0, 0}, {0, 1});
        Vertex p4(z, {0, 0, 0}, {1, 0});
        Vertex p5 = p2 + (p3 - p1);
        Vertex p6 = p2 + (p4 - p1);
        Vertex p7 = p3 + (p4 - p1);
        Vertex p8 = p2 + (p3 - p1) + (p4 - p1);

        f1 = Quad(p3, p1, p4);
        f2 = Quad(p8, p6, p2);
        f3 = Quad(p6, p4, p1);
        f4 = Quad(p5, p3, p7);
        f5 = Quad(p5, p2, p1);
        f6 = Quad(p7, p4, p6);
    }

    std::vector<Vertex> toPoints() {
        std::vector<Vertex> points;
#define tmp(F) for (auto &p : F.toPoints()) {\
            points.push_back(p);\
        }
        tmp(f1);
        tmp(f2);
        tmp(f3);
        tmp(f4);
        tmp(f5);
        tmp(f6);
#undef tmp
        return points;
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
