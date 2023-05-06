#pragma once

#include <glpp/extra/Vertex.hpp>
#include <vector>

namespace singe {
    using std::vector;
    using glpp::extra::Vertex;

    /// @brief Vector of Vertex points
    class Mesh : public vector<Vertex> {
    public:
        virtual ~Mesh() {}
    };
}
