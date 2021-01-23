#include "s3e/Keymap.hpp"

namespace Tom::s3e {
    Keymap::Keymap() : currentSet(std::make_shared<KeySet>()) {
        states.fill(false);
        last_states = states;
    }

    Keymap::~Keymap() { }

    bool Keymap::isKeyValid(sf::Keyboard::Key key) {
        return key != sf::Keyboard::Unknown;
    }

    void Keymap::onKeyPressed(const sf::Event::KeyEvent & event) {
        if (isKeyValid(event.code)) {
            states[event.code] = true;

            currentSet->addKey(event.code);
        }
    }

    void Keymap::onKeyReleased(const sf::Event::KeyEvent & event) {
        if (isKeyValid(event.code)) {
            states[event.code] = false;

            currentChord.addSet(currentSet);
            currentSet = std::make_shared<KeySet>();

            if (currentIsAction()) {
                sendActionEvent();
            }
        }
    }

    void Keymap::update() {
        last_states = states;
    }

    bool Keymap::addAction(const std::string & name, sf::Keyboard::Key key) {
        if (isKeyValid(key)) {
            actions[name] = key;
            return true;
        }
        else {
            return false;
        }
    }

    bool Keymap::isKeyPressed(sf::Keyboard::Key key) {
        return states[key];
    }

    bool Keymap::isKeyReleased(sf::Keyboard::Key key) {
        return !states[key];
    }

    bool Keymap::isKeyJustPressed(sf::Keyboard::Key key) {
        return isKeyPressed(key) && !last_states[key];
    }

    bool Keymap::isKeyJustReleased(sf::Keyboard::Key key) {
        return isKeyReleased(key) && last_states[key];
    }

    bool Keymap::isActionPressed(const std::string & action) {
        auto kv = actions.find(action);
        return kv != actions.end() && isKeyPressed(kv->second);
    }

    bool Keymap::isActionReleased(const std::string & action) {
        auto kv = actions.find(action);
        return kv != actions.end() && isKeyReleased(kv->second);
    }

    bool Keymap::isActionJustPressed(const std::string & action) {
        auto kv = actions.find(action);
        return kv != actions.end() && isKeyJustPressed(kv->second);
    }

    bool Keymap::isActionJustReleased(const std::string & action) {
        auto kv = actions.find(action);
        return kv != actions.end() && isKeyJustReleased(kv->second);
    }
}

