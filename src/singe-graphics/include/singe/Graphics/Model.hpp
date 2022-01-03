#pragma once

#include <Transform3d.hpp>
#include <memory>
#include <singe/Core/RenderState.hpp>
#include <string>

#include "singe/Graphics/Geometry.hpp"
#include "singe/Graphics/Material.hpp"

namespace singe {
    /**
     * A Model which has a mesh and model transforms.
     */
    struct Model : public Transform3d {

        using Ptr = std::shared_ptr<Model>;
        using ConstPtr = std::shared_ptr<const Model>;

        std::vector<Geometry> geometry;
        std::vector<Material::Ptr> materials;
        std::string name;

        /**
         * Create a new Model with name and Mesh parameters.
         *
         * @param name the model name
         * @param mode the Mesh draw mode
         * @param usage the Mesh update usage
         */
        Model(const std::string & name);

        /**
         * Destruct the Model.
         */
        virtual ~Model();

        /**
         * Bind textures and draw the mesh.
         */
        void draw(glpp::Buffer::Mode mode, RenderState state) const;
    };
}
