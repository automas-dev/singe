#pragma once

#include <SFML/Window.hpp>
#include <memory>
#include <unordered_map>
#include <array>
#include <string>

namespace Tom::s3e {

    class Keymap {
        std::array<bool, sf::Keyboard::KeyCount> states;
        std::array<bool, sf::Keyboard::KeyCount> last_states;
        std::unordered_map<std::string, sf::Keyboard::Key> actions;

        bool isKeyValid(sf::Keyboard::Key key);
        bool isJustHandled(sf::Keyboard::Key key);

    public:
        Keymap(void);
        ~Keymap();

        void onKeyPressed(const sf::Event::KeyEvent & event);
        void onKeyReleased(const sf::Event::KeyEvent & event);

        void update(void);

        bool addAction(const std::string & name, sf::Keyboard::Key key);

        bool isKeyPressed(sf::Keyboard::Key key);
        bool isKeyReleased(sf::Keyboard::Key key);

        bool isKeyJustPressed(sf::Keyboard::Key key);
        bool isKeyJustReleased(sf::Keyboard::Key key);

        bool isActionPressed(const std::string & key);
        bool isActionReleased(const std::string & key);

        bool isActionJustPressed(const std::string & key);
        bool isActionJustReleased(const std::string & key);
    };
}

