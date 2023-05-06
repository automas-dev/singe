#pragma once

#include <glpp/Buffer.hpp>
#include <glpp/extra/Vertex.hpp>
#include <memory>

#include "Mesh.hpp"
#include "RenderState.hpp"

namespace singe {
    using std::shared_ptr;

    using glpp::Buffer;
    using glpp::extra::VertexBufferArray;

    struct Geometry {
        shared_ptr<Mesh> mesh;
        VertexBufferArray array;
        size_t n;

        Geometry();

        Geometry(const shared_ptr<Mesh> & mesh);

        Geometry(Geometry && other);

        Geometry & operator=(Geometry && other);

        Geometry(const Geometry &) = delete;
        Geometry & operator=(const Geometry &) = delete;

        ~Geometry();

        void update(Buffer::Usage usage = Buffer::Static);

        void draw(RenderState & state) const;
    };
}
