#include "SceneParse.hpp"

#include <fstream>
#include <rapidxml.hpp>
#include <singe/Support/Util.hpp>
#include <stdexcept>
using namespace rapidxml;

namespace singe {

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
                        auto split = splitString(value);
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