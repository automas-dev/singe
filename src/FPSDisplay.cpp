#include "s3e/FPSDisplay.hpp"

#include <spdlog/fmt/fmt.h>

const sf::Color light(200, 200, 200);

namespace Tom::s3e {
    FPSDisplay::FPSDisplay() : frames(0), time(0.0f), rate(0.1f) {
        setString("FPS: N/A");
        setCharacterSize(14);
        setFillColor(light);
        setOrigin(getLocalBounds().left, getLocalBounds().top);
        setPosition(10, 10);
    }

    FPSDisplay::~FPSDisplay() {}

    float FPSDisplay::getFPS() {
        return fps;
    }

    void FPSDisplay::updateLabel() {
        setString(fmt::format("FPS: {:.2f}", fps));
    }

    void FPSDisplay::setRate(float delta) {
        rate = delta;
    }

    void FPSDisplay::update(const sf::Time & delta) {
        frames++;
        time += delta.asSeconds();

        if (time > rate) {
            fps = (float)frames / time;
            updateLabel();
            frames = 0;
            time = 0.0f;
        }
    }
}
