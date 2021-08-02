#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "s3e/Graphics/VBO.hpp"

namespace Tom::s3e {

    using UV = glm::vec2;

    struct TexCoord {
        UV uv1, uv2;

        TexCoord(void);

        TexCoord(const glm::vec4 & vec);

        TexCoord(const UV & uv1, const UV & uv2);

        TexCoord(float u1, float v1, float u2, float v2);

        TexCoord(const TexCoord & other) = default;

        TexCoord(TexCoord && other) = default;

        TexCoord & operator=(const TexCoord & other) = default;
    };

    struct Quad {
        std::array<Vertex, 4> points;
        bool enabled = true;

        Quad(void);

        Quad(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3);

        Quad(const std::array<Vertex, 4> p);

        Quad(const Quad & other) = default;

        Quad(Quad && other) = default;

        Quad & operator=(const Quad & other) = default;

        Quad & operator=(Quad && other) = default;

        void setTexCoord(const TexCoord & coord);

        std::vector<Vertex> toPoints(const glm::vec3 & offset);
    };

    struct BlockStyle {
        TexCoord north;
        TexCoord south;
        TexCoord east;
        TexCoord west;
        TexCoord bottom;
        TexCoord top;

        using Ptr = std::shared_ptr<BlockStyle>;
        using ConstPtr = std::shared_ptr<const BlockStyle>;

        BlockStyle(void);

        BlockStyle(const TexCoord & north,
                   const TexCoord & south,
                   const TexCoord & east,
                   const TexCoord & west,
                   const TexCoord & bottom,
                   const TexCoord & top);

        BlockStyle(const BlockStyle & other) = default;

        BlockStyle(BlockStyle && other) = default;

        BlockStyle & operator=(const BlockStyle & other) = default;

        BlockStyle & operator=(BlockStyle && other) = default;
    };

    struct Cube {
        std::array<Quad, 6> faces;
        bool enabled = false;
        BlockStyle::Ptr style;

        enum Face { West = 0, East, Bottom, Top, South, North };

        Cube(void);

        Cube(const glm::vec3 & origin, const BlockStyle::Ptr & style);

        Cube(const Cube & other) = default;

        Cube(Cube && other) = default;

        Cube & operator=(const Cube & other) = default;

        Cube & operator=(Cube && other) = default;

        std::vector<Vertex> toPoints(const glm::vec3 & offset);
    };

    struct SubChunk {
        static constexpr size_t N = 16;

        std::array<std::array<std::array<Cube, N>, N>, N> cubes;
        bool enabled = true;

        using Ptr = std::shared_ptr<SubChunk>;
        using ConstPtr = std::shared_ptr<const SubChunk>;

        SubChunk(void);

        SubChunk(const SubChunk & other) = default;

        SubChunk(SubChunk && other) = default;

        SubChunk & operator=(const SubChunk & other) = default;

        SubChunk & operator=(SubChunk && other) = default;

        Cube & get(int x, int y, int z);

        std::vector<Vertex> toPoints(const glm::vec3 & offset);
    };

    struct Chunk {
        glm::vec3 pos;
        std::vector<SubChunk> subchunks;
        bool enabled = true;

        using Ptr = std::shared_ptr<Chunk>;
        using ConstPtr = std::shared_ptr<const Chunk>;

        Chunk(void);

        Chunk(const glm::vec3 & pos);

        Chunk(const glm::vec3 & pos, int n_sub);

        void set(int x, int y, int z, const BlockStyle::Ptr & style);

        Cube & get(int x, int y, int z);

        std::vector<Vertex> toPoints(void);
    };
};
