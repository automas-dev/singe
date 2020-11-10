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

    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float specularExponent;
        float alpha;

        std::string name;

        typedef std::shared_ptr<Material> Ptr;
        typedef std::shared_ptr<const Material> ConstPtr;

        Material(void);
        ~Material();

        void print(std::ostream &os = std::cout) const;

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
