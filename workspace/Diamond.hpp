#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/ResourceManager.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Graphics/Model.hpp>
#include <singe/Graphics/Scene.hpp>
#include <singe/Graphics/Shader.hpp>
#include <singe/Support/log.hpp>
using namespace singe;

#include <glm/glm.hpp>
#include <glpp/Buffer.hpp>
#include <glpp/extra/Shapes.hpp>
#include <memory>
#include <vector>
using glpp::BufferArray;
using glpp::Buffer;


class Diamond {
    glpp::extra::Line::Ptr line;
    glm::vec2 size;
    glm::vec4 color;
    glm::vec2 pos;

    Camera * camera;

    void updateLine() {
        std::vector<glm::vec3> points;

        float x = pos.x;
        float y = pos.y;

        points.emplace_back(x - size.x, y, 0);
        points.emplace_back(x, y - size.y, 0);
        points.emplace_back(x, y - size.y, 0);
        points.emplace_back(x + size.x, y, 0);
        points.emplace_back(x + size.x, y, 0);
        points.emplace_back(x, y + size.y, 0);
        points.emplace_back(x, y + size.y, 0);
        points.emplace_back(x - size.x, y, 0);

        line->setPoints(std::move(points));
    }

    glm::vec2 reverseProject(const glm::vec3 & point, const glm::mat4 & mvp) {
        auto projPoint = mvp * vec4(point, 1.0);
        projPoint /= projPoint.w;
        return vec3(projPoint.x, projPoint.y, 0.0);
    }

public:
    Diamond(const glm::vec2 & size, const glm::vec4 & color, Camera * camera)
        : line(std::make_shared<glpp::extra::Line>(glm::vec4(1.0, 0.0, 0.0, 1.0))),
          size(size),
          color(color),
          pos(0),
          camera(camera) {
        updateLine();
    }

    const glm::vec2 & getSize() const {
        return size;
    }

    void setSize(const glm::vec2 & size) {
        this->size = size;
        updateLine();
    }

    glm::vec2 getPos() const {
        return pos;
    }

    void setPos(const glm::vec2 & pos) {
        this->pos = pos;
        updateLine();
    }

    void setPos(const glm::vec3 & point) {
        glm::mat4 vp = camera->projMatrix() * camera->viewMatrix();
        setPos(reverseProject(point, vp));
        float r =
            (float)camera->getScreenSize().x / (float)camera->getScreenSize().y;

        setSize({size.x, size.x * r});
    }

    void draw() const {
        line->draw(glm::mat4(1.0));
    }
};
