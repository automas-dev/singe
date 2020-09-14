#include "Menu.hpp"

const sf::Color light (200, 200, 200);
const sf::Color dark (10, 10, 10);

namespace game {

    MenuItem::MenuItem() { }

    MenuItem::~MenuItem() { }

    void MenuItem::setCallback(std::function<void(void)> callback) {
        this->callback = callback;
    }

    void MenuItem::click() {
        callback();
    }

    bool MenuItem::contains(sf::Vector2f point) {
        auto tPoint = getTransform().getInverse().transformPoint(point);
        return getLocalBounds().contains(tPoint);
    }

    MenuItem::Ptr MenuItem::create() {
        auto item = std::make_shared<MenuItem>();
        return item;
    }
}

namespace game {

    Menu::Menu() { }

    Menu::Menu(const sf::Font & font) : font(font) {
        title.setFont(this->font);
        title.setCharacterSize(48);
    }
    Menu::Menu(const sf::Font & font, const std::string &title) : font(font) {
        this->title.setString(title);
        this->title.setFont(this->font);
        this->title.setCharacterSize(48);
    }

    Menu::~Menu() { }

    void Menu::setFont(const sf::Font &font) {
        this->font = font;
        this->title.setFont(this->font);
        for (auto item : items) {
            item.setFont(this->font);
        }
    }

    void Menu::setTitle(const std::string &text) {
        this->title.setString(text);
    }

    void Menu::show() {
        isVisible = true;
    }

    void Menu::hide() {
        isVisible = false;
    }

    void Menu::addMenuItem(const std::string &text, std::function<void(void)> callback) {
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

    void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const {
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

    void Menu::onMouseMove(sf::Event::MouseMoveEvent e) {
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

    void Menu::onMouseDown(sf::Event::MouseButtonEvent e) {
        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        isMouseDown = true;

        for (auto & item : items) {
            if (item.contains(point))
                item.setFillColor(sf::Color::Green);
        }
    }

    void Menu::onMouseUp(sf::Event::MouseButtonEvent e) {
        auto point = getTransform().getInverse().transformPoint(e.x, e.y);
        isMouseDown = false;

        for (auto & item : items) {
            if (item.contains(point))
                item.click();
        }
    }

    Menu::Ptr Menu::create() {
        auto menu = std::make_shared<Menu>();
        return menu;
    }

    Menu::Ptr Menu::create(const sf::Font &font) {
        auto menu = std::make_shared<Menu>(font);
        return menu;
    }

    Menu::Ptr Menu::create(const sf::Font &font, const std::string &title) {
        auto menu = std::make_shared<Menu>(font, title);
        return menu;
    }
}
