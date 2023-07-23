#include "singe/Graphics/ResourceManager.hpp"

#include <Wavefront.hpp>
#include <filesystem>
#include <fstream>
#include <rapidxml.hpp>
#include <singe/Support/log.hpp>
#include <string_view>

namespace singe {
    using std::ifstream;
    using std::istreambuf_iterator;
    using std::make_shared;
    using std::move;
    using std::string_view;
    using rapidxml::xml_document;

    namespace Logging {
        Logger::Ptr Resource = make_shared<Logger>("Resource");
    }

    ResourceManager::ResourceManager(const fs::path & root) : root(root) {
        Logging::Resource->trace("Resource manager created with root {}",
                                 root.c_str());
    }

    ResourceManager::ResourceManager(ResourceManager && other)
        : root(other.root) {}

    ResourceManager & ResourceManager::operator=(ResourceManager && other) {
        root = other.root;
        return *this;
    }

    ResourceManager::~ResourceManager() {}

    void ResourceManager::setRoot(const fs::path & root) {
        Logging::Resource->trace("ResourceManager::setRoot {}", root.c_str());
        this->root = root;
    }

    const fs::path & ResourceManager::getRoot() const {
        return root;
    }

    fs::path ResourceManager::resourceAt(const fs::path & subPath) const {
        if (subPath.is_absolute())
            return subPath;
        else
            return root / subPath;
    }

    shared_ptr<Texture> & ResourceManager::getTexture(const string & name) {
        Logging::Resource->debug("ResourceManager::getTexture {}", name);
        static fs::path subPath("img");

        if (textures.find(name) == textures.end()) {
            fs::path fullPath = resourceAt(subPath / name);
            Logging::Resource->trace("Full path is {}", fullPath.c_str());

            auto texture = make_shared<Texture>(Texture::fromPath(fullPath));
            textures[name] = move(texture);
        }

        return textures[name];
    }

    shared_ptr<Shader> & ResourceManager::getShader(const string & name) {
        Logging::Resource->debug("ResourceManager::getShader {}", name);
        static fs::path subPath("shader");

        if (shaders.find(name) == shaders.end()) {
            fs::path fullVertexPath = resourceAt(subPath / (name + ".vert"));
            fs::path fullFragmentPath = resourceAt(subPath / (name + ".frag"));
            Logging::Resource->trace("Vertex path is {}", fullVertexPath.c_str());
            Logging::Resource->trace("Fragment path is {}",
                                     fullFragmentPath.c_str());

            auto shader = make_shared<Shader>(
                glpp::Shader::fromPaths(fullVertexPath, fullFragmentPath));
            shaders[name] = move(shader);
        }

        return shaders[name];
    }

    shared_ptr<MVPShader> ResourceManager::getMVPShader(const string & name) {
        Logging::Resource->debug("ResourceManager::getMVPShader {}", name);
        static fs::path subPath("shader");
        fs::path fullVertexPath = resourceAt(subPath / (name + ".vert"));
        fs::path fullFragmentPath = resourceAt(subPath / (name + ".frag"));
        Logging::Resource->trace("Vertex path is {}", fullVertexPath.c_str());
        Logging::Resource->trace("Fragment path is {}", fullFragmentPath.c_str());

        auto shader = make_shared<MVPShader>(
            glpp::Shader::fromPaths(fullVertexPath, fullFragmentPath));

        return shader;
    }

    shared_ptr<Shader> & ResourceManager::getShaderFragmentOnly(const string & name) {
        Logging::Resource->debug("ResourceManager::getShaderFragmentOnly {}", name);
        static fs::path subpath("shader");

        if (shaders.find(name) == shaders.end()) {
            fs::path fullFragmentPath = resourceAt(subpath / (name + ".frag"));
            Logging::Resource->trace("Fragment path is {}",
                                     fullFragmentPath.c_str());

            auto shader = make_shared<Shader>(
                glpp::Shader::fromFragmentPath(fullFragmentPath));
            shaders[name] = move(shader);
        }

        return shaders[name];
    }

    vector<shared_ptr<Model>> ResourceManager::loadModel(const string & name) {
        Logging::Resource->debug("ResourceManager::loadModel {}", name);
        static fs::path subPath("model");

        fs::path fullPath = resourceAt(subPath / name);
        Logging::Resource->trace("Full path is {}", fullPath.c_str());

        wavefront::Model wfModel;
        wfModel.loadModelFrom(fullPath);

        if (wfModel.materials.empty())
            Logging::Resource->warning("Model has no material");

        if (wfModel.objects.empty()) {
            Logging::Resource->error("Model has no objects");
            return {};
        }

        vector<shared_ptr<Material>> materials;

        for (auto & mat : wfModel.materials) {
            auto material = materials.emplace_back(make_shared<Material>());

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

        vector<shared_ptr<Model>> models;

        for (auto & obj : wfModel.objects) {
            auto & model = models.emplace_back(make_shared<Model>());

            if (obj->size() == 0)
                Logging::Resource->warning("Object " + obj->name + " has no points");

            for (int i = 0; i < obj->size(); i++) {
                model->points.emplace_back(obj->vertices[i], obj->normals[i],
                                           obj->texcoords[i]);
            }

            model->update();

            if (0 > obj->matId >= materials.size())
                Logging::Resource->error("Invalid material id");
            else
                model->material = materials[obj->matId];
        }

        return models;
    }

    shared_ptr<Scene> ResourceManager::loadScene(const string & name) {
        Logging::Resource->debug("ResourceManager::loadScene {}", name);
        static fs::path subPath("scene");

        fs::path fullPath = resourceAt(subPath / name);
        Logging::Resource->trace("Full path is {}", fullPath.c_str());

        ifstream is(fullPath);
        if (!is.is_open()) {
            Logging::Resource->error("Failed to open scene file {}",
                                     fullPath.c_str());
            return nullptr;
        }

        string body((istreambuf_iterator<char>(is)), istreambuf_iterator<char>());

        xml_document doc;
        doc.parse<0>(body.data());

        auto * root = doc.first_node("scene");
        if (!root) {
            Logging::Resource->error("No root scene node");
            return nullptr;
        }

        auto * nameAttr = root->first_attribute("name");
        if (!nameAttr) {
            Logging::Resource->error("Root scene has no name");
            return nullptr;
        }

        string sceneName(nameAttr->value(), nameAttr->value_size());
        // TODO: Add name field to Scene struct

        shared_ptr<Scene> scene = make_shared<Scene>();

        return scene;
    }
}
