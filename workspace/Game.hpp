#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <s3e.hpp>
#include <thread>
#include <vector>
using namespace Tom::s3e;

class Game : public GameBase {
    DefaultResourceManager resManager;
    FPSDisplay::Ptr fps;
    Texture::Ptr devTexture;
    Model::Ptr floorModel;
    Model::Ptr objectModel;
    std::unique_ptr<Physics> physics;

    void loadObjects();

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    bool onCreate(void) override;
    void onDestroy(void) override;

    void onKeyPressed(const sf::Event::KeyEvent & e) override;
    void onKeyReleased(const sf::Event::KeyEvent & e) override;
    void onMouseMove(const sf::Event::MouseMoveEvent & e) override;
    void onMouseDown(const sf::Event::MouseButtonEvent & e) override;
    void onMouseUp(const sf::Event::MouseButtonEvent & e) override;
    void onResized(const sf::Event::SizeEvent & e) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
