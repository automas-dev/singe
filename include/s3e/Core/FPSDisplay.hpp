#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

namespace Tom::s3e {

    /**
     * An SFML Text that shows calculates and shows the frame rate
     */
    class FPSDisplay : public sf::Text {
        int frames;
        float time;
        float fps;
        float rate;

        /**
         * Update the sf::Text object with the latest fps value.
         */
        void updateLabel();

    public:
        using Ptr = std::shared_ptr<FPSDisplay>;
        using ConstPtr = std::shared_ptr<const FPSDisplay>;

        /**
         * Create a new FPSDisplay.
         */
        FPSDisplay();

        /**
         * Destructor for FPSDisplay.
         */
        virtual ~FPSDisplay();

        /**
         * Return the lates fps.
         *
         * @return the lates fps
         */
        float getFPS();

        /**
         * Set the update rate for the text display.
         *
         * @param delta the time between updates
         */
        void setRate(float delta);

        /**
         * Update and refresh display.
         *
         * This method increments the frame count by one and uses delta as the
         * total time for that frame
         *
         * @param delta the delta time
         */
        void update(const sf::Time & delta);
    };
}
