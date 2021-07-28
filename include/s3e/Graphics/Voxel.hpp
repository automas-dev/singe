#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "VBO.hpp"

namespace Tom::s3e {

    struct UV {
        float u1, u2;
        float v1, v2;

        UV(void);

        UV(const glm::vec4 & vec);

        UV(float u1, float v1, float u2, float v2);

        UV(const UV & other) = default;

        UV(UV && other) = default;

        UV & operator=(const UV & other) = default;
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

        void setUV(const UV & uv);

        std::vector<Vertex> toPoints(const glm::vec3 & offset);
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

        BlockStyle(void);

        BlockStyle(const UV & north,
                   const UV & south,
                   const UV & east,
                   const UV & west,
                   const UV & bottom,
                   const UV & top);

        BlockStyle(const BlockStyle & other) = default;

        BlockStyle(BlockStyle && other) = default;

        BlockStyle & operator=(const BlockStyle & other) = default;
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

        std::vector<Vertex> toPoints(const glm::vec3 & offset);
    };

    struct SubChunk {
        static constexpr size_t N = 16;

        std::array<std::array<std::array<Cube, N>, N>, N> cubes;
        bool enabled = true;

        typedef std::shared_ptr<SubChunk> Ptr;
        typedef std::shared_ptr<const SubChunk> ConstPtr;

        SubChunk(void);

        SubChunk(const SubChunk & other) = default;

        SubChunk(SubChunk && other) = default;

        SubChunk & operator=(const SubChunk & other) = default;

        Cube & get(int x, int y, int z);

        std::vector<Vertex> toPoints(const glm::vec3 & offset);
    };

    struct Chunk {
        glm::vec3 pos;
        std::vector<SubChunk> subchunks;
        bool enabled = true;

        typedef std::shared_ptr<Chunk> Ptr;
        typedef std::shared_ptr<const Chunk> ConstPtr;

        Chunk(void);

        Chunk(const glm::vec3 & pos);

        Chunk(const glm::vec3 & pos, int n_sub);

        void set(int x, int y, int z, const BlockStyle::Ptr & style);

        Cube & get(int x, int y, int z);

        std::vector<Vertex> toPoints(void);
    };
};
