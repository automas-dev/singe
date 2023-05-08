#include <spdlog/spdlog.h>

#include <glpp/extra/debug.hpp>

#include "Game.hpp"

int main() {
    spdlog::set_level(spdlog::level::trace);

    try {
        Window window("Scene");

        glpp::extra::initDebug();

        Game game(window);
        game.Start();
    }
    catch (std::runtime_error & e) {
        SPDLOG_ERROR("Game threw a runtime_error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
