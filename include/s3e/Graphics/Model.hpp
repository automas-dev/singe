#pragma once

#include <memory>
#include <string>

#include "Material.hpp"
#include "VBO.hpp"
#include "s3e/Support/Transform3d.hpp"

namespace Tom::s3e {
    /**
     * A Model which has a mesh and model transforms.
     */
    struct Model : public Mesh {

        using Ptr = std::shared_ptr<Model>;
        using ConstPtr = std::shared_ptr<const Model>;

        std::vector<Material::Ptr> materials;
        std::string name;

        Model(const std::string & name,
              Mode mode = Mode::Triangles,
              Usage usage = Usage::Static);

        /**
         * Destruct the Model.
         */
        virtual ~Model();

        /**
         * Bind textures and draw the mesh.
         */
        void draw() const;
    };
};
