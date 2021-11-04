#include "Wavefront.hpp"

#include <array>
#include <fstream>
#include <stdexcept>

#include "WavefrontParser.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace wavefront {
    ImageData::ImageData(const char * path) {
        data = stbi_load(path, &width, &height, &nrComponents, 0);
    }

    ImageData::~ImageData() {
        if (data) {
            stbi_image_free(data);
        }
    }

    bool ImageData::isLoaded() const {
        return data != nullptr;
    }
}

namespace wavefront {
    Mesh::Mesh() : matId(0) {}
}

namespace wavefront {
    Model::Model() {}

    Model::~Model() {
        clear();
    }

    void Model::clear() {
        for (auto * ptr : objects) {
            delete ptr;
        }
        objects.clear();
        for (auto * ptr : materials) {
            delete ptr;
        }
        materials.clear();
    }

    void Model::loadMaterialsFrom(const std::string & path,
                                  const std::string & name) {
        std::ifstream is(path);
        if (!is.is_open()) {
            throw ModelLoadException("Failed to open file " + path);
        }
        Parser parser(is);
    }

    void Model::loadModelFrom(const std::string & path) {
        std::ifstream is(path);
        if (!is.is_open()) {
            throw ModelLoadException("Failed to open file " + path);
        }
        Parser parser(is);

        std::vector<glm::vec3> av;
        std::vector<glm::vec2> avt;
        std::vector<glm::vec3> avn;

        Mesh * mesh = nullptr;

        for (auto & token : parser) {
            switch (token.key[0]) {
                case 'o': {
                    mesh = new Mesh();
                    mesh->name = token.value;
                    objects.push_back(mesh);
                } break;
                case 'v': {
                    if (token.key.size() == 1) { // v
                        auto & v = av.emplace_back();
                        auto params = token.params();
                        if (params.size() != v.length())
                            throw ModelLoadException(
                                "Vertex (v) must have 3 values");
                        for (int i = 0; i < v.length(); i++) //
                            v[i] = std::stof(params[i]);
                    }
                    else { // token.key.size() > 1
                        switch (token.key[1]) {
                            case 't': { // vt
                                auto & vt = avt.emplace_back();
                                auto params = token.params();
                                if (params.size() != vt.length())
                                    throw ModelLoadException(
                                        "TexCoord (vt) must have 2 values");
                                for (int i = 0; i < vt.length(); i++)
                                    vt[i] = std::stof(params[i]);
                            } break;
                            case 'n': { // vn
                                auto & vn = avn.emplace_back();
                                auto params = token.params();
                                if (params.size() != vn.length())
                                    throw ModelLoadException(
                                        "Normal (vn) must have 3 values");
                                for (int i = 0; i < vn.length(); i++)
                                    vn[i] = std::stof(params[i]);
                            } break;
                        }
                    }
                } break;
                case 'f': {
                    if (!mesh)
                        throw ModelLoadException(
                            "Got a face (f) before starign an object (o)");
                    auto params = token.params();
                    if (params.size() != 3)
                        throw ModelLoadException("Face (f) must have 3 components");
                    for (int i = 0; i < 3; i++) {
                        auto subParams = splitString(params[i], '/');
                        if (subParams.size() != 3)
                            throw ModelLoadException(
                                "Face (c) component must have 3 values");
                        size_t iv = std::stoul(subParams[0]);
                        if (iv >= av.size())
                            throw ModelLoadException("Vertex out of bounds");
                        mesh->vertices.push_back(av[iv]);

                        size_t it = std::stoul(subParams[1]);
                        if (it >= avt.size())
                            throw ModelLoadException("TexCoord out of bounds");
                        mesh->texcoords.push_back(avt[it]);

                        size_t in = std::stoul(subParams[2]);
                        if (in >= avn.size())
                            throw ModelLoadException("Normal out of bounds");
                        mesh->normals.push_back(avn[in]);
                    }
                } break;
                case 'm': { // mtllib
                    if (token.key != "mtllib")
                        break;
                    auto lastSlash = path.find_last_of('/');
                    std::string mtlPath = path.substr(0, lastSlash + 1);
                    mtlPath += token.value;
                    loadMaterialsFrom(mtlPath, token.value);
                } break;
                case 'u': { // usemtl
                    if (!mesh)
                        throw ModelLoadException(
                            "Got a (usemtl) before starign an object (o)");
                    if (token.key != "usemtl")
                        break;
                    for (int i = 0; i < materials.size(); i++) {
                        if (materials[i]->name == token.value) {
                            mesh->matId = i;
                            break;
                        }
                    }
                    throw ModelLoadException("Unknown material name " + token.value);
                } break;
                default:
                    break;
            }
        }

        clear();
    }
}
