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
    class Model : public Transform3d {
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
         * Load a single Mesh from points and use the default Material.
         *
         * @param points the points for the mesh
         */
        bool loadFromPoints(const std::vector<Vertex> & points);

        /**
         * Load a single Mesh from points and use material.
         *
         * @param points the points for the mesh
         * @param material the material to use
         */
        bool loadFromPoints(const std::vector<Vertex> & points,
                            Material::Ptr & material);

        /**
         * Get a Material by name or nullptr if none exist.
         *
         * @param name the material name
         *
         * @return a const pointer reference to the Material
         */
        Material::ConstPtr getMaterial(const std::string & name) const;

        /**
         * Draw the Model.
         */
        void draw() const;

        /**
         * Draw the Model using shader.
         *
         * @param shader the MaterialShader used to draw this Model
         */
        void draw(const MaterialShader::Ptr & shader) const;
    };
};
