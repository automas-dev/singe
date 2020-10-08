#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

#include "Game.hpp"
using namespace game;

inline void modeProjection(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

inline void modeModel(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "My Window", sf::Style::Default | sf::Style::Fullscreen,
                            settings);

    window.setVerticalSyncEnabled(true); // call it once, after creating the window
    window.setFramerateLimit(60); // call it once, after creating the window
    window.setActive();
    window.setKeyRepeatEnabled(false);

    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    // glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "glewInit failed: " << glewGetErrorString(err) << endl;
        return EXIT_FAILURE;
    }

    sf::Font font;
    if (!font.loadFromFile("./Questrial_Regular.ttf")) {
        cerr << "Failed to load font" << endl;
        return EXIT_FAILURE;
    }

    Game::Ptr game;
    try {
        game = Game::create(window, font);
    }
    catch (std::runtime_error & e) {
        std::cout << "Game failed, exiting" << std::endl;
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    sf::Clock clock;

    // glClearColor(0.2, 0.5, 0.7, 1.0);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    game->onKeyPressed(event.key);
                    break;
                case sf::Event::KeyReleased:
                    game->onKeyReleased(event.key);
                    break;
                case sf::Event::MouseMoved:
                    game->onMouseMove(event.mouseMove);
                    break;
                case sf::Event::MouseButtonPressed:
                    game->onMouseDown(event.mouseButton);
                    break;
                case sf::Event::MouseButtonReleased:
                    game->onMouseUp(event.mouseButton);
                    break;
                case sf::Event::Closed:
                    break;
                case sf::Event::Resized: {
                        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                        window.setView(sf::View(visibleArea));
                        glViewport(0, 0, event.size.width, event.size.height);
                        game->onResized(event.size);
                    }
                    break;
            }
        }

        sf::Time delta = clock.restart();

        game->update(delta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modeProjection();
        float aspect = window.getSize().x / window.getSize().y;
        gluPerspective(93, aspect, 0.01, 100.0);
        modeModel();

        game->draw();

        window.display();
    }

    return 0;
}
