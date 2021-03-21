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
#include <exception>
#include <s3e.hpp>
#include <glm/gtx/normal.hpp>
using namespace Tom::s3e;

struct UV {
    float u1, u2;
    float v1, v2;

    UV(void) : UV(0, 0, 1, 1) { }

    UV(const glm::vec4 & vec) : UV(vec[0], vec[1], vec[2], vec[3]) { }

    UV(const UV & other) = default;

    UV(UV && other) = default;

    UV & operator=(const UV & other) = default;

    UV(float u1, float v1, float u2, float v2) : u1(std::min(u1, u2)), u2(std::max(u1, u2)), v1(std::min(v1, v2)),
        v2(std::max(v1, v2)) { }
};

struct Quad {
    std::array<Vertex, 4> points;
    bool enabled = true;

    Quad(void) { }

    Quad(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3) {
        auto norm = glm::triangleNormal(p1, p2, p3);
        auto p4 = p1 + p3 - p2;
        points = {
            Vertex(p1, norm, {0, 1}),
            Vertex(p2, norm, {0, 0}),
            Vertex(p3, norm, {1, 0}),
            Vertex(p4, norm, {1, 1})
        };
    }

    Quad(const std::array<Vertex, 4> p) : points(p) { }

    Quad(const Quad & other) = default;

    Quad(Quad && other) = default;

    Quad & operator=(const Quad & other) = default;

    void setUV(const UV & uv) {
        points[0].uv = {uv.u1, uv.v1};
        points[1].uv = {uv.u1, uv.v2};
        points[2].uv = {uv.u2, uv.v2};
        points[3].uv = {uv.u2, uv.v1};
    }

    std::vector<Vertex> toPoints(const glm::vec3 & offset) {
        std::vector<Vertex> cloud;
        if (!enabled)
            return cloud;

        cloud.push_back(points[0] + offset);
        cloud.push_back(points[1] + offset);
        cloud.push_back(points[2] + offset);

        cloud.push_back(points[2] + offset);
        cloud.push_back(points[3] + offset);
        cloud.push_back(points[0] + offset);

        return cloud;
    }
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
    bool enabled = false;
    BlockStyle::Ptr style;

    enum Face {
        West = 0,
        East,
        Bottom,
        Top,
        South,
        North
    };

    Cube(void) : Cube({0, 0, 0}, std::make_shared<BlockStyle>()) { }

    Cube(const glm::vec3 & origin, const BlockStyle::Ptr & style) : style(style) {
        auto & p = origin;
        glm::vec3 px = origin + glm::vec3(1, 0, 0);
        glm::vec3 py = origin + glm::vec3(0, 1, 0);
        glm::vec3 pz = origin + glm::vec3(0, 0, 1);
        glm::vec3 pxy = px + (py - p);
        glm::vec3 pxz = px + (pz - p);
        glm::vec3 pyz = py + (pz - p);
        glm::vec3 pxyz = px + (py - p) + (pz - p);

        faces[West  ] = Quad(py, p, pz);
        faces[East  ] = Quad(pxyz, pxz, px);
        faces[Bottom] = Quad(pz, p, px);
        faces[Top   ] = Quad(py, pyz, pxyz);
        faces[South ] = Quad(pxy, px, p);
        faces[North ] = Quad(pyz, pz, pxz);
    }

    Cube(const Cube & other) = default;

    Cube(Cube && other) = default;

    Cube & operator=(const Cube & other) = default;

    std::vector<Vertex> toPoints(const glm::vec3 & offset) {
        faces[West  ].setUV(style->west);
        faces[East  ].setUV(style->east);
        faces[Bottom].setUV(style->bottom);
        faces[Top   ].setUV(style->top);
        faces[South ].setUV(style->south);
        faces[North ].setUV(style->north);

        std::vector<Vertex> cloud;
        if (!enabled)
            return cloud;

        for (auto & face : faces) {
            auto p = face.toPoints(offset);
            cloud.insert(cloud.end(), p.begin(), p.end());
        }
        return cloud;
    }
};


struct SubChunk {
    static constexpr size_t N = 10;

    std::array<std::array<std::array<Cube, N>, N>, N> cubes;
    bool enabled = true;

    typedef std::shared_ptr<SubChunk> Ptr;
    typedef std::shared_ptr<const SubChunk> ConstPtr;

    SubChunk(void) {
        auto style = std::make_shared<BlockStyle>();

        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                for (int z = 0; z < N; z++) {
                    cubes[y][x][z] = Cube({x, y, z}, style);
                }
            }
        }
    }

    SubChunk(const SubChunk & other) = default;

    SubChunk(SubChunk && other) = default;

    SubChunk & operator=(const SubChunk & other) = default;

    Cube & get(int x, int y, int z) {
        if (y < 0 || y >= SubChunk::N)
            throw std::out_of_range("Cube access y is out of range");
        if (x < 0 || x >= SubChunk::N)
            throw std::out_of_range("Cube access x is out of range");
        if (z < 0 || z >= SubChunk::N)
            throw std::out_of_range("Cube access z is out of range");

        return cubes[y][x][z];
    }

    std::vector<Vertex> toPoints(const glm::vec3 & offset) {
        std::vector<Vertex> points;
        if (!enabled)
            return points;

        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                for (int z = 0; z < N; z++) {
                    auto & cube = cubes[y][x][z];
                    if (!cube.enabled)
                        continue;

                    cube.faces[Cube::West  ].enabled = x == 0 || !cubes[y][x - 1][z].enabled;
                    cube.faces[Cube::East  ].enabled = x == (N - 1) || !cubes[y][x + 1][z].enabled;
                    cube.faces[Cube::Bottom].enabled = y == 0 || !cubes[y - 1][x][z].enabled;
                    cube.faces[Cube::Top   ].enabled = y == (N - 1) || !cubes[y + 1][x][z].enabled;
                    cube.faces[Cube::South ].enabled = z == 0 || !cubes[y][x][z - 1].enabled;
                    cube.faces[Cube::North ].enabled = z == (N - 1) || !cubes[y][x][z + 1].enabled;
                }
            }
        }

        for (auto & y : cubes) {
            for (auto & x : y) {
                for (auto & cube : x) {
                    auto p = cube.toPoints(offset);
                    points.insert(points.end(), p.begin(), p.end());
                }
            }
        }

        return points;
    }
};


struct Chunk {
    glm::vec3 pos;
    std::vector<SubChunk> subchunks;
    bool enabled = true;

    typedef std::shared_ptr<Chunk> Ptr;
    typedef std::shared_ptr<const Chunk> ConstPtr;

    Chunk(void) : Chunk({0, 0, 0}) { }

    Chunk(const glm::vec3 & pos) : pos(pos) {
        subchunks.emplace_back();
    }

    Chunk(const glm::vec3 & pos, int n_sub) : pos(pos) {
        while (subchunks.size() < n_sub)
            subchunks.emplace_back();
    }

    void set(int x, int y, int z, const BlockStyle::Ptr & style) {
        auto & cube = get(x, y, z);
        cube.style = style;
        cube.enabled = true;
    }

    Cube & get(int x, int y, int z) {
        x = x - (int)pos.x;
        z = z - (int)pos.z;
        int sub = y / SubChunk::N;
        y = y % SubChunk::N;

        if (sub < 0)
            throw std::out_of_range("Cube access y is too low");
        if (x < 0 || x >= SubChunk::N)
            throw std::out_of_range("Cube access x is out of range");
        if (z < 0 || z >= SubChunk::N)
            throw std::out_of_range("Cube access z is out of range");

        while (subchunks.size() <= sub)
            subchunks.emplace_back();

        return subchunks[sub].get(x, y, z);
    }

    std::vector<Vertex> toPoints(void) {
        std::vector<Vertex> points;
        if (!enabled)
            return points;

        for (int i = 0; i < subchunks.size(); i++) {
            auto & subchunk = subchunks[i];
            auto offset = pos + glm::vec3(0, i * SubChunk::N, 0);
            auto p = subchunk.toPoints(offset);
            points.insert(points.begin(), p.begin(), p.end());
        }

        return points;
    }
};

struct ChunkManager {
    std::map<std::pair<int, int>, Chunk::Ptr> chunks;

    Chunk::Ptr & get(int x, int y, int z) {
        
    }
};

class Game : public GameBase {
    DefaultResourceManager resManager;
    Shader::Ptr shader;
    FPSDisplay::Ptr fps;
    Texture::Ptr devTexture;
    Model::Ptr model;
    Chunk::Ptr chunk;
    std::vector<BlockStyle::Ptr> styles;

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
