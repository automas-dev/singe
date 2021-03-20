#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <s3e.hpp>
using namespace Tom::s3e;

struct Quad {
    std::array<Vertex, 4> points;
    bool enabled = true;

    Quad(void) { }

    Quad(const Vertex & p1, const Vertex & p2, const Vertex & p3) : Quad({p1, p2, p3, p1 + p3 - p2}) { }

    Quad(const std::array<Vertex, 4> p) {
        points = p;
    }

    Quad(const Quad & other) = default;

    Quad(Quad && other) = default;

    Quad & operator=(const Quad & other) = default;

    std::vector<Vertex> toPoints(void) {
        std::vector<Vertex> cloud;
        if (!enabled)
            return cloud;

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

struct BlockStyle {
    UV north;
    UV south;
    UV east;
    UV west;
    UV bottom;
    UV top;

    typedef std::shared_ptr<BlockStyle> Ptr;
    typedef std::shared_ptr<const BlockStyle> ConstPtr;

    BlockStyle(void) { }

    BlockStyle(const UV & north,  const UV & south, const UV & east, const UV & west, const UV & bottom, const UV & top) 
        : north(north), south(south), east(east), west(west), bottom(bottom), top(top) { }

    BlockStyle(const BlockStyle & other) = default;

    BlockStyle(BlockStyle && other) = default; 

    BlockStyle & operator=(const BlockStyle & other) = default;
};

struct Cube {
    std::array<Quad, 6> faces;
    bool enabled = true;
    std::shared_ptr<BlockStyle> style;

    Cube(void) : Cube({0, 0, 0}, std::make_shared<BlockStyle>()) { }

    Cube(const glm::vec3 & origin, const std::shared_ptr<BlockStyle> & style) : style(style) {
        auto & p = origin;
        glm::vec3 px = origin + glm::vec3(1, 0, 0);
        glm::vec3 py = origin + glm::vec3(0, 1, 0);
        glm::vec3 pz = origin + glm::vec3(0, 0, 1);
        glm::vec3 pxy = px + (py - p);
        glm::vec3 pxz = px + (pz - p);
        glm::vec3 pyz = py + (pz - p);
        glm::vec3 pxyz = px + (py - p) + (pz - p);

        faces[0] = Quad({py, {-1, 0, 0}, {style->west.u1, style->west.v1}},
                    {p, {-1, 0, 0}, {style->west.u1, style->west.v2}},
                    {pz, {-1, 0, 0}, {style->west.u2, style->west.v2}});
        faces[1] = Quad({pxyz, {1, 0, 0}, {style->east.u1, style->east.v1}},
                    {pxz, {1, 0, 0}, {style->east.u1, style->east.v2}},
                    {px, {1, 0, 0}, {style->east.u2, style->east.v2}});
        faces[2] = Quad({pz, {0, -1, 0}, {style->bottom.u1, style->bottom.v1}},
                    {p, {0, -1, 0}, {style->bottom.u1, style->bottom.v2}},
                    {px, {0, -1, 0}, {style->bottom.u2, style->bottom.v2}});
        faces[3] = Quad({py, {0, 1, 0}, {style->top.u1, style->top.v1}},
                    {pyz, {0, 1, 0}, {style->top.u1, style->top.v2}},
                    {pxyz, {0, 1, 0}, {style->top.u2, style->top.v2}});
        faces[4] = Quad({pxy, {0, 0, 1}, {style->south.u1, style->south.v1}},
                    {px, {0, 0, 1}, {style->south.u1, style->south.v2}},
                    {p, {0, 0, 1}, {style->south.u2, style->south.v2}});
        faces[5] = Quad({pyz, {0, 0, -1}, {style->north.u1, style->north.v1}},
                  {pz, {0, 0, -1}, {style->north.u1, style->north.v2}},
                  {pxz, {0, 0, -1}, {style->north.u2, style->north.v2}});
    }

    Cube(const Cube & other) = default;

    Cube(Cube && other) = default;

    Cube & operator=(const Cube & other) = default;

    std::vector<Vertex> toPoints(void) {
        std::vector<Vertex> cloud;
        if (!enabled)
            return cloud;

        for (auto & face : faces) {
            auto p = face.toPoints();
            cloud.insert(cloud.end(), p.begin(), p.end());
        }
        return cloud;
    }
};


struct Chunk {
    static constexpr size_t N = 16;

    std::array<std::array<std::array<Cube, N>, N>, N> cubes;
    bool enabled = true;

    typedef std::shared_ptr<Chunk> Ptr;
    typedef std::shared_ptr<const Chunk> ConstPtr;

    Chunk(void) {
        std::map<int, std::shared_ptr<BlockStyle>> styles;

        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                for (int z = 0; z < N; z++) {

                    UV uv (0.1 * (x+z), 0.1 * y, 0.1 * (z+x+1), 0.1 * (y+1));

                    std::shared_ptr<BlockStyle> style;
                    if (styles.count(x+z) > 0)
                        style = styles[x+z];
                    else
                        style = std::make_shared<BlockStyle>(uv, uv, uv, uv, uv, uv);

                    cubes[x][y][z] = Cube({x, y, z}, style);
                    cubes[x][y][z].enabled = x % 2 == 0 && z % 2 == 0 && y % 2 == 0;
                }
            }
        }
    }

    Chunk(const Chunk & other) = default;

    Chunk(Chunk && other) = default;

    Chunk & operator=(const Chunk & other) = default;

    std::vector<Vertex> toPoints(void) {
        std::vector<Vertex> points;
        if (!enabled)
            return points;

        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                for (int z = 0; z < N; z++) {
                    auto & cube = cubes[x][y][z];
                    if (!cube.enabled)
                        continue;

                    cube.faces[0].enabled = x == 0 || !cubes[x-1][y][z].enabled;
                    cube.faces[1].enabled = x == (N-1) || !cubes[x+1][y][z].enabled;
                    cube.faces[2].enabled = y == 0 || !cubes[x][y-1][z].enabled;
                    cube.faces[3].enabled = y == (N-1) || !cubes[x][y+1][z].enabled;
                    cube.faces[4].enabled = z == 0 || !cubes[x][y][z-1].enabled;
                    cube.faces[5].enabled = z == (N-1) || !cubes[x][y][z+1].enabled;
                }
            }
        }
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
    Chunk::Ptr chunk;

    int step = 8;

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
