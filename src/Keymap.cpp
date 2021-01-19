#include "s3e/Keymap.hpp"

namespace Tom::s3e {
    Keymap::Keymap() { }

    Keymap::~Keymap() { }

    bool Keymap::wasHandled(const sf::Keyboard::Key & key) {
        return handledThisFrame.find(key) != handledThisFrame.end();
    }

    void Keymap::onKeyPressed(const sf::Event::KeyEvent & event) {
        if (!states[event.code] && !wasHandled(event.code))
            justPressed.insert(event.code);
        states[event.code] = true;
        handledThisFrame.insert(event.code);
    }

    void Keymap::onKeyReleased(const sf::Event::KeyEvent & event) {
        if (states[event.code] && !wasHandled(event.code))
            justReleased.insert(event.code);
        states[event.code] = false;
        handledThisFrame.insert(event.code);
    }

    void Keymap::update() {
        handledThisFrame.clear();
        justPressed.clear();
        justReleased.clear();
    }

    bool Keymap::isKeyPressed(const sf::Keyboard::Key & key) {
        return states[key];
    }

    bool Keymap::isKeyJustPressed(const sf::Keyboard::Key & key) {
        return justPressed.find(key) != justPressed.end();
    }

    bool Keymap::isActionPressed(const std::string & action) {
        auto key = actions[action];
        return states[key];
    }

    bool Keymap::isActionJustPressed(const std::string & action) {
        auto key = actions[action];
        return justPressed.find(key) != justPressed.end();
    }

    bool Keymap::isKeyReleased(const sf::Keyboard::Key & key) {
        return !states[key];
    }

    bool Keymap::isKeyJustReleased(const sf::Keyboard::Key & key) {
        return justReleased.find(key) != justReleased.end();
    }

    bool Keymap::isActionReleased(const std::string & action) {
        auto key = actions[action];
        return !states[key];
    }

    bool Keymap::isActionJustReleased(const std::string & action) {
        auto key = actions[action];
        return justReleased.find(key) != justReleased.end();
    }
}

