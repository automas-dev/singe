#pragma once

#include "s3e/Graphics/Model.hpp"
#include "s3e/Graphics/Shader.hpp"
#include "s3e/Graphics/VBO.hpp"
#include "s3e/Support/Transform3d.hpp"
#include "s3e/Support/log.hpp"
using namespace Tom::s3e;

#include <array>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <string>
#include <vector>
using std::array;
using std::vector;
using std::string;
using std::getline;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::ifstream;

// Material properties will have to be textures, ie specular map, etc.
struct ModelObject {

    using Ptr = shared_ptr<ModelObject>;
    using ConstPtr = shared_ptr<const ModelObject>;

    Mesh::Ptr mesh;
    vector<Texture::Ptr> textures;
    string name;

    ModelObject(const string & name) : name(name), mesh(nullptr) {}

    void bindTextures() const {
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            textures[i]->bind();
        }
    }

    void draw() const {
        bindTextures();
        mesh->draw();
    }
};

struct TempModel {
    struct Face {
        struct Point {
            size_t v, t, n;
        };
        array<Point, 3> p;
    };

    string name;
    string mtl;
    vector<Face> af;

    TempModel(const string & name) : name(name) {}

    ModelObject::Ptr toModel(const vector<glm::vec3> & av,
                             const vector<glm::vec2> & avt,
                             const vector<glm::vec3> & avn) {
        auto model = make_shared<ModelObject>(name);
        auto mesh = make_shared<Mesh>();
        model->mesh = mesh;
        for (auto & face : af) {
            for (auto & point : face.p) {
                if (point.v > av.size() || point.n > avn.size()
                    || point.t > avt.size()) {
                    Logging::Core->error("Point out of range");
                }
                mesh->points.emplace_back(av[point.v - 1],
                                          avn[point.n - 1],
                                          avt[point.t - 1]);
            }
        }
        mesh->send();
        return model;
    }
};

struct Scene : public Transform3d {

    using Ptr = shared_ptr<Scene>;
    using ConstPtr = shared_ptr<const Scene>;

    string name;
    vector<ModelObject::Ptr> models;
    vector<Scene::Ptr> children;

    void draw(const Shader::Ptr & shader,
              const glm::mat4 & parentTransform = glm::mat4(1)) const {
        auto transform = parentTransform * toMatrix();
        shader->setMat4("model", transform);
        for (auto & model : models) {
            model->draw();
        }
        for (auto & child : children) {
            child->draw(shader, transform);
        }
    }

    static Scene::Ptr loadFromPoints(const vector<Vertex> & points) {
        auto scene = make_shared<Scene>();
        scene->name = "points";
        auto model = make_shared<ModelObject>("points");
        auto mesh = make_shared<Mesh>();
        mesh->loadFromPoints(points);
        model->mesh = mesh;
        scene->models.push_back(model);
        return scene;
    }

#define PARSE_ERROR(TAG)                                                    \
    {                                                                       \
        Logging::Core->error("Failed to parse tag {}: {}", (TAG), line_no); \
        return nullptr;                                                     \
    }

#define ASSERT_NREAD(N, TAG) \
    if (nRead != (N))        \
        PARSE_ERROR(TAG);

    static Scene::Ptr loadFrom(const string & path) {
        ifstream is(path);

        if (!is.is_open()) {
            Logging::Core->warning("Failed to open file {}", path);
            return nullptr;
        }

        auto scene = make_shared<Scene>();
        unique_ptr<TempModel> tmpModel;
        string mtllib;

        vector<glm::vec3> av;
        vector<glm::vec2> avt;
        vector<glm::vec3> avn;

        int nRead;
        string line;
        string str;
        for (size_t line_no = 0; getline(is, line); line_no++) {
            if (line.empty() || line[0] == '#')
                continue;

            switch (line[0]) {
                case 'o':
                    if (tmpModel)
                        scene->models.push_back(tmpModel->toModel(av, avt, avn));
                    tmpModel = make_unique<TempModel>(line.substr(2));
                    break;
                case 'v':
                    switch (line[1]) {
                        case ' ': // v
                            glm::vec3 v;
                            str = line.substr(2);
                            nRead = std::sscanf(str.c_str(), "%f %f %f", &v.x,
                                                &v.y, &v.z);
                            ASSERT_NREAD(3, 'v');
                            av.push_back(v);
                            break;
                        case 't': // vt
                            glm::vec2 vt;
                            str = line.substr(3);
                            nRead =
                                std::sscanf(str.c_str(), "%f %f", &vt.x, &vt.y);
                            ASSERT_NREAD(2, "vt");
                            avt.push_back(vt);
                            break;
                        case 'n': // vn
                            glm::vec3 vn;
                            str = line.substr(3);
                            nRead = std::sscanf(str.c_str(), "%f %f %f", &vn.x,
                                                &vn.y, &vn.z);
                            ASSERT_NREAD(3, "vn");
                            avn.push_back(vn);
                            break;
                    }
                    break;
                case 'f':
                    TempModel::Face f;
                    str = line.substr(2);
                    nRead = std::sscanf(
                        str.c_str(), "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu",
                        &f.p[0].v, &f.p[0].t, &f.p[0].n, &f.p[1].v, &f.p[1].t,
                        &f.p[1].n, &f.p[2].v, &f.p[2].t, &f.p[2].n);
                    tmpModel->af.push_back(f);
                    break;
                case 'm': // mtllib
                    if (line.size() < 8)
                        PARSE_ERROR("mtllib")
                    mtllib = line.substr(7);
                    break;
                case 'u': // usemtl
                    if (line.size() < 8)
                        PARSE_ERROR("usemtl")
                    tmpModel->mtl = line.substr(7);
                    break;
                default:
                    break;
            }
        }

        if (tmpModel)
            scene->models.push_back(tmpModel->toModel(av, avt, avn));

        {
            auto i = path.find_last_of('/');
            if (i != std::string::npos) {
                i++;
                auto n = path.size() - 4 - i;
                scene->name = path.substr(i, n);
            }
        }

        return scene;
    }
};

#undef PARSE_ERROR
#undef ASSERT_NREAD