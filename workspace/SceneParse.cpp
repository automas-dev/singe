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
    using std::move;

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
                          + " :" __FILE__ ":" _STR(__LINE__) " " + (msg))

#define PTR_CHECK(node)                      \
    {                                        \
        if (!(node))                         \
            ERROR(node, "received nullptr"); \
    }

    static vec3 parseVec3(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto parts = splitString(string(node->value(), node->value_size()));
        if (parts.size() < 3)
            throw SceneParseError(traceNode(node) + " : must have 3 components");

        return vec3(stof(parts[0]), stof(parts[1]), stof(parts[2]));
    }

    static vec4 parseVec4(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto parts = splitString(string(node->value(), node->value_size()));
        if (parts.size() < 4)
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

    static Transform parseTransform(const xml_node<char> * node) {
        PTR_CHECK(node);

        Transform transform;

        auto * pos_node = node->first_node("position");
        if (pos_node)
            transform.pos = parseVec3(pos_node);

        auto * rot_node = node->first_node("rotation");
        if (rot_node)
            transform.rot = parseVec3(rot_node);

        auto * scale_node = node->first_node("scale");
        if (scale_node)
            transform.scale = parseVec3(scale_node);

        return transform;
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

        auto * type_attr = node->first_attribute("type");
        if (!type_attr)
            ERROR(node, "missing type attribute");

        string type_str(type_attr->value(), type_attr->value_size());
        auto type = uniformType(type_str);

        string value(node->value(), node->value_size());

        Shader::Uniform uniform(name, type, value);
        return uniform;
    }

    static Shader::Source parseSource(const xml_node<char> * node) {
        PTR_CHECK(node);

        auto * type_attr = node->first_attribute("type");
        if (!type_attr)
            ERROR(node, "missing type attribute");

        string type(type_attr->value(), type_attr->value_size());

        auto * path_attr = node->first_attribute("path");
        if (!path_attr)
            ERROR(node, "missing path attribute");

        string path(path_attr->value(), path_attr->value_size());

        return Shader::Source(type, path);
    }

    static Shader parseShader(const xml_node<char> * node, Scene & parent) {
        PTR_CHECK(node);

        // Check for ref

        auto * ref_attr = node->first_attribute("ref");
        if (ref_attr) {
            string ref(ref_attr->value(), ref_attr->value_size());
            return parent.findShader(ref);
        }

        // Create Shader

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());

        auto * type_attr = node->first_attribute("type");
        if (!type_attr)
            ERROR(node, "missing type attribute");

        string type(type_attr->value(), type_attr->value_size());

        Shader shader(name, type);

        auto * source_node = node->first_node("source");
        while (source_node) {
            shader.source.emplace_back(parseSource(source_node));
            source_node = source_node->next_sibling("source");
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

        auto * path_attr = node->first_attribute("path");
        if (!path_attr)
            ERROR(node, "missing path attribute");

        string path(path_attr->value(), path_attr->value_size());
        Model::Mesh mesh(path);

        return mesh;
    }

    static Model parseModel(const xml_node<char> * node, Scene & parent) {
        PTR_CHECK(node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());

        auto * mesh_node = node->first_node("mesh");
        if (!mesh_node)
            ERROR(node, "missing mesh node");

        Model::Mesh mesh = parseMesh(mesh_node);

        auto * shader_node = node->first_node("shader");
        if (!shader_node)
            ERROR(node, "missing shader node");
        Shader shader = parseShader(shader_node, parent);

        Model model(name, mesh, shader);

        auto * transform_node = node->first_node("transform");
        if (transform_node)
            model.transform = parseTransform(transform_node);

        return model;
    }

    static Grid parseGrid(const xml_node<char> * node) {
        PTR_CHECK(node);

        Grid grid;

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

    static shared_ptr<Scene> parseScene(const xml_node<char> * node,
                                        shared_ptr<Scene> parent) {
        PTR_CHECK(node);

        auto * name_attr = node->first_attribute("name");
        if (!name_attr)
            ERROR(node, "missing name attribute");

        string name(name_attr->value(), name_attr->value_size());

        shared_ptr<Scene> scene = make_shared<Scene>(parent, name);

        auto * transform_node = node->first_node("transform");
        if (transform_node)
            scene->transform = parseTransform(transform_node);

        auto * grid_node = node->first_node("grid");
        if (grid_node)
            scene->grid = make_shared<Grid>(parseGrid(grid_node));

        auto * camera_node = node->first_node("camera");
        while (camera_node) {
            scene->cameras.emplace_back(parseCamera(camera_node));
            camera_node = camera_node->next_sibling("camera");
        }

        // IMPORTANT: Shaders must be loaded before models
        auto * shader_node = node->first_node("shader");
        while (shader_node) {
            scene->shaders.emplace_back(parseShader(shader_node, *scene));
            shader_node = shader_node->next_sibling("shader");
        }

        auto * model_node = node->first_node("model");
        while (model_node) {
            scene->models.emplace_back(parseModel(model_node, *scene));
            model_node = model_node->next_sibling("model");
        }

        auto * scene_node = node->first_node("scene");
        while (scene_node) {
            scene->children.emplace_back(parseScene(scene_node, scene));
            scene_node = scene_node->next_sibling("scene");
        }

        return scene;
    }

    Shader & Scene::findShader(const string & name) {
        for (auto & shader : shaders) {
            if (shader.name == name) {
                return shader;
            }
        }
        if (parent) {
            return parent->findShader(name);
        }
        throw SceneParseError("Unable to find shader " + name);
    }
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

        try {
            return scene::parseScene(root, nullptr);
        }
        catch (const scene::SceneParseError & e) {
            Logging::Resource->error("Failed to parse scene: {}", e.what());
        }

        return nullptr;
    }
}
