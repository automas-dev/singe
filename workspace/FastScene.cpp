#include "FastScene.hpp"

#include <fmt/core.h>

#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <string_view>

using std::string;
using std::string_view;
using std::vector;

class elementCountError : public std::exception {
public:
    size_t expect;
    size_t actual;

    elementCountError(size_t expect, size_t actual)
        : expect(expect), actual(actual) {
    }
};

static vector<string> split(const string_view & str) {
    vector<string> parts;
    for (size_t i = 0; i < str.length(); i++) {
        if (isspace(str[i]))
            continue;

        size_t start = i;
        while (i < str.length() && !isspace(str[i])) i++;
        parts.emplace_back(&str[start], &str[i]);
    }
    return parts;
}

static int countSplit(const string_view & str) {
    int count = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (isspace(str[i]))
            continue;

        while (i < str.length() && !isspace(str[i])) i++;
        count++;
    }
    return count;
}

#define CATCH_PARSE_ERROR                                                                                                                               \
    catch (elementCountError const & e) {                                                                                                               \
        throw ParseError(format("Failed to parse node {} on line {} needed {} values but found {}", el->Name(), el->GetLineNum(), e.expect, e.actual)); \
    }                                                                                                                                                   \
    catch (std::invalid_argument const & e) {                                                                                                           \
        throw ParseError(format("Failed to parse node {} on line {}", el->Name(), el->GetLineNum()));                                                   \
    }                                                                                                                                                   \
    catch (std::out_of_range const & e) {                                                                                                               \
        throw ParseError(format("Failed to parse node {} on line {}", el->Name(), el->GetLineNum()));                                                   \
    }

namespace fast {
    using std::stoi;
    using std::stof;
    using std::make_shared;
    using glm::vec3;
    using glm::vec4;
    using fmt::format;

    static vec3 parseVec3(const string_view & str) {
        auto parts = split(str);
        if (parts.size() != 3)
            throw elementCountError(3, parts.size());

        return vec3 {
            stof(parts[0]),
            stof(parts[1]),
            stof(parts[2]),
        };
    }

    static vec4 parseVec4(const string_view & str) {
        auto parts = split(str);
        if (parts.size() != 4)
            throw elementCountError(4, parts.size());

        return vec4 {
            stof(parts[0]),
            stof(parts[1]),
            stof(parts[2]),
            stof(parts[3]),
        };
    }

    static int parseIntElement(const XMLElement * el) {
        assert(el != nullptr);

        try {
            return stoi(el->GetText());
        }
        CATCH_PARSE_ERROR
    }

    static float parseFloatElement(const XMLElement * el) {
        assert(el != nullptr);

        try {
            return stof(el->GetText());
        }
        CATCH_PARSE_ERROR
    }

    static vec3 parseVec3Element(const XMLElement * el) {
        assert(el != nullptr);

        try {
            return parseVec3(el->GetText());
        }
        CATCH_PARSE_ERROR
    }

    static vec4 parseVec4Element(const XMLElement * el) {
        assert(el != nullptr);

        try {
            return parseVec4(el->GetText());
        }
        CATCH_PARSE_ERROR
    }

    static vec4 parseColorElement(const XMLElement * el) {
        assert(el != nullptr);

        try {
            auto * text = el->GetText();
            int nParts = countSplit(text);
            if (nParts == 3)
                return vec4(parseVec3(text), 1.0);
            else if (nParts == 4)
                return parseVec4(text);
            else
                throw ParseError(format("Failed to parse node {} on line {}", el->Name(), el->GetLineNum()));
        }
        CATCH_PARSE_ERROR
    }

    static Transform parseTransformElement(const XMLElement * el) {
        assert(el != nullptr);

        Transform t;
        try {
            auto * posAttr = el->Attribute("pos");
            if (posAttr)
                t.setPosition(parseVec3(posAttr));

            auto * rotAttr = el->Attribute("rot");
            if (rotAttr) {
                int nParts = countSplit(rotAttr);
                if (nParts == 3)
                    t.setRotation({parseVec3(rotAttr)});
                else if (nParts == 4)
                    t.setRotation({parseVec4(rotAttr)});
                else
                    throw ParseError(format("Failed to parse rotation attribute of node {} on line {}", el->Name(), el->GetLineNum()));
            }

            auto * scaleAttr = el->Attribute("scale");
            if (scaleAttr) {
                int nParts = countSplit(scaleAttr);
                if (nParts == 1)
                    t.setScale(vec3(stof(scaleAttr)));
                else if (nParts == 3)
                    t.setScale(parseVec3(scaleAttr));
                else
                    throw ParseError(format("Failed to parse scale attribute of node {} on line {}", el->Name(), el->GetLineNum()));
            }
        }
        CATCH_PARSE_ERROR

        return t;
    }

    static void parseNodeInplace(const XMLElement * el, Node::Ptr node) {
        assert(el != nullptr);

        auto * name = el->Attribute("name");
        if (name)
            node->name = name;

        auto * transformEl = el->FirstChildElement("transform");
        if (transformEl)
            node->transform = parseTransformElement(transformEl);
    }

    CameraNode::Ptr FastScene::parseCamera(const XMLElement * el) {
        assert(el != nullptr);

        CameraNode::Ptr camera = make_shared<CameraNode>();
        if (!camera)
            throw OutOfMemory("Failed to create a Camera Node");

        parseNodeInplace(el, camera);

        if (camera->name.empty())
            throw ParseError(format("Camera node on line {} missing name attribute", el->GetLineNum()));

        auto * controlAttr = el->Attribute("control");
        if (controlAttr) {
            string ctl(controlAttr);

            if (ctl == "fpv")
                camera->setControl(CameraNode::FPV);

            else if (ctl == "dolly")
                camera->setControl(CameraNode::Dolly);

            else
                throw ParseError(format("Failed to parse control attribute of node {} on line {}", el->Name(), el->GetLineNum()));
        }

        auto * activeAttr = el->Attribute("active");
        if (activeAttr) {
            bool value = false;
            if (!tinyxml2::XMLUtil::ToBool(activeAttr, &value))
                throw ParseError(format("Failed to parse active attribute of node {} on line {}", el->Name(), el->GetLineNum()));
            camera->setActive(value);
        }

        auto * projectionEl = el->FirstChildElement("projection");
        if (projectionEl) {

            auto * modeAttr = projectionEl->Attribute("mode");
            if (modeAttr) {
                string mode(modeAttr);

                if (mode == "perspective")
                    camera->setProjectionMode(singe::Camera::Perspective);

                else if (mode == "orthographic")
                    camera->setProjectionMode(singe::Camera::Orthographic);

                else
                    throw ParseError(format("Failed to parse mode attribute of node {} on line {}", el->Name(), el->GetLineNum()));
            }

            try {
                auto * fovAttr = projectionEl->Attribute("fov");
                if (fovAttr)
                    camera->setFov(stof(fovAttr));

                auto * nearAttr = projectionEl->Attribute("znear");
                if (nearAttr)
                    camera->setNear(stof(nearAttr));

                auto * farAttr = projectionEl->Attribute("zfar");
                if (farAttr)
                    camera->setFar(stof(farAttr));
            }
            CATCH_PARSE_ERROR
        }

        if (activeRoot->hasCamera(camera->name))
            throw ParseError(format("Camera already exists with name {}, line {}", camera->name, el->GetLineNum()));
        activeRoot->addCamera(camera);
        return camera;
    }

    ImageNode::Ptr FastScene::parseImage(const XMLElement * el) {
        assert(el != nullptr);

        ImageNode::Ptr image = make_shared<ImageNode>();
        if (!image)
            throw OutOfMemory("Failed to create an Image Node");

        auto * refAttr = el->Attribute("ref");
        if (refAttr) {
            if (!activeRoot->hasImage(refAttr))
                throw ParseError(format("Reference to unknown image {} on line {}", refAttr, el->GetLineNum()));
            auto ref = activeRoot->instanceImage(refAttr);
            assert(ref);
            return ref;
        }

        parseNodeInplace(el, image);

        if (image->name.empty())
            throw ParseError(format("Image node on line {} missing name attribute", el->GetLineNum()));

        auto * pathAttr = el->Attribute("path");
        if (!pathAttr)
            throw ParseError(format("Missing path attribute of node {} on line {}", el->Name(), el->GetLineNum()));
        image->setPath(pathAttr);

        auto * channelsAttr = el->Attribute("channels");
        if (channelsAttr) {
            string channels(channelsAttr);
            if (channels == "gray")
                image->setChannels(glpp::Texture::Format::Gray);

            else if (channels == "rgb")
                image->setChannels(glpp::Texture::Format::RGB);

            else if (channels == "rgba")
                image->setChannels(glpp::Texture::Format::RGBA);

            else
                throw ParseError(format("Failed to parse channels attribute of node {} on line {}", el->Name(), el->GetLineNum()));
        }

        activeRoot->addImage(image);
        return image;
    }

    MaterialNode::Ptr FastScene::parseMaterial(const XMLElement * el) {
        assert(el != nullptr);

        MaterialNode::Ptr material = make_shared<MaterialNode>();
        if (!material)
            throw OutOfMemory("Failed to create a Material Node");

        auto * refAttr = el->Attribute("ref");
        if (refAttr) {
            if (!activeRoot->hasMaterial(refAttr))
                throw ParseError(format("Reference to unknown material {} on line {}", refAttr, el->GetLineNum()));
            auto ref = activeRoot->instanceMaterial(refAttr);
            assert(ref);
            return ref;
        }

        parseNodeInplace(el, material);

        if (material->name.empty())
            throw ParseError(format("Material node on line {} missing name attribute", el->GetLineNum()));

        // TODO parse material elements (each with value or image override)

        activeRoot->addMaterial(material);
        return material;
    }

    MeshNode::Ptr FastScene::parseMesh(const XMLElement * el) {
        assert(el != nullptr);

        MeshNode::Ptr mesh = make_shared<MeshNode>();
        if (!mesh)
            throw OutOfMemory("Failed to create a Mesh Node");

        auto * refAttr = el->Attribute("ref");
        if (refAttr) {
            if (!activeRoot->hasMesh(refAttr))
                throw ParseError(format("Reference to unknown mesh {} on line {}", refAttr, el->GetLineNum()));
            auto ref = activeRoot->instanceMesh(refAttr);
            assert(ref);
            return ref;
        }

        parseNodeInplace(el, mesh);

        if (mesh->name.empty())
            throw ParseError(format("Mesh node on line {} missing name attribute", el->GetLineNum()));

        auto * pathAttr = el->Attribute("path");
        if (!pathAttr)
            throw ParseError(format("Missing path attribute of node {} on line {}", el->Name(), el->GetLineNum()));
        mesh->setPath(pathAttr);

        auto * visibleAttr = el->Attribute("visible");
        if (visibleAttr) {
            bool value = false;
            if (!tinyxml2::XMLUtil::ToBool(visibleAttr, &value))
                throw ParseError(format("Failed to parse visible attribute of node {} on line {}", el->Name(), el->GetLineNum()));
            mesh->setVisible(value);
        }

        auto * materialEl = el->FirstChildElement("material");
        if (materialEl)
            mesh->setMaterial(parseMaterial(materialEl));

        activeRoot->addMesh(mesh);
        return mesh;
    }

    Node::Ptr FastScene::parseScene(const XMLElement * el) {
        static int sceneDefault = 1;

        assert(el != nullptr);

        Node::Ptr scene = make_shared<Node>();
        if (!scene)
            throw OutOfMemory("Failed to create a Node");

        parseNodeInplace(el, scene);

        for (el = el->FirstChildElement(); el != nullptr; el = el->NextSiblingElement()) {
            string name(el->Name());

            Node::Ptr child;
            if (name == "scene")
                child = parseScene(el);

            else if (name == "camera")
                child = parseCamera(el);

            else if (name == "image")
                child = parseImage(el);

            else if (name == "material")
                child = parseMaterial(el);

            else if (name == "mesh")
                child = parseMesh(el);

            else if (name == "environment")
                continue; // TODO Implement this later

            else if (name == "transform")
                continue; // Handled in parseNodeInplace

            else
                throw ParseError(format("Found unknown element {} while parsing scene on line {}", name, el->GetLineNum()));

            assert(child);
            scene->addChild(child);
        }

        return scene;
    }

    Root::Ptr FastScene::loadScene(const XMLElement * rootElement) {
        assert(rootElement != nullptr);

        Root::Ptr root = activeRoot = make_shared<Root>();
        if (!root)
            throw OutOfMemory("Failed to create a Scene Root");

        try {
            root->rootScene = parseScene(rootElement);
        }
        catch (ParseError const & error) {
            root.reset();
            activeRoot.reset();
            throw;
        }

        activeRoot.reset();
        return root;
    }

    Root::Ptr FastScene::loadFile(const string_view & path) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(path.data());
        if (doc.ErrorID()) {
            return nullptr;
        }

        auto rootEl = doc.FirstChildElement("root");
        return loadScene(rootEl);
    }
}
