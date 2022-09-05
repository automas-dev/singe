#pragma once

#include <glpp/extra/Transform.hpp>
#include <memory>
#include <string>

#include "singe/Graphics/Geometry.hpp"
#include "singe/Graphics/Material.hpp"
#include "singe/Graphics/RenderState.hpp"

namespace singe {
    using std::vector;
    using std::string;
    using glpp::Buffer;
    using glpp::extra::Transform;

    /**
     * A Model which has a mesh and model transforms.
     */
    struct Model {
        vector<Geometry> geometry;
        vector<Material::Ptr> materials;
        string name;
        Transform transform;

        /**
         * Create a new Model with name and Mesh parameters.
         *
         * @param name the model name
         * @param mode the Mesh draw mode
         * @param usage the Mesh update usage
         */
        Model(const string & name);

        Model(Model && other);
        
        Model & operator=(Model && other);
        
        Model(const Model &) = delete;
        Model & operator=(const Model &) = delete;

        ~Model();

        /**
         * Bind textures and draw the mesh.
         */
        void draw(Buffer::Mode mode, RenderState state) const;
    };
}
