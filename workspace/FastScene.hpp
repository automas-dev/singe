#pragma once

#include <tinyxml2.h>

#include <glpp/extra/Grid.hpp>
#include <singe/Graphics/Scene.hpp>
#include <stdexcept>

#include "Node.hpp"

namespace fast {
    using tinyxml2::XMLDocument;
    using tinyxml2::XMLElement;
    using glpp::extra::Grid;

    class ParseError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class OutOfMemory : public ParseError {
    public:
        using ParseError::ParseError;
    };

    class FastScene {
        Root::Ptr activeRoot;

        CameraNode::Ptr parseCamera(const XMLElement * el);

        ImageNode::Ptr parseImage(const XMLElement * el);

        MaterialNode::Ptr parseMaterial(const XMLElement * el);

        MeshNode::Ptr parseMesh(const XMLElement * el);

        Node::Ptr parseScene(const XMLElement * el);

    public:
        FastScene() {}

        FastScene(FastScene && other) = delete;
        FastScene(const FastScene &) = delete;

        FastScene & operator=(FastScene && other) = delete;
        FastScene & operator=(const FastScene &) = delete;

        Root::Ptr loadScene(const XMLElement * rootElement);

        Root::Ptr loadFile(const string_view & path);
    };
}
