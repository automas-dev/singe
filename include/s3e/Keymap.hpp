#pragma once

#include <SFML/Window.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <string>

namespace Tom::s3e {

    class Mapping {
    };

    class Keymap {
        std::unordered_map<sf::Keyboard::Key, bool> states;
        std::unordered_set<sf::Keyboard::Key> handledThisFrame;
        std::unordered_set<sf::Keyboard::Key> justPressed;
        std::unordered_set<sf::Keyboard::Key> justReleased;
        std::unordered_map<std::string, sf::Keyboard::Key> actions;
        std::unordered_map<sf::Keyboard::Key, Mapping> mapping;

        bool wasHandled(const sf::Keyboard::Key & key);

    public:
        Keymap(void);
        ~Keymap();

        void onKeyPressed(const sf::Event::KeyEvent & event);

        void onKeyReleased(const sf::Event::KeyEvent & event);

        void update();

        bool isKeyPressed(const sf::Keyboard::Key & key);
        bool isKeyJustPressed(const sf::Keyboard::Key & key);

        bool isActionPressed(const std::string & key);
        bool isActionJustPressed(const std::string & key);

        bool isKeyReleased(const sf::Keyboard::Key & key);
        bool isKeyJustReleased(const sf::Keyboard::Key & key);

        bool isActionReleased(const std::string & key);
        bool isActionJustReleased(const std::string & key);
    };
}

