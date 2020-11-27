#include <s3e/log.hpp>
#include "Game.hpp"

int main() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_level(spdlog::level::debug);
    try {
        Game game ("../../demo/res/");
        if (game.Create("My Game"))
            game.Start();
    }
    catch (std::runtime_error & e) {
        SPDLOG_ERROR("Game threw a runtime_error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
