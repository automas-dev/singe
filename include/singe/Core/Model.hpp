#pragma once

#include <Transform3d.hpp>
#include <memory>
#include <string>

#include "Transform3d.hpp"
#include "singe/Core/Geometry.hpp"
#include "singe/Core/Material.hpp"
#include "singe/Graphics/RenderState.hpp"
#include "glpp/VBO.hpp"

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
         * Send the Mesh and any materials to the OpenGL context.
         */
        void send();

        /**
         * Bind textures and draw the mesh.
         */
        void draw(RenderState state) const;
    };
}
