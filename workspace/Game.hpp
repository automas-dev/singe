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

struct UV {
    float u1, u2;
    float v1, v2;

    UV(void) : UV(0, 0, 1, 1) { }

    UV(const glm::vec4 & vec) : UV(vec[0], vec[1], vec[2], vec[3]) { }

    UV(float u1, float v1, float u2, float v2) : u1(std::min(u1, u2)), u2(std::max(u1, u2)), v1(std::min(v1, v2)), v2(std::max(v1, v2)) { }
};

struct Cube {
    Quad f1;
    Quad f2;
    Quad f3;
    Quad f4;
    Quad f5;
    Quad f6;

    Cube(void) : Cube({0, 0, 0}) { }

    /// p1 = origin, p2 = +x, p3 = +y, p4 = +z
    Cube(const glm::vec3 & origin, const UV & north=UV(), const UV & east=UV(), const UV & south=UV(), const UV & west=UV(), const UV & bottom=UV(), const UV & top=UV()) {
        auto & p = origin;
        glm::vec3 px = origin + glm::vec3(1, 0, 0);
        glm::vec3 py = origin + glm::vec3(0, 1, 0);
        glm::vec3 pz = origin + glm::vec3(0, 0, 1);
        glm::vec3 pxy = px + (py - p);
        glm::vec3 pxz = px + (pz - p);
        glm::vec3 pyz = py + (pz - p);
        glm::vec3 pxyz = px + (py - p) + (pz - p);

        f1 = Quad({py, {-1, 0, 0}, {west.u1, west.v1}},
                  {p, {-1, 0, 0}, {west.u1, west.v2}},
                  {pz, {-1, 0, 0}, {west.u2, west.v2}});
        f2 = Quad({pxyz, {1, 0, 0}, {east.u1, east.v1}},
                  {pxz, {1, 0, 0}, {east.u1, east.v2}},
                  {px, {1, 0, 0}, {east.u2, east.v2}});
        f3 = Quad({pz, {0, -1, 0}, {bottom.u1, bottom.v1}},
                  {p, {0, -1, 0}, {bottom.u1, bottom.v2}},
                  {px, {0, -1, 0}, {bottom.u2, bottom.v2}});
        f4 = Quad({py, {0, 1, 0}, {top.u1, top.v1}},
                  {pyz, {0, 1, 0}, {top.u1, top.v2}},
                  {pxyz, {0, 1, 0}, {top.u2, top.v2}});
        f5 = Quad({pxy, {0, 0, -1}, {south.u1, south.v1}},
                  {px, {0, 0, -1}, {south.u1, south.v2}},
                  {p, {0, 0, -1}, {south.u2, south.v2}});
        f6 = Quad({pyz, {0, 0, 1}, {north.u1, north.v1}},
                  {pz, {0, 0, 1}, {north.u1, north.v2}},
                  {pxz, {0, 0, 1}, {north.u2, north.v2}});
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
