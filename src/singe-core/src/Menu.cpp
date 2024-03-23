#include "singe/Core/Menu.hpp"

#include <singe/Support/log.hpp>

const sf::Color light(200, 200, 200);
const sf::Color dark(10, 10, 10);

namespace singe {

    MenuItem::MenuItem(function<void()> callback) : callback(callback) {}

    MenuItem::~MenuItem() {}

    void MenuItem::setCallback(function<void(void)> callback) {
        this->callback = callback;
    }

    void MenuItem::onClick() const {
        Logging::Graphics->info("MenuItem {} was clicked, calling callback",
                                string(getString()));
        callback();
    }

    bool MenuItem::contains(const sf::Vector2f & point) const {
        auto tPoint = getTransform().getInverse().transformPoint(point);
        return getLocalBounds().contains(tPoint);
    }
}

namespace singe {
    using std::make_shared;

    Menu::Menu() : isMouseDown(false), visible(false) {}

    Menu::Menu(const sf::Font & font)
        : isMouseDown(false), visible(false), font(font) {
        title.setFont(this->font);
        title.setCharacterSize(48);
    }
    Menu::Menu(const sf::Font & font, const string & title)
        : isMouseDown(false), visible(false), font(font) {
        this->title.setString(title);
        this->title.setFont(this->font);
        this->title.setCharacterSize(48);
    }

    Menu::~Menu() {}

    void Menu::setFont(const sf::Font & font) {
        this->font = font;
        this->title.setFont(this->font);
        for (auto & item : items) {
            item->setFont(this->font);
        }
    }

    void Menu::setTitle(const string & text) {
        this->title.setString(text);
    }

    void Menu::show() {
        Logging::Graphics->info("Menu is shown");
        visible = true;
    }

    void Menu::hide() {
        Logging::Graphics->info("Menu is hidden");
        visible = false;
    }

    bool Menu::isVisible() const {
        return visible;
    }

    bool Menu::addMenuItem(const string & text, function<void()> callback) {
        auto menuItem = make_shared<MenuItem>(callback);

        menuItem->setFont(this->font);
        menuItem->setString(text);
        menuItem->setCharacterSize(24);
        menuItem->setFillColor(light);
        menuItem->setOrigin(menuItem->getLocalBounds().left,
                            menuItem->getLocalBounds().top);

        if (items.empty()) {
            menuItem->setPosition(0, title.getGlobalBounds().height * 2);
        }
        else {
            auto & item = items.back();
            menuItem->setPosition(
                0,
                item->getGlobalBounds().top + item->getCharacterSize() * 1.5);
        }

        items.push_back(menuItem);
        return true;
    }

    void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const {
        if (!visible)
            return;

        states.transform *= getTransform();

        auto size = sf::Vector2f(target.getSize().x, target.getSize().y);
        sf::RectangleShape background(size);
        background.setFillColor(sf::Color(0, 0, 0, 100));

        target.draw(background);

        target.draw(title, states);

        for (auto & item : items) {
            target.draw(*item, states);
        }
    }

    void Menu::onMouseMove(const sf::Event::MouseMoveEvent & e) {
        if (!visible)
            return;

        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        if (isMouseDown)
            return;

        for (auto & item : items) {
            if (item->contains(point))
                item->setFillColor(sf::Color::Red);
            else
                item->setFillColor(light);
        }
    }

    void Menu::onMouseDown(const sf::Event::MouseButtonEvent & e) {
        if (!visible)
            return;

        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        isMouseDown = true;

        for (auto & item : items) {
            if (item->contains(point))
                item->setFillColor(sf::Color::Green);
        }
    }

    void Menu::onMouseUp(const sf::Event::MouseButtonEvent & e) {
        if (!visible)
            return;

        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        isMouseDown = false;

        for (auto & item : items) {
            if (item->contains(point))
                item->onClick();
        }
    }
}
