#pragma once

#include <SFML/Window.hpp>
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Tom::s3e {

    // TODO: Serialize / De-serialize mappings
    // TODO: Include Ctrl / Alt / Shift modifiers in action mapping
    // TODO: Allow multiple key bindings
    // TODO: Allow read / modify mappings
    // TODO: Handle key combinations (eg. H+C)
    // TODO: Handle key chords (eg. C-K C-D)

    /**
     * Manage keyboard events and keyboard mapping through actions.
     */
    class Keymap {

        std::array<bool, sf::Keyboard::KeyCount> states;
        std::array<bool, sf::Keyboard::KeyCount> last_states;
        std::unordered_map<std::string, sf::Keyboard::Key> actions;

        /**
         * Check if key is equal to sf::Keyboard::Unknown.
         *
         * @return key != sf::Keyboard::Unknown
         */
        bool isKeyValid(sf::Keyboard::Key key);

    public:
        /**
         * Construct a new Keymap.
         */
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
