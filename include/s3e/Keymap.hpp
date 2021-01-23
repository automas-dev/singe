#pragma once

#include <SFML/Window.hpp>
#include <memory>
#include <unordered_map>
#include <array>
#include <vector>
#include <string>

namespace Tom::s3e {

    // TODO: Serialize / De-serialize mappings
    // TODO: Include Ctrl / Alt / Shift modifiers in action mapping
    // TODO: Allow multiple key bindings
    // TODO: Allow read / modify mappings
    // TODO: Handle key combinations (eg. H+C)
    // TODO: Handle key chords (eg. C-K C-D)
    
    /*
     * Cases
     *
     * Single Keys (A single key is pressed at once)
     * S
     * Space
     * RAlt
     *
     * KeySet (A group of keys all pressed at the same time)
     *
     * Ctrl-C
     * Ctrl-X
     * H-C
     * X-C
     * M-G
     *
     * KeyChord (A sequence of key sets)
     *
     * Ctrl-K + Ctrl-R
     * Ctrl-K + Ctrl-B
     * Ctrl-K + B
     * H-I + D
     * H-I + Ctrl-X
     * G + I + H
     *
     *  When the first key is pressed, the action is started.
     *  If the action is held, it continues to be active.
     *
     *  When the last key is released, the action is completed.
     *
     *  Keys used for multiple actions (eg. w for move and w+k for
     *  something) will stop the w action and start the w+k action.
     *  If a key is held (eg. w) and anther action is pressed (eg.
     *  h + c) then the w action will continue and the h + c will
     *  be ignored, since it does not start with the w key.
     */

    /*
     * On first key pressed:
     *  Start key chord
     *
     * On last key released:
     *  Stop key chord and
     *
     * On key press:
     *  Add key to current key set (multiple keys held at once)
     *
     *  if another key is pressed:
     *      start a new key set in the current key chord
     *
     * On key release:
     *  Stop the current key set
     *  If action matches, send event
     */

    /*
     * Subscribe to:
     *
     * Single Key Events
     * Key Set Events
     * Key Chord Events
     * Action Events
     */

    /**
     * Manage keyboard events and keyboard mapping through actions.
     */
    class Keymap {
        struct KeySet {

            /// A single combination of keys
            std::vector<sf::Keyboard::Key> keys;

            void addKey(sf::Keyboard::Key key) {
                keys.push_back(key);
            }

            void removeKey(sf::Keyboard::Key key) {
                for (auto iter = keys.begin(); iter != keys.end(); iter++) {
                    if (*iter == key) {
                        keys.erase(iter);
                        break;
                    }
                }
            }

            bool hasKeys() { return !keys.empty(); }
        };

        struct KeyChord {

            /// Sequence of key combinations
            std::vector<std::shared_ptr<KeySet>> sets;

            void addSet(std::shared_ptr<KeySet> & set) {
                auto newSet = std::make_shared<KeySet>();
                for (auto key : set->keys) {
                    newSet->keys.push_back(key);
                }
                sets.push_back(newSet);
            }
            
            bool hasKeys() { return !sets.empty(); }
        };

        struct ActionMapping {

            /// KeyChords for a single Action
            std::vector<KeyChord> keyCombinations;
        };
        
        std::array<bool, sf::Keyboard::KeyCount> states;
        std::array<bool, sf::Keyboard::KeyCount> last_states;
        std::unordered_map<std::string, ActionMapping> actions;
        std::shared_ptr<KeySet> currentSet;
        KeyChord currentChord;

        /**
         * Check if key is equal to sf::Keyboard::Unknown.
         *
         * @return key != sf::Keyboard::Unknown
         */
        bool isKeyValid(sf::Keyboard::Key key);

        bool currentIsAction() { return false; }

        void sendActionEvent() { }

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

