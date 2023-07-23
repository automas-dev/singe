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

        Camera(const string & name,
               Projection projection = Projection(),
               Pose pose = Pose())
            : name(name), projection(projection), pose(pose) {}
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
            string value;

            Uniform(const string & name, Type type, const string & value = "")
                : name(name), type(type), value(value) {}
        };

        struct Source {
            string type;
            string path;

            Source(const string & type, const string & path)
                : type(type), path(path) {}
        };

        string name;
        string type;
        vector<Source> source;
        vector<Uniform> uniforms;

        Shader(const string & name, const string & type) : name(name) {}
    };

    struct Model {
        struct Mesh {
            string path;

            Mesh(const string & path) : path(path) {}
        };

        string name;
        Transform transform;
        Mesh mesh;
        Shader shader;

        Model(const string & name,
              const Mesh & mesh,
              const Shader & shader,
              const Transform & transform = Transform())
            : name(name), mesh(mesh), shader(shader), transform(transform) {}
    };

    struct Grid {
        int size;
        vec4 color;

        Grid(int size = 10, const vec4 & color = vec4(1))
            : size(size), color(color) {}
    };

    struct Scene {
        shared_ptr<Scene> parent;
        string name;
        Transform transform;
        shared_ptr<Grid> grid;
        vector<Camera> cameras;
        vector<Shader> shaders;
        vector<Model> models;
        vector<shared_ptr<Scene>> children;

        /// Find a shader by ref name
        Shader & findShader(const string & name);

        Scene(const shared_ptr<Scene> & parent, const string & name)
            : parent(parent), name(name) {}
    };
}

namespace singe {
    class SceneResourceManager : public ResourceManager {
    public:
        using ResourceManager::ResourceManager;

        shared_ptr<scene::Scene> loadScene(const string & name);
    };
}
