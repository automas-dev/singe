#include <spdlog/spdlog.h>

#include "Game.hpp"

int main() {
    spdlog::set_level(spdlog::level::trace);

    try {
        Window window("Workspace");
        Game game(window, "../../workspace/res/");
        game.Start();
    }
    catch (std::runtime_error & e) {
        SPDLOG_ERROR("Game threw a runtime_error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
