#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "StateManager.h"

class Game {
private:
    sf::RenderWindow window;
    StateManager stateManager;

public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float dt);
    void render();
};