#pragma once

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <singe/Graphics/ResourceManager.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace singe::scene {
    using std::string;
    using std::vector;
    using std::map;
    using std::shared_ptr;

    using glm::vec3;
    using glm::vec4;

    class SceneParseError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    struct Pose {
        vec3 pos;
        vec3 rot;

        Pose(const vec3 & pos = vec3(0), const vec3 & rot = vec3(0))
            : pos(pos), rot(rot) {}
    };

    struct Transform {
        vec3 pos;
        vec3 rot;
        vec3 scale;

        Transform(const vec3 & pos = vec3(0),
                  const vec3 & rot = vec3(0),
                  const vec3 & scale = vec3(1))
            : pos(pos), rot(rot), scale(scale) {}
    };

    struct Camera {
        struct Projection {
            enum Mode {
                ORTHOGRAPHIC,
                PERSPECTIVE,
            };

            Mode mode;
            float fov;
            float near;
            float far;

            Projection(float fov = 80,
                       float near = 0.01,
                       float far = 1000,
                       Mode mode = PERSPECTIVE)
                : fov(fov), near(near), far(far), mode(mode) {}
        };

        string name;
        Pose pose;
        Projection projection;

        Camera(const string & name) : name(name) {}
    };

    struct Shader {
        struct Uniform {
            enum Type {
                BOOL,
                INT,
                UINT,
                FLOAT,
                VEC2,
                VEC3,
                VEC4,
                MAT2,
                MAT3,
                MAT4,
            };

            string name;
            Type type;
            string defaultValue;

            Uniform(const string & name) : name(name) {}
        };

        string name;
        string type;
        vector<Uniform> uniforms;

        Shader(const string & name) : name(name) {}
    };

    struct Material {
        // TODO: material
    };

    struct Model {
        struct Mesh {
            string name;

            Mesh(const string & name) : name(name) {}
        };

        string name;
        Mesh mesh;
        Transform transform;
        shared_ptr<Shader> shader;
        // shared_ptr<Material> material;

        Model(const string & name, const Mesh & mesh)
            : name(name), mesh(mesh) {}
    };

    struct Grid {
        string name;
        int size;
        vec4 color;

        Grid(const string & name = "", int size = 10, const vec4 & color = vec4(1))
            : name(name), size(size), color(color) {}
    };

    struct Defs {
        vector<shared_ptr<Camera>> cameras;
        vector<shared_ptr<Shader>> shaders;
        vector<shared_ptr<Model>> models;
        vector<shared_ptr<Grid>> grids;
    };

    struct Scene {
        shared_ptr<Scene> parent;
        string name;
        Defs defs;
        Transform transform;
        vector<shared_ptr<Camera>> cameras;
        vector<shared_ptr<Shader>> shaders;
        vector<shared_ptr<Model>> models;
        vector<shared_ptr<Grid>> grids;
        vector<shared_ptr<Scene>> children;

        Scene(const shared_ptr<Scene> & parent, const string & name)
            : parent(parent), name(name) {}
    };

    class SceneReader {
    public:
        SceneReader();

        shared_ptr<Scene> parse(const string & name);
    };
}

namespace singe {
    class SceneResourceManager : public ResourceManager {
    public:
        using ResourceManager::ResourceManager;

        shared_ptr<scene::Scene> loadScene(const string & name);
    };
}
