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

#define CATCH_STR_ERROR(TYPE)                                                              \
    catch (std::invalid_argument const & e) {                                              \
        throw ParseError(format("Failed to parse " TYPE " on line {}", el->GetLineNum())); \
    }                                                                                      \
    catch (std::out_of_range const & e) {                                                  \
        throw ParseError(format("Failed to parse " TYPE " on line {}", el->GetLineNum())); \
    }

namespace fast {
    using std::stoi;
    using std::stof;
    using std::make_shared;
    using glm::vec3;
    using glm::vec4;
    using fmt::format;

    static int parseInt(const XMLElement * el) {
        assert(el != nullptr);

        try {
            return stoi(el->GetText());
        }
        CATCH_STR_ERROR("int")
    }

    static float parseFloat(const XMLElement * el) {
        assert(el != nullptr);

        try {
            return stof(el->GetText());
        }
        CATCH_STR_ERROR("float")
    }

    static vec3 parseVec3(const XMLElement * el) {
        assert(el != nullptr);

        auto parts = split(el->GetText());
        if (parts.size() != 3) {
            throw ParseError(format("Vec3 on line {} must have 3 parts, got {}", el->GetLineNum(), parts.size()));
        }

        try {
            return vec3 {
                stof(parts[0]),
                stof(parts[1]),
                stof(parts[2]),
            };
        }
        CATCH_STR_ERROR("vec3")
    }

    static vec4 parseVec4(const XMLElement * el) {
        assert(el != nullptr);

        auto parts = split(el->GetText());
        if (parts.size() != 4) {
            throw ParseError(format("Vec4 on line {} must have 4 parts, got {}", el->GetLineNum(), parts.size()));
        }

        try {
            return vec4 {
                stof(parts[0]),
                stof(parts[1]),
                stof(parts[2]),
                stof(parts[3]),
            };
        }
        CATCH_STR_ERROR("vec4")
    }

    Grid::Ptr parseGrid(const XMLElement * el) {
        int size = 10;
        vec4 color(1);

        auto * sizeEl = el->FirstChildElement("size");
        if (sizeEl)
            size = parseInt(sizeEl);

        auto * colorEl = el->FirstChildElement("color");
        if (colorEl)
            color = parseVec4(colorEl);
        
        return make_shared<Grid>(size, color, true);
    }

    singe::Scene::Ptr parseScene(const XMLDocument * doc) {
        assert(doc != nullptr);

        auto * root = doc->FirstChildElement("scene");
        if (!root) {
            throw ParseError("Failed to find root scene node");
        }

        singe::Scene::Ptr scene = make_shared<singe::Scene>();
        if (scene) {
            auto * grid = root->FirstChildElement("grid");
            if (grid) {
                scene->grid = parseGrid(grid);
                if (grid->NextSiblingElement("grid")) {
                    fmt::print("WARNING you have more than 1 grid\n");
                }
            }
        }
        return scene;
    }
}
