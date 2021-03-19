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
    std::array<Vertex, 4> points;

    Quad() { }

    Quad(const Vertex & p1, const Vertex & p2, const Vertex & p3) : Quad({p1, p2, p3, p1 + p3 - p2}) { }

    Quad(const std::array<Vertex, 4> p) {
        points = p;
    }

    Quad(const Quad & other) {
        points = other.points;
    }

    Quad(Quad && other) {
        std::swap(points, other.points);
    }

    Quad & operator=(const Quad & other) {
        points = other.points;
        return *this;
    }

    std::vector<Vertex> toPoints() {
        std::vector<Vertex> cloud;
        cloud.push_back(points[0]);
        cloud.push_back(points[1]);
        cloud.push_back(points[2]);

        cloud.push_back(points[2]);
        cloud.push_back(points[3]);
        cloud.push_back(points[0]);
        return cloud;
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
    std::array<Quad, 6> faces;

    Cube(void) : Cube({0, 0, 0}) { }

    Cube(const glm::vec3 & origin, const UV & north=UV(), const UV & east=UV(), const UV & south=UV(), const UV & west=UV(), const UV & bottom=UV(), const UV & top=UV()) {
        auto & p = origin;
        glm::vec3 px = origin + glm::vec3(1, 0, 0);
        glm::vec3 py = origin + glm::vec3(0, 1, 0);
        glm::vec3 pz = origin + glm::vec3(0, 0, 1);
        glm::vec3 pxy = px + (py - p);
        glm::vec3 pxz = px + (pz - p);
        glm::vec3 pyz = py + (pz - p);
        glm::vec3 pxyz = px + (py - p) + (pz - p);

        faces[0] = Quad({py, {-1, 0, 0}, {west.u1, west.v1}},
                    {p, {-1, 0, 0}, {west.u1, west.v2}},
                    {pz, {-1, 0, 0}, {west.u2, west.v2}});
        faces[1] = Quad({pxyz, {1, 0, 0}, {east.u1, east.v1}},
                    {pxz, {1, 0, 0}, {east.u1, east.v2}},
                    {px, {1, 0, 0}, {east.u2, east.v2}});
        faces[2] = Quad({pz, {0, -1, 0}, {bottom.u1, bottom.v1}},
                    {p, {0, -1, 0}, {bottom.u1, bottom.v2}},
                    {px, {0, -1, 0}, {bottom.u2, bottom.v2}});
        faces[3] = Quad({py, {0, 1, 0}, {top.u1, top.v1}},
                    {pyz, {0, 1, 0}, {top.u1, top.v2}},
                    {pxyz, {0, 1, 0}, {top.u2, top.v2}});
        faces[4] = Quad({pxy, {0, 0, 1}, {south.u1, south.v1}},
                    {px, {0, 0, 1}, {south.u1, south.v2}},
                    {p, {0, 0, 1}, {south.u2, south.v2}});
        faces[5] = Quad({pyz, {0, 0, -1}, {north.u1, north.v1}},
                  {pz, {0, 0, -1}, {north.u1, north.v2}},
                  {pxz, {0, 0, -1}, {north.u2, north.v2}});
    }

    Cube(const Cube & other) {
        faces = other.faces;
    }

    Cube(Cube && other) {
        std::swap(faces, other.faces);
    }

    Cube & operator=(const Cube & other) {
        if (this == &other)
            return *this;
        
        faces = other.faces;

        return *this;
    }

    std::vector<Vertex> toPoints() {
        std::vector<Vertex> points;
        for (auto & face : faces) {
            auto p = face.toPoints();
            points.insert(points.end(), p.begin(), p.end());
        }
        return points;
    }
};


template<std::size_t N=8>
struct Chunk {
    std::array<std::array<std::array<Cube, N>, N>, N> cubes;

    Chunk(void) {
        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                for (int z = 0; z < N; z++) {
                    cubes[x][y][z].pos = {x, y, z};
                }
            }
        }
    }

    std::vector<Vertex> toPoints() {
        std::vector<Vertex> points;
        for (auto & y : cubes) {
            for (auto & z : y) {
                for (auto & cube : z) {
                    auto p = cube.toPoints();
                    points.insert(points.end(), p.begin(), p.end());
                }
            }
        }
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
