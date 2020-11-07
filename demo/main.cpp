#include <iostream>
using namespace std;

#include "Game.hpp"

int main() {
    try {
        Game game ("../../demo/res/");
        if (game.Create("My Game"))
            game.Start();
    }
    catch (std::runtime_error & e) {
        std::cout << "Game failed, exiting" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
