#include "singe/Graphics/ResourceManager.hpp"

#include <Wavefront.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace singe {
    using std::make_shared;
    using std::move;

    ResourceManager::ResourceManager(const path & root) : root(root) {}

    ResourceManager::ResourceManager(ResourceManager && other)
        : root(other.root) {}

    ResourceManager & ResourceManager::operator=(ResourceManager && other) {
        root = other.root;
        return *this;
    }

    ResourceManager::~ResourceManager() {}

    void ResourceManager::setRoot(const path & root) {
        this->root = root;
    }

    const path & ResourceManager::getRoot() const {
        return root;
    }

    path ResourceManager::resourceAt(const path & subPath) const {
        if (subPath.is_absolute())
            return subPath;
        else
            return root / subPath;
    }

    shared_ptr<Texture> & ResourceManager::getTexture(const string & name) {
        static path subPath("img");

        if (textures.find(name) == textures.end()) {
            path fullPath = resourceAt(subPath / name);
            auto texture = make_shared<Texture>(Texture::fromPath(fullPath));
            textures[name] = move(texture);
        }

        return textures[name];
    }

    shared_ptr<Shader> & ResourceManager::getShader(const string & name) {
        static path subPath("shader");

        if (shaders.find(name) == shaders.end()) {
            path fullVertexPath = resourceAt(subPath / (name + ".vert"));
            path fullFragmentPath = resourceAt(subPath / (name + ".frag"));
            auto shader = make_shared<Shader>(
                glpp::Shader::fromPaths(fullVertexPath, fullFragmentPath));
            shaders[name] = move(shader);
        }

        return shaders[name];
    }

    shared_ptr<Shader> & ResourceManager::getShaderFragmentOnly(const string & name) {
        static path subpath("shader");

        if (shaders.find(name) == shaders.end()) {
            path fullFragmentPath = resourceAt(subpath / (name + ".frag"));
            auto shader = make_shared<Shader>(
                glpp::Shader::fromFragmentPath(fullFragmentPath));
            shaders[name] = move(shader);
        }

        return shaders[name];
    }

    shared_ptr<Mesh> ResourceManager::loadModel(const string & name) {
        static path subPath("model");

        path fullPath = resourceAt(subPath / name);

        wavefront::Model wfModel;
        wfModel.loadModelFrom(fullPath);

        auto model = std::make_shared<singe::Mesh>();

        if (!wfModel.objects.empty()) {
            auto * obj = wfModel.objects[0];

            for (int i = 0; i < obj->size(); i++) {
                model->points.emplace_back(obj->vertices[i], obj->normals[i],
                                           obj->texcoords[i]);
            }

            model->update();
        }

        if (!wfModel.materials.empty()) {
            model->material = make_shared<Material>();

            auto * mat = wfModel.materials[0];
            auto & material = model->material;
            material->name = mat->name;
            material->ambient = mat->colAmbient;
            material->diffuse = mat->colDiffuse;
            material->specular = mat->colSpecular;
            material->specExp = mat->specExp;
            material->alpha = mat->alpha;
            if (!mat->texAlbedo.empty())
                material->texture = getTexture(mat->texAlbedo);
            if (!mat->texNormal.empty())
                material->normalTexture = getTexture(mat->texNormal);
            if (!mat->texSpecular.empty())
                material->specularTexture = getTexture(mat->texSpecular);
        }

        return model;
    }

    // shared_ptr<Scene> ResourceManager::getScene(const string & name) {
    //     static path subPath("scene");

    //     if (scenes.find(name) == scenes.end()) {
    //         path fullPath = resourceAt(subPath / name);
    //         auto scene = make_shared<Scene>(Scene::fromPath(fullPath));
    //         scenes[name] = scene;
    //         return scene;
    //     }
    //     else
    //         return scenes[name];
    // }
}
