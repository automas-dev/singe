#pragma once

#include "s3e/Graphics/Texture.hpp"
#include "s3e/Support/log.hpp"
using namespace Tom::s3e;

#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
using std::vector;
using std::string;
using std::getline;
using std::shared_ptr;
using std::make_shared;
using std::ifstream;

struct Material {
    using Ptr = shared_ptr<Material>;
    using ConstPtr = shared_ptr<const Material>;

    enum IlluminationModel : int {
        ColorOn_AmbientOff = 0,
        ColorOn_AmbientOn,
        HighlightOn,
        ReflectionOn_RayTraceOn,
        GlassOn_RayTraceOn,
        FresnelOn_RayTraceOn,
        RefractionOn_FresnelOff_RayTraceOn,
        RefractionOn_FresnelOn_RayTraceOn,
        ReflectionOn_RayTraceOff,
        GlassOn_RayTraceOff,
        CastShadowsOnInvisibleSurfaces,
    };

    string name;
    glm::vec3 ambient; // Ka
    glm::vec3 diffuse; // Kd
    glm::vec3 specular; // Ks
    float specularExponent; // Ns
    float alpha; // d = 1 - Tr
    glm::vec3 transmissionColor; // Tf
    float indexOfRefraction; // Ni
    IlluminationModel illuminationModel; // illum

    Texture::Ptr ambientMap; // map_Ka
    Texture::Ptr diffuseMap; // map_Kd
    Texture::Ptr specularMap; // map_Ks
    Texture::Ptr specularExponentMap; // map_Ns
    Texture::Ptr alphaMap; // map_d
    Texture::Ptr bumpMap; // bump = map_bump
    Texture::Ptr displacementMap; // disp
    Texture::Ptr stencilMap; // decal

    static vector<Material::Ptr> loadFrom(const string & path) {
        vector<Material::Ptr> materials;

        ifstream is(path);
        if (!is.is_open()) {
            Logging::Core->warning("Failed to open file {}", path);
            return materials;
        }

        Material::Ptr tmpMaterial;

        string line;
        string str;
        for (size_t line_no = 0; getline(is, line); line_no++) {
            if (line.empty() || line[0] == '#')
                continue;

            switch (line[0]) {
                case 'K':
                    switch (line[1]) {
                        case 'a': // Ka
                            break;
                        case 'd': // Kd
                            break;
                        case 's': // Ks
                            break;
                    }
                    break;
                case 'N':
                    switch (line[1]) {
                        case 's': // Ns
                            break;
                        case 'i': // Ni
                            break;
                    }
                    break;
                case 'd':
                    switch (line[1]) {
                        case ' ': // d = 1 - Tr
                            break;
                        case 'i': // disp
                            break;
                        case 'e': // decal
                            break;
                    }
                    break;
                case 'T':
                    switch (line[1]) {
                        case 'r': // Tr = 1 - d
                            break;
                        case 'f': // Tf
                            break;
                    }
                    break;
                case 'i': // illum
                    break;
                case 'm':
                    break;
                case 'b':
                    break;
                default:
                    break;
            }
        }

        if (tmpMaterial) {
            materials.push_back(tmpMaterial);
        }

        return materials;
    }
};