#pragma once

#include <glm/glm.hpp>
#include <glpp/Texture.hpp>
#include <string>

namespace singe {
    using std::string;
    using glm::vec3;
    using glpp::Texture;

    struct Material {
        string name;
        vec3 ambient, diffuse, specular;
        float specExp;
        float alpha;
        shared_ptr<Texture> texture;
        shared_ptr<Texture> normalTexture;
        shared_ptr<Texture> specularTexture;

        Material();

        Material(Material && other);

        Material & operator=(Material && other);

        Material(const Material &) = delete;
        Material & operator=(const Material &) = delete;

        ~Material();
    };
}
