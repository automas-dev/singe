#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "s3e/Shader.hpp"

namespace Tom::s3e {

    /**
     * A single Wavefront OBJ material loaded from a .mtl file.
     */
    class Material {
    public:
        /**
         * The materials ambient property.
         */
        glm::vec3 ambient;

        /**
         * The material diffuse property.
         */
        glm::vec3 diffuse;

        /**
         * The material specular property.
         */
        glm::vec3 specular;

        /**
         * The material specular exponent.
         */
        float specularExponent;

        /**
         * The material alpha.
         */
        float alpha;

        /**
         * The material name
         */
        std::string name;

    public:
        /**
         * A shared pointer that manages a Material.
         */
        typedef std::shared_ptr<Material> Ptr;

        /**
         * A shared pointer that manages a const Material.
         */
        typedef std::shared_ptr<const Material> ConstPtr;

        /**
         * Construct a new Material with ambient, diffuse and specular set to
         * (0, 0, 0) and the specular exponent set to 0.
         */
        Material(void);

        /**
         * Destruct the Material.
         */
        ~Material();

        /**
         * Print the material field to the ostream `os`.
         *
         * @param os the ostream to write to
         */
        void print(std::ostream & os = std::cout) const;
    };

    /**
     * Specialization of the Shader class that applies a Material as uniforms
     * before drawing the Model.
     */
    class MaterialShader : public Shader {
    public:
        /**
         * A shared pointer that manages a MaterialShader.
         */
        typedef std::shared_ptr<MaterialShader> Ptr;

        /**
         * A shared pointer that manages a const MaterialShader.
         */
        typedef std::shared_ptr<const MaterialShader> ConstPtr;

        /**
         * Inherit the constructors from Shader.
         */
        using Shader::Shader;

        /**
         * Apply material as shader uniforms.
         *
         * @param material the Material to apply as uniforms
         */
        void setMaterial(const Material::ConstPtr & material) const;
    };

    /**
     * A collection of Wavefront OBJ Materials loaded from a .mtl file.
     */
    class MaterialLibrary {
        std::vector<Material::Ptr> materials;
        std::string path;

    public:
        /**
         * A shared pointer that manages a MaterialLibrary.
         */
        typedef std::shared_ptr<MaterialLibrary> Ptr;

        /**
         * A shared pointer that manages a const MaterialLibrary.
         */
        typedef std::shared_ptr<const MaterialLibrary> ConstPtr;

        /**
         * The name from the .mtl file.
         */
        std::string name;

        /**
         * Construct a new MaterialLibrary.
         */
        MaterialLibrary(void);

        /**
         * Destruct the MaterialLibrary.
         */
        ~MaterialLibrary();

        /**
         * Load a .mtl file from mtlPath.
         *
         * @param mtlPath the path to the .mtl file
         *
         * @return true if the file was loaded successfully
         */
        bool loadFromPath(const std::string & mtlPath);

        /**
         * Get the number of materials loaded from the .mtl file.
         *
         * @return the number of materials
         */
        std::size_t size(void) const;

        /**
         * Get the material at index or nullptr if the index is out of bounds.
         *
         * @param index the index of the material
         *
         * @return a const pointer to the material or nullptr
         */
        Material::ConstPtr getMaterial(int index) const;

        /**
         * Get the material by name or nullptr if no material exists with name.
         *
         * @param name the name of the material to return
         *
         * @return a const pointer to the material or nullptr
         */
        Material::ConstPtr getMaterial(const std::string & name) const;
    };
}

