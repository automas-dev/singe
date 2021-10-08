# SFML 3D  Engine

My implementation of C++ support classes and functions for 3D rendering using
OpenGL and SFML.


### Project Goals

This project is meant to provide a quick setup, with all of the basic features
of an opengl project already implemented, so that I can work on new opengl / 3d
rendering concepts faster.

The features implemented here are not meant to be full featured, but provide
the basic functionality with minimal setup. This means having any required
resources built-in, and having sensible defaults.

1. Fast setup
2. No external resources
    - Default font loaded from library binary
    - Default shader loaded from library binary
3. Provide basic functionality
    - Window and Events
    - Resource loading
    - Physics / Collision Detection
    - Textures / Shading / FrameBuffer / VBO
    - Camera Controls
    - Menu System
4. Provide access to underlying implementation for finer control
5. Sensible Defaults


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

```sh
git submodule add https://gitlab.com/twh2898/sfml-s3e.git
```

CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.15.0)
project(app VERSION 0.1.0)

add_subdirectory(sfml-s3e EXCLUDE_FROM_ALL)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE s3e)
```

main.cpp
```cpp
#include <s3e/Core.hpp>
using namespace Tom::s3e;

class App : public GameBase {

public:
    App() : GameBase() { }
    ~App() { }

    void onCreate(void) {
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


## Examples

Example programs can be found in the [examples](examples) folder.

- [Demo](examples/demo)
- [Geometry Shader](examples/geometry)
- [Instanced Rendering](examples/instanced)
- [Outline Shader](examples/outline)
- [Physics](examples/physics)
- [Scenes](examples/scenes)
- [Stencil](examples/stencil)
- [Voxel](examples/voxel)


## Demo

```cpp
#include <s3e/Core.hpp>
#include <memory>
using namespace Tom::s3e;

class Game : public GameBase {
    ResourceManager res;
    FPSDisplay::Ptr fps;
    Scene::Ptr scene;

public:
    Game(const sf::String & resPath) : GameBase(), res(resPath) { }
    ~Game() { }

    void onCreate(void) {
        // defautFont loaded from memory by GameBase

        fps = std::make_shared<FPSDisplay>();
        fps->setFont(uiFont);
        fps->setRate(0.1f);

        camera->move({3, 2, 1});
        camera->rotate({30, -70});
        camera->setFov(80);

        menu->addMenuItem("Resume", [&]() {
            menu->hide();
            SetMouseGrab(true);
        });
        menu->addMenuItem("Exit", [&]() {
            window->close();
        });

        camera->move({5, 2, 5});
        camera->rotateEuler({0, -1, 0});
        camera->setFov(70);
        
        // defaultShader loaded by GameBase

        scene = std::make_shared<Scene>("Root");

        auto cubeScene = res.loadScene("model/cube.obj");
        if (!cubeScene)
            throw std::runtime_error("Failed to load model/cube.obj");
        cubeScene->move({3, 0, 0});

        scene->children.push_back(cubeScene);
        // Send vertices to the GPU
        scene->send();

        SetMouseGrab(true);
    }
    void onDestroy(void) {
    }
    void onUpdate(const sf::Time & delta) {
        float deltaS = delta.asSeconds();
        fps->update(delta);
    }
    void onDraw() const {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

        defaultShader->bind();
        defaultShader->setMat4("mvp", vp);
        scene->draw(defaultShader);

        window->pushGLStates();
        window->draw(*fps);
        window->popGLStates();
    }
};

int main() {
    spdlog::set_level(spdlog::level::debug);
    // res is in the project root, one up from the build folder
    try {
        Game game("../res/");
        if (game.Create("Window"))
            game.Start();
    }
    catch (std::runtime_error & e) {
        SPDLOG_ERROR("runtime_error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```
