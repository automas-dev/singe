#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <functional>
#include <string>
#include <sstream>
#include <memory>
using namespace std;
#include "vbo.hpp"

const sf::Color light (200, 200, 200);
const sf::Color dark (10, 10, 10);

class MenuItem : public sf::Text {
    std::function<void(void)> callback;

public:

    void setCallback(std::function<void(void)> callback) {
        this->callback = callback;
    }

    void click() {
        callback();
    }

    bool contains(sf::Vector2f point) {
        auto tPoint = getTransform().getInverse().transformPoint(point);
        return getLocalBounds().contains(tPoint);
    }
};

class Menu : public sf::Drawable, public sf::Transformable {
    sf::Font font;
    sf::Text title;
    std::list<MenuItem> items;
    bool isMouseDown = false;

public:
    bool isVisible = false;

    Menu(sf::Font font) : font(font) {
        title.setFont(this->font);
        title.setCharacterSize(48);
    }

    void setTitle(std::string text) {
        this->title.setString(text);
    }

    void show() {
        isVisible = true;
    }

    void hide() {
        isVisible = false;
    }

    void addMenuItem(std::string text, std::function<void(void)> callback) {
        MenuItem menuItem;
        menuItem.setCallback(callback);
        menuItem.setFont(this->font);
        menuItem.setString(text);
        menuItem.setCharacterSize(24);
        menuItem.setFillColor(light);
        menuItem.setOrigin(menuItem.getLocalBounds().left, menuItem.getLocalBounds().top);
        if (items.empty()) {
            menuItem.setPosition(0, title.getGlobalBounds().height * 2);
        }
        else {
            auto & item = items.back();
            menuItem.setPosition(0, item.getGlobalBounds().top + item.getGlobalBounds().height * 2);
        }
        items.push_back(std::move(menuItem));
    }

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override {
        if (!isVisible)
            return;

        states.transform *= getTransform();

        auto size = sf::Vector2f(target.getSize().x, target.getSize().y);
        sf::RectangleShape background (size);
        background.setFillColor(sf::Color(0, 0, 0, 100));

        target.draw(background);

        target.draw(title, states);

        for (auto & item : items) {
            target.draw(item, states);
        }
    }

    void onMouseMove(sf::Event::MouseMoveEvent e) {
        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        if (isMouseDown)
            return;

        for (auto & item : items) {
            if (item.contains(point))
                item.setFillColor(sf::Color::Red);
            else
                item.setFillColor(light);
        }
    }

    void onMouseDown(sf::Event::MouseButtonEvent e) {
        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        isMouseDown = true;

        for (auto & item : items) {
            if (item.contains(point))
                item.setFillColor(sf::Color::Green);
        }
    }

    void onMouseUp(sf::Event::MouseButtonEvent e) {
        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        isMouseDown = false;

        for (auto & item : items) {
            if (item.contains(point))
                item.click();
        }
    }
};

class Camera {
    sf::Vector2u screen;

public:
    glm::vec2 rot;
    glm::vec3 pos;

    Camera() : rot(0, 0), pos(0, 0, 0) { }

    void setScreen(sf::Vector2u screen) {
        this->screen = screen;
    }

    /**
     * Rotate on the x an y axis in degrees.
     */
    void rotate(float x, float y) {
        rot -= glm::vec2(x, y);
        rot.x = glm::clamp(rot.x, -89.0f, 89.0f);
    }

    void moveLinear(float x, float y, float z) {
        pos += glm::vec3(x, y, z);
    }

    void move(float x, float y, float z) {
        float yRot = glm::radians(rot.y);

        float dz = -x * std::sin(yRot) + z * std::cos(yRot);
        float dx = -x * -std::cos(yRot) + z * std::sin(yRot);

        float dy = y;

        pos += glm::vec3(dx, dy, dz);
    }

    void pushTransform() {
        glPushMatrix();
        glScalef(1.0f,(float)screen.x/screen.y,1.0f);
        glRotatef(-this->rot.x, 1, 0, 0);
        glRotatef(-this->rot.y, 0, 1, 0);
        glScalef(1.0f,(float)screen.y/screen.x,1.0f);
        glTranslatef(-this->pos.x, -this->pos.y, -this->pos.z);
    }

    void popTransform() {
        glPopMatrix();
    }
};

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

    // sf::Vector2f lastMouse (200, 200);
    sf::Vector2i lastMouse (200, 200);
    sf::Mouse::setPosition(lastMouse, window);

    Menu menu (font);
    menu.setTitle("Game");
    menu.setPosition(300, 300);
    menu.addMenuItem("New", [&]() {
        menu.hide();
        window.setMouseCursorGrabbed(true);
        window.setMouseCursorVisible(false);
        // sf::Mouse::setPosition(lastMouse, window);
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
        // sf::Vector2f mouseDelta;

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    menu.show();
                    window.setMouseCursorGrabbed(false);
                    window.setMouseCursorVisible(true);
                    // window.close();
                    break;
                }
                break;
            case sf::Event::MouseMoved:
                menu.onMouseMove(event.mouseMove);
                // if (!menu.isVisible)
                // {
                //     mouseDelta.x += event.mouseMove.x - lastMouse.x;
                //     mouseDelta.y += event.mouseMove.y - lastMouse.y;
                // }
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
        
        // TODO: update

        if (!menu.isVisible) {
            auto mPos = sf::Mouse::getPosition(window);
            sf::Vector2f mDelta (mPos.x - lastMouse.x, mPos.y - lastMouse.y);
            cam.rotate(mDelta.y * 0.2, mDelta.x * 0.2);
            sf::Mouse::setPosition(lastMouse, window);

            int x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            int z = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            cam.move(x * 0.2, 0, z * 0.2);
        }

        // window.clear(dark);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modeProjection();
        float aspect = window.getSize().x / window.getSize().y;
        gluPerspective(63, aspect, 0.01, 100.0);
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
