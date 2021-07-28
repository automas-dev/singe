#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

namespace Tom::s3e {

    class FPSDisplay : public sf::Text {
        int frames;
        float time;
        float fps;
        float rate;

        void updateLabel(void);

    public:
        typedef std::shared_ptr<FPSDisplay> Ptr;
        typedef std::shared_ptr<const FPSDisplay> ConstPtr;

        FPSDisplay(void);
        virtual ~FPSDisplay();

        float getFPS(void);

        void setRate(float delta);

        void update(const sf::Time & delta);
    };
}
