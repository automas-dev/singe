# SFML 3D  Engine

## Building

```sh
git clone https://gitlab.com/twh2898/sfml-s3e.git
cd sfml-s3e
mkdir build
cd build
cmake ..
make
```

## Quickstart

bash
```sh
git submodule add https://gitlab.com/twh2898/sfml-s3e.git
```

CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.15.0)
project(app VERSION 0.1.0)

add_subdirectory(sfml-s3e EXCLUDE_FROM_ALL)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE Tom::s3e)
```

main.cpp
```cpp
#include <s3e.hpp>
using namespace Tom::s3e;

class App : public GameBase {

public:
    App() : GameBase() { }
    ~App() { }

    bool onCreate(void) {
        return true;
    }
    void onDestroy(void) {
    }
    void onUpdate(const sf::Time & delta) {
    }
    void onDraw() const {
    }
};

int main() {
    App app;
    if (app.Create("App"))
        app.Start();
    return 0;
}
```

## Demo

```cpp
#include <s3e.hpp>
using namespace Tom::s3e;

class Game : public GameBase {
    DefaultResourceManager res;
    sf::Font font;
    Shader::Ptr shader;
    Model::Ptr model;

public:
    Game() : GameBase(), res("../res") { }
    ~Game() { }

    bool onCreate(void) {
        if (!font.loadFromFile(res.resourceAt("default_font.ttf"))) {
            SPDLOG_ERROR("failed to load default font");
            return false;
        }

        camera->move({3, 2, 1});
        camera->rotate({30, -70});
        camera->setFov(80);

        menu = std::make_shared<Menu>(font, "Game");
        menu->setPosition(300, 300);
        menu->addMenuItem("Resume", [&]() {
            menu->hide();
            SetMouseGrab(true);
        });
        menu->addMenuItem("Exit", [&]() {
            window->close();
        });

        shader = res.loadShader("shader/shader.vert", "shader/shader.frag");

        model = res.loadModel("model/cube.obj");
        model->setPosition({0, 0, 0});
        model->setScale({0.1, 0.1, 0.1});
        model->setRotation({M_PI/2, M_PI/3, M_PI/6});

        SetMouseGrab(true);

        return true;
    }
    void onDestroy(void) {
    }
    void onUpdate(const sf::Time & delta) {
    }
    void onDraw() const {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glm::mat4 vp = camera->projMatrix() * camera->viewMatrix();

        shader->bind();
        shader->setMat4("mvp", vp);

        shader->setMat4("model", model->modelMatrix());
        model->draw();

        shader->unbind();
    }
};

int main() {
    spdlog::set_level(spdlog::level::debug);
    Game game;
    if (game.Create("Window"))
        game.Start();
    return 0;
}
```

shader.vert
```glsl
#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 mvp;
uniform mat4 model;

void main() {
    gl_Position = mvp * model * vec4(position, 1.0);
}
```

shader.frag
```glsl
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
```

