#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Texture.hpp"
#include "VBO.hpp"
#include "s3e/Support/Transform3d.hpp"

namespace Tom::s3e {

    struct Material {
        using Ptr = std::shared_ptr<Material>;
        using ConstPtr = std::shared_ptr<const Material>;

        std::string name;
        glm::vec3 ambient, diffuse, specular;
        float specExp;
        float alpha;
        Texture::Ptr texture;

        Material()
            : name("None"),
              ambient(0.8),
              diffuse(0.8),
              specular(0.8),
              specExp(500),
              alpha(1.0),
              texture(nullptr) {}

        void bind() const {
            if (texture)
                texture->bind();
        }
    };

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
