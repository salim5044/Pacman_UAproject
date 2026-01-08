#include "Game.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "\n========================================\n";
        std::cerr << "ERROR: Game initialization failed!\n";
        std::cerr << "========================================\n";
        std::cerr << e.what() << "\n";
        std::cerr << "========================================\n\n";
        std::cerr << "The game cannot start without required files.\n";
        std::cerr << "Please check that all assets are in place.\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "\n========================================\n";
        std::cerr << "UNEXPECTED ERROR\n";
        std::cerr << "========================================\n";
        std::cerr << "Exception: " << e.what() << "\n";
        std::cerr << "========================================\n";
        return 1;
    }
    catch (...) {
        std::cerr << "\n========================================\n";
        std::cerr << "CRITICAL ERROR\n";
        std::cerr << "========================================\n";
        std::cerr << "Unknown exception occurred!\n";
        std::cerr << "========================================\n";
        return 1;
    }

    return 0;
}