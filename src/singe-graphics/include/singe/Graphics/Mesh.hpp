#pragma once

#include <glpp/Buffer.hpp>
#include <glpp/extra/Buffer.hpp>
#include <vector>

#include "RenderState.hpp"

namespace singe {
    using std::vector;
    using glpp::Buffer;
    using glpp::BufferArray;
    using glpp::extra::Vertex;

    class Mesh {
        BufferArray array;
        size_t n;

    public:
        Mesh();

        Mesh(Mesh && other);

        Mesh & operator=(Mesh && other);

        Mesh(const Mesh &) = delete;
        Mesh & operator=(const Mesh &) = delete;

        ~Mesh();

        void bufferData(const vector<Vertex> & data, Buffer::Usage usage = Buffer::Static);

        void draw(RenderState & state) const;
    };
}
