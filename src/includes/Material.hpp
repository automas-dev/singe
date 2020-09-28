#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

namespace game {

    struct Material : public std::enable_shared_from_this<Material> {
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

    class MaterialLibrary : public std::enable_shared_from_this<MaterialLibrary> {
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
