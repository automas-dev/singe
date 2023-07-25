#include "singe/Graphics/ResourceManager.hpp"

#include <Wavefront.hpp>
#include <filesystem>
#include <fstream>
#include <singe/Support/SceneParser.hpp>
#include <singe/Support/log.hpp>
#include <string_view>

namespace singe {
    using std::ifstream;
    using std::istreambuf_iterator;
    using std::make_shared;
    using std::move;
    using std::string_view;

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

    shared_ptr<Texture> & ResourceManager::getTexture(const string & path,
                                                      bool useCached) {
        Logging::Resource->debug("ResourceManager::getTexture {} {}", path,
                                 useCached);

        fs::path fullPath = resourceAt(path);
        Logging::Resource->trace("Full path is {}", fullPath.c_str());

        map<string, shared_ptr<Texture>>::iterator cached;
        if (useCached && (cached = textures.find(path)) != textures.end()) {
            return cached->second;
        }

        auto texture = make_shared<Texture>(Texture::fromPath(fullPath));
        if (useCached) {
            textures[path] = move(texture);
        }
        return texture;
    }

    shared_ptr<Shader> & ResourceManager::getShader(const string & vertPath,
                                                    const string & fragPath,
                                                    bool useCached) {
        Logging::Resource->debug("ResourceManager::getShader {} {} {}",
                                 vertPath, fragPath, useCached);

        fs::path fullVertexPath = resourceAt(vertPath);
        fs::path fullFragmentPath = resourceAt(fragPath);
        Logging::Resource->trace("Vertex path is {}", fullVertexPath.c_str());
        Logging::Resource->trace("Fragment path is {}", fullFragmentPath.c_str());

        map<string, shared_ptr<Shader>>::iterator cached;
        if (useCached
            && (cached = shaders.find(vertPath + fragPath)) == shaders.end()) {
            return cached->second;
        }

        auto shader = make_shared<Shader>(
            glpp::Shader::fromPaths(fullVertexPath, fullFragmentPath));
        if (useCached) {
            shaders[vertPath + fragPath] = move(shader);
        }
        return shader;
    }

    shared_ptr<MVPShader> & ResourceManager::getMVPShader(const string & vertPath,
                                                          const string & fragPath,
                                                          bool useCached) {
        Logging::Resource->debug("ResourceManager::getMVPShader {} {} {}",
                                 vertPath, fragPath, useCached);

        fs::path fullVertexPath = resourceAt(vertPath);
        fs::path fullFragmentPath = resourceAt(fragPath);
        Logging::Resource->trace("Vertex path is {}", fullVertexPath.c_str());
        Logging::Resource->trace("Fragment path is {}", fullFragmentPath.c_str());

        map<string, shared_ptr<MVPShader>>::iterator cached;
        if (useCached
            && (cached = mvpShaders.find(vertPath + fragPath)) == mvpShaders.end()) {
            return cached->second;
        }

        auto shader = make_shared<MVPShader>(
            glpp::Shader::fromPaths(fullVertexPath, fullFragmentPath));
        if (useCached) {
            mvpShaders[vertPath + fragPath] = move(shader);
        }
        return shader;
    }

    vector<shared_ptr<Model>> ResourceManager::loadModel(const string & path) {
        Logging::Resource->debug("ResourceManager::loadModel {}", path);

        fs::path fullPath = resourceAt(path);
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

    inline Transform convertTransform(const scene::Transform & transform) {
        return Transform(transform.pos, glm::quat(transform.rot), transform.scale);
    }

    static shared_ptr<Scene> convertScene(ResourceManager * res,
                                          shared_ptr<scene::Scene> & resScene) {
        auto scene = make_shared<Scene>();

        if (resScene->grid) {
            scene->grid = make_shared<Grid>(resScene->grid->size,
                                            resScene->grid->color,
                                            false);
        }

        scene->transform = convertTransform(resScene->transform);

        // TODO: Cameras

        for (auto & resModel : resScene->models) {
            auto model = make_shared<Model>();

            model->transform = convertTransform(resModel.transform);

            // TODO: mesh
            // model->res->loadModel()

            // TODO: shader
            // model->material->shader = res->loadShader()

            scene->models.emplace_back(model);
        }

        for (auto & child : resScene->children) {
            scene->children.emplace_back(convertScene(res, child));
        }

        return scene;
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

        auto resScene = scene::SceneParser().parse(is);
        auto scene = convertScene(this, resScene);
        return scene;
    }
}
