#include "SceneParse.hpp"

#include <fstream>
#include <rapidxml.hpp>
#include <singe/Support/Util.hpp>
#include <singe/Support/log.hpp>
#include <stdexcept>

using namespace rapidxml;

namespace singe::scene {
    using std::make_shared;
    using std::stof;
    using std::stoi;

    static string traceNode(const xml_node<char> * node) {
        if (!node)
            return "NO TRACE";

        string trace(node->name(), node->name_size());
        while (node->parent()) {
            node = node->parent();
            trace = string(node->name(), node->name_size()) + " > " + trace;
        }

        return trace;
    }

#define _STR2(x) #x
#define _STR(x) _STR2(x)

#define ERROR(node, msg)                  \
    throw SceneParseError(traceNode(node) \
                          + " :" __FILE__ ":"_STR(__LINE__) " " + (msg))

#define PTR_CHECK(node)                      \
    {                                        \
        if (!(node))                         \
            ERROR(node, "received nullptr"); \
    }

    static vec4 parseVec4(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto parts = splitString(string(node->value(), node->value_size()));
        if (parts.size() < 6)
            throw SceneParseError(traceNode(node) + " : must have 4 components");

        return vec4(stof(parts[0]), stof(parts[1]), stof(parts[2]), stof(parts[3]));
    }

    static Pose parsePose(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto parts = splitString(string(node->value(), node->value_size()));
        if (parts.size() < 6)
            throw SceneParseError(traceNode(node) + " : must have 6 components");

        return Pose(vec3(stof(parts[0]), stof(parts[1]), stof(parts[2])),
                    vec3(stof(parts[3]), stof(parts[4]), stof(parts[5])));
    }

    static Camera::Projection::Mode parseMode(const xml_node<char> * node) {
        PTR_CHECK(node);

        string value(node->value(), node->value_size());
        if (value == "orthographic")
            return Camera::Projection::ORTHOGRAPHIC;

        else if (value == "perspective")
            return Camera::Projection::PERSPECTIVE;

        else
            ERROR(node, "invalid value " + value);
    }

    static Camera::Projection parseProjection(const xml_node<char> * node) {
        PTR_CHECK(node);

        Camera::Projection projection;

        auto * mode_node = node->first_node("mode");
        if (mode_node)
            projection.mode = parseMode(mode_node);

        auto * fov_node = node->first_node("fov");
        if (fov_node) {
            string fov_str(fov_node->value(), fov_node->value_size());
            projection.fov = stof(fov_str);
        }

        auto * near_node = node->first_node("near");
        if (near_node) {
            string near_str(near_node->value(), near_node->value_size());
            projection.near = stof(near_str);
        }

        auto * far_node = node->first_node("far");
        if (far_node) {
            string far_str(far_node->value(), far_node->value_size());
            projection.far = stof(far_str);
        }

        return projection;
    }

    static Camera parseCamera(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());
        Camera camera(name);

        auto * pose_node = node->first_node("pose");
        if (pose_node)
            camera.pose = parsePose(pose_node);

        auto * projection_node = node->first_node("projection");
        if (projection_node)
            camera.projection = parseProjection(projection_node);

        return camera;
    }

    static Shader::Uniform::Type uniformType(const string & value) {
        if (value == "bool")
            return Shader::Uniform::BOOL;

        if (value == "int")
            return Shader::Uniform::INT;

        if (value == "uint")
            return Shader::Uniform::UINT;

        if (value == "float")
            return Shader::Uniform::FLOAT;

        if (value == "vec2")
            return Shader::Uniform::VEC2;

        if (value == "vec3")
            return Shader::Uniform::VEC3;

        if (value == "vec4")
            return Shader::Uniform::VEC4;

        if (value == "mat2")
            return Shader::Uniform::MAT2;

        if (value == "mat3")
            return Shader::Uniform::MAT3;

        if (value == "mat4")
            return Shader::Uniform::MAT4;

        throw SceneParseError("Unknown uniform type " + value);
    }

    static Shader::Uniform parseUniform(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());
        Shader::Uniform uniform(name);

        auto * type_attr = node->first_attribute("type");
        if (!type_attr)
            ERROR(node, "missing type attribute");

        string type_str(type_attr->value(), type_attr->value_size());
        uniform.type = uniformType(type_str);

        uniform.defaultValue = string(node->value(), node->value_size());

        return uniform;
    }

    static Shader parseShader(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());
        Shader shader(name);

        auto * type_attr = node->first_attribute("name");
        if (type_attr) {
            string type(name_attr->value(), name_attr->value_size());
            shader.type = type;
        }

        auto * uniform_node = node->first_node("uniform");
        while (uniform_node) {
            shader.uniforms.emplace_back(parseUniform(uniform_node));
            uniform_node = uniform_node->next_sibling("uniform");
        }

        return shader;
    }

    static Model::Mesh parseMesh(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());
        Model::Mesh mesh(name);

        // Add any attrib or node parsing here, if Mesh gets anything

        return mesh;
    }

    static Model parseModel(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto * mesh_node = node->first_node("mesh");
        if (!mesh_node)
            ERROR(node, "missing mesh node");

        Model::Mesh mesh = parseMesh(mesh_node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());
        Model model(name, mesh);

        // TODO: shader

        return model;
    }

    static Grid parseGrid(const xml_node<char> * node) {
        PTR_CHECK(node);

        Grid grid;

        auto * name_attr = node->first_attribute("name");
        if (name_attr) {
            string name(name_attr->value(), name_attr->value_size());
            grid.name = name;
        }

        auto * size_node = node->first_node("size");
        if (size_node) {
            string size_str(size_node->value(), size_node->value_size());
            grid.size = stoi(size_str);
        }

        auto * color_node = node->first_node("color");
        if (color_node)
            grid.color = parseVec4(color_node);

        return grid;
    }

    static Defs parseDefs(const xml_node<char> * root) {
        PTR_CHECK(root);

        Defs defs;

        auto * cameras = root->first_node("cameras");
        if (cameras) {
            auto * camera_node = cameras->first_node("camera");
            while (camera_node) {
                defs.cameras.emplace_back(
                    make_shared<Camera>(parseCamera(camera_node)));
                camera_node = camera_node->next_sibling("camera");
            }
        }

        auto * shaders = root->first_node("shaders");
        if (shaders) {
            auto * shader_node = shaders->first_node("shader");
            while (shader_node) {
                defs.shaders.emplace_back(
                    make_shared<Shader>(parseShader(shader_node)));
                shader_node = shader_node->next_sibling("shader");
            }
        }

        auto * models = root->first_node("models");
        if (models) {
            auto * model_node = models->first_node("model");
            while (model_node) {
                defs.models.emplace_back(
                    make_shared<Model>(parseModel(model_node)));
                model_node = model_node->next_sibling("model");
            }
        }

        auto * grids = root->first_node("grids");
        if (grids) {
            auto * grid_node = grids->first_node("grid");
            while (grid_node) {
                defs.grids.emplace_back(make_shared<Grid>(parseGrid(grid_node)));
                grid_node = grid_node->next_sibling("grid");
            }
        }

        return defs;
    }


    class SceneReader {
    public:
        SceneReader();

        shared_ptr<Scene> parse(const xml_node<char> * root) {
            auto * nameAttr = root->first_attribute("name");
            if (!nameAttr) {
                Logging::Resource->error("Root scene has no name attribute");
                return nullptr;
            }

            string name(nameAttr->value(), nameAttr->value_size());
            shared_ptr<Scene> scene = make_shared<Scene>(nullptr, name);
            if (scene) {
            }
            return scene;
        }
    };
}

namespace singe {
    using std::ifstream;
    using std::istreambuf_iterator;
    using std::make_shared;

    shared_ptr<scene::Scene> SceneResourceManager::loadScene(const string & name) {
        Logging::Resource->debug("SceneResourceManager::loadScene {}", name);
        static fs::path subPath("scene");

        fs::path fullPath = resourceAt(subPath / name);
        Logging::Resource->trace("Full path is {}", fullPath);

        ifstream is(fullPath);
        if (!is.is_open()) {
            Logging::Resource->error("Failed to open scene file {}", fullPath);
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
            Logging::Resource->error("Root scene has no name attribute");
            return nullptr;
        }

        string sceneName(nameAttr->value(), nameAttr->value_size());
        shared_ptr<scene::Scene> scene =
            make_shared<scene::Scene>(nullptr, sceneName);

        parseDefs(scene, root);

        // TODO: everything else

        return scene;
    }
}

namespace singe::scene {
    SceneStruct parseScene(const std::string & file, const ResourceManager & res) {
        std::ifstream is(res.resourceAt(file));
        if (!is.is_open()) {
            throw std::runtime_error("Failed to open file " + file);
        }
        std::string body((std::istreambuf_iterator<char>(is)),
                         std::istreambuf_iterator<char>());

        xml_document<> doc;
        doc.parse<0>(body.data());

        SceneStruct ss;

        auto * scene = doc.first_node("scene");

        for (auto * node = scene->first_node(); node; node = node->next_sibling()) {
            std::string name(node->name(), node->name_size());
            if (name == "grid") {
                int n = 100;
                float step = 1.0;
                glm::vec4 color(1);
                bool fade = false;
                auto * p = node->first_node("n");
                if (p) {
                    std::string value(p->value(), p->value_size());
                    n = std::stoi(value);
                }
                p = node->first_node("step");
                if (p) {
                    std::string value(p->value(), p->value_size());
                    step = std::stof(value);
                }
                auto * mat = node->first_node("material");
                if (mat) {
                    auto * p = mat->first_node("color");
                    if (p) {
                        std::string value(p->value(), p->value_size());
                        auto split = splitString(value, ' ');
                        color.r = std::stof(split[0]);
                        color.g = std::stof(split[1]);
                        color.b = std::stof(split[2]);
                        color.a = std::stof(split[3]);
                    }
                }
                ss.grid = std::make_shared<Grid>(n, step, color, fade);
            }
        }

        return ss;
    }
}