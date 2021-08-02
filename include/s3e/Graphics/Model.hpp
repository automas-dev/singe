#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Material.hpp"
#include "VBO.hpp"
#include "s3e/Support/Transform3d.hpp"

namespace Tom::s3e {

    /**
     * A Model which has a mesh and model transforms.
     */
    class Model : public Mesh, public Transform3d {
    public:
        using Ptr = std::shared_ptr<Model>;
        using ConstPtr = std::shared_ptr<const Model>;

        using Mesh::Mesh;

        /**
         * Destruct the Model.
         */
        virtual ~Model();
    };
};
