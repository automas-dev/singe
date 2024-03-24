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

#include <memory>

class Diamond {
    BufferArray::Ptr array;
    int n;
    glm::vec2 size;
    glm::vec4 color;
    glm::vec2 pos;

public:
    Diamond(glm::vec2 size, const glm::vec4 & color)
        : n(0),
          size(size),
          color(color),
          pos(0),
          array(std::make_shared<BufferArray>(vector<vector<Buffer::Attribute>> {
              {Buffer::Attribute {0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0}},
              {Buffer::Attribute {1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0}},
          })) {
        updateBuffer();
    }

    glm::vec2 getSize() const {
        return size;
    }

    void setSize(glm::vec2 size) {
        this->size = size;
        updateBuffer();
    }

    vec2 getPos() const {
        return pos;
    }

    void setPos(vec2 pos) {
        this->pos = pos;
        updateBuffer();
    }

    void draw() const {
        array->drawArrays(Buffer::Lines, 0, n);
    }

private:
    void updateBuffer() {
        vector<glm::vec2> vertices;
        vector<glm::vec4> colors;

        int i = 0;
        float x = pos.x;
        float y = pos.y;
        vertices.emplace_back(x - size.x, y);
        vertices.emplace_back(x, y - size.y);

        vertices.emplace_back(x, y - size.y);
        vertices.emplace_back(x + size.x, y);

        vertices.emplace_back(x + size.x, y);
        vertices.emplace_back(x, y + size.y);

        vertices.emplace_back(x, y + size.y);
        vertices.emplace_back(x - size.x, y);

        for (int j = 0; j < i; j++) {
            colors.push_back(color);
        }

        n = vertices.size();

        array->bind();
        array->bufferData(0, n * sizeof(glm::vec2), vertices.data());
        array->bufferData(1, n * sizeof(glm::vec4), colors.data());
        array->unbind();
    }
};

class Game : public GameBase {
    ResourceManager res;
    FPSDisplay fps;
    std::shared_ptr<singe::MVPShader> shader;
    std::shared_ptr<singe::Shader> circle_shader;
    Scene scene;
    glm::vec3 point;
    shared_ptr<Diamond> circle;

public:
    Game(Window::Ptr & window);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
