#include <s3e/log.hpp>
#include "Game.hpp"

int main() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_level(spdlog::level::debug);
    try {
        Game game ("../../workspace/res/");
        if (game.Create("Workspace"))
            game.Start();
    }
    catch (std::runtime_error & e) {
        SPDLOG_ERROR("Game threw a runtime_error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
