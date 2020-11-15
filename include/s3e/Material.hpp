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
    struct Material {
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
        void print(std::ostream &os = std::cout) const;

        /**
         * Create a new Material that is managed by a std::shared_ptr.
         *
         * @return a shared pointer to a new Material
         */
        static Ptr create();
    };

    class MaterialShader : public Shader {
    public:
        typedef std::shared_ptr<MaterialShader> Ptr;
        typedef std::shared_ptr<const MaterialShader> ConstPtr;

        using Shader::Shader;

        void setMaterial(const Material::ConstPtr & material) const;

        static Ptr create(const std::string & vertexPath,
                          const std::string & fragmentPath);
    };

    class MaterialLibrary {
        std::vector<Material::Ptr> materials;

    public:
        typedef std::shared_ptr<MaterialLibrary> Ptr;
        typedef std::shared_ptr<const MaterialLibrary> ConstPtr;

        std::string name;

        MaterialLibrary(void);
        MaterialLibrary(const std::string & path);
        ~MaterialLibrary();

        bool loadFromPath(const std::string &path);

        std::size_t size(void) const;

        Material::ConstPtr getMaterial(int index) const;

        Material::ConstPtr getMaterial(const std::string & name) const;

        static MaterialLibrary::Ptr create(const std::string & path);
    };
}
