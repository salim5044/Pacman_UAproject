#include "Game.h"
#include "Menustate.h"
#include "Stopwatch.h"
#include "Config.h"

Game::Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pac-Man Project") {
    window.setFramerateLimit(60);

    // Start with menu state
    auto menuState = std::make_shared<Menustate>(window);
    stateManager.pushState(menuState);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();

        Stopwatch::getInstance().tick();
        float dt = Stopwatch::getInstance().getDeltaTime();

        update(dt);
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Let the state manager handle all input
        stateManager.handleInput(event);
    }
}

void Game::update(float dt) {
    // State manager handles all updates
    stateManager.update(dt);
}

void Game::render() {
    window.clear();
    stateManager.draw(window);
    window.display();
}