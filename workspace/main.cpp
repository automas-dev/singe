#include <s3e/Support/log.hpp>

#include "Game.hpp"

static Logger logSupport("Support");

int main() {
    logSupport.warning("Support here 1");
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_level(spdlog::level::debug);
    logSupport.warning("Support here 2");
    try {
        Game game("../../workspace/res/");
    logSupport.warning("Support here 3");
        if (game.Create("Workspace"))
            game.Start();
    }
    catch (std::runtime_error & e) {
        SPDLOG_ERROR("Game threw a runtime_error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
