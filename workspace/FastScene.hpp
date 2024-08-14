#pragma once

#include <tinyxml2.h>

#include <glpp/extra/Grid.hpp>
#include <singe/Graphics/Scene.hpp>
#include <stdexcept>

namespace fast {
    using tinyxml2::XMLDocument;
    using tinyxml2::XMLElement;
    using glpp::extra::Grid;

    class ParseError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    Grid::Ptr parseGrid(const XMLElement * el);

    singe::Scene::Ptr parseScene(const XMLDocument * doc);
}
