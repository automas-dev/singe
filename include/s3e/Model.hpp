#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "s3e/VBO.hpp"
#include "s3e/Material.hpp"

namespace Tom::s3e {

    /**
     * Manage a single mesh from a Wavefront OBJ file.
     */
    class Mesh : public VBO {
        std::string name;
        Material::ConstPtr material;

    public:
        /**
         * A shared pointer that manages a Mesh.
         */
        typedef std::shared_ptr<Mesh> Ptr;

        /**
         * A shared pointer that manages a const Mesh.
         */
        typedef std::shared_ptr<const Mesh> ConstPtr;

        /**
         * Construct a new empty Mesh.
         */
        Mesh(void);

        /**
         * Construct a new Mesh from a name, material and vector of Vertex.
         *
         * @param name the material name
         * @param material the Material for the mesh
         */
        Mesh(const std::string & name, const Material::ConstPtr & material);

        /**
         * Destruct the Mesh.
         */
        virtual ~Mesh();

        /**
         * Get a const pointer reference to the Material for this Mesh.
         *
         * @return a const pointer reference to the Material for this Mesh
         */
        const Material::ConstPtr & getMaterial(void) const;
    };

    /**
     * A Wavefront OBJ .obj model file.
     */
    class Model {
        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 size;

        std::vector<Mesh::Ptr> models;
        std::string path;

        MaterialLibrary::Ptr materials;

    public:
        /**
         * A shared pointer that manages a Mesh.
         */
        typedef std::shared_ptr<Model> Ptr;

        /**
         * A shared pointer that manages a const Mesh.
         */
        typedef std::shared_ptr<const Model> ConstPtr;

        /**
         * Construct a new empty Model.
         */
        Model(void);

        /**
         * Destruct the Model.
         */
        virtual ~Model();

        /**
         * Load a .obj file from objPath.
         *
         * @param objPath the path to the .obj file
         *
         * @return true if the file was loaded successfully
         */
        bool loadFromPath(const std::string & objPath);

        /**
         * Move the Model by adding pos to the position.
         *
         * @param pos the delta position
         */
        void move(glm::vec3 pos);

        /**
         * Rotate the Model by adding rot to the rotation.
         *
         * @param rot the delta rotation
         */
        void rotate(glm::vec3 rot);

        /**
         * Scale the Model by multiplying the scale by scale.
         *
         * @param scale the delta scale
         */
        void scale(glm::vec3 scale);

        /**
         * Get the current position.
         *
         * @return the current position
         */
        const glm::vec3 & getPosition(void) const;

        /**
         * Set the Model position.
         *
         * @param pos the new position
         */
        void setPosition(glm::vec3 pos);

        /**
         * Get the current rotation.
         *
         * @return the current rotation
         */
        const glm::vec3 & getRotation(void) const;

        /**
         * Set the Model rotation.
         *
         * @param rot the new rotation
         */
        void setRotation(glm::vec3 rot);

        /**
         * Get the current scale.
         *
         * @return the current scale
         */
        const glm::vec3 & getScale(void) const;

        /**
         * Set the Model scale.
         *
         * @param scale the new scale
         */
        void setScale(glm::vec3 scale);

        /**
         * Get the model matrix. This is generated from position, rotation and
         * scale.
         *
         * @return the model matrix
         */
        glm::mat4 modelMatrix() const;

        /**
         * Get a Material by name or nullptr if none exist.
         *
         * @param name the material name
         *
         * @return a const pointer reference to the Material
         */
        Material::ConstPtr getMaterial(const std::string & name) const;

        /**
         * Draw the Model using shader.
         *
         * @param shader the MaterialShader used to draw this Model
         */
        void draw(const MaterialShader::Ptr & shader) const;
    };
};

