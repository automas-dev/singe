#include "singe/Graphics/Geometry.hpp"

#include <memory>

namespace singe {
    using std::move;

    Geometry::Geometry() : mesh(nullptr), array(), n(0) {}

    Geometry::Geometry(const shared_ptr<Mesh> & mesh)
        : mesh(mesh), array(), n(0) {

        if (mesh)
            update();
    }

    Geometry::Geometry(Geometry && other)
        : mesh(move(other.mesh)), array(move(other.array)), n(other.n) {}

    Geometry & Geometry::operator=(Geometry && other) {
        mesh = move(other.mesh);
        array = move(other.array);
        n = other.n;
        return *this;
    }

    Geometry::~Geometry() {}

    void Geometry::update(Buffer::Usage usage) {
        if (mesh) {
            array.bufferData(*mesh, usage);
            n = mesh->size();
            array.unbind();
        }
        else
            n = 0;
    }

    void Geometry::draw(RenderState & state) const {
        if (mesh)
            array.drawArrays(Buffer::Triangles, 0, n);
    }
}
