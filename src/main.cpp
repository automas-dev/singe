#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

#include "vbo.hpp"
#include "Camera.hpp"
#include "Menu.hpp"
using namespace game;

void drawGrid(int steps=10) {
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 1.0, 1.0);
        for (int x = -steps; x <= steps; x++) {
            if (x == 0) {
                glVertex3f(0, 0, -steps);
                glVertex3f(0, 0, 0);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, steps);
            }
            else {
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(x, 0, -steps);
                glVertex3f(x, 0, steps);
            }
        }
        for (int z = -steps; z <= steps; z++) {
            if (z == 0) {
                glVertex3f(-steps, 0, 0);
                glVertex3f(0, 0, 0);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3f(0, 0, 0);
                glVertex3f(steps, 0, 0);
            }
            else {
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(-steps, 0, z);
                glVertex3f(steps, 0, z);
            }
        }
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, steps, 0);
    }
    glEnd();
}

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

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "My Window", sf::Style::Default | sf::Style::Fullscreen, settings);

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

    sf::Vector2i lastMouse (200, 200);
    sf::Mouse::setPosition(lastMouse, window);

    Menu menu (font);
    menu.setTitle("Game");
    menu.setPosition(300, 300);
    menu.addMenuItem("New", [&]() {
        menu.hide();
        window.setMouseCursorGrabbed(true);
        window.setMouseCursorVisible(false);
    });
    menu.addMenuItem("Load", []() {});
    menu.addMenuItem("Options", []() {});
    menu.addMenuItem("Exit", [&]() {
        window.close();
    });

    Camera cam;
    cam.setScreen(window.getSize());
    cam.move(3, 2, 1);

    sf::Clock clock;

    glClearColor(0.2, 0.5, 0.7, 1.0);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    menu.show();
                    window.setMouseCursorGrabbed(false);
                    window.setMouseCursorVisible(true);
                    break;
                }
                break;
            case sf::Event::MouseMoved:
                menu.onMouseMove(event.mouseMove);
                break;
            case sf::Event::MouseButtonPressed:
                menu.onMouseDown(event.mouseButton);
                break;
            case sf::Event::MouseButtonReleased:
                menu.onMouseUp(event.mouseButton);
                break;
            case sf::Event::KeyReleased:
                break;
            case sf::Event::Closed:
                break;
            case sf::Event::Resized:
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                glViewport(0, 0, event.size.width, event.size.height);
                cam.setScreen(window.getSize());
                break;
            }
        }

        sf::Time delta = clock.restart();
        float deltaS = delta.asSeconds();
        
        // TODO: update

        if (!menu.isVisible) {
            auto mPos = sf::Mouse::getPosition(window);
            sf::Vector2f mDelta (mPos.x - lastMouse.x, mPos.y - lastMouse.y);
            cam.rotate(mDelta.y * 0.2, mDelta.x * 0.2);
            sf::Mouse::setPosition(lastMouse, window);

            int x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            int y = sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
            int z = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            cam.moveLook(x * deltaS, y * deltaS, z * deltaS);
        }

        // window.clear(dark);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modeProjection();
        float aspect = window.getSize().x / window.getSize().y;
        gluPerspective(93, aspect, 0.01, 100.0);
        modeModel();

        cam.pushTransform();
        drawGrid(20);
        cam.popTransform();

        window.pushGLStates();
        window.draw(menu);
        window.popGLStates();

        window.display();
    }

    return 0;
}
