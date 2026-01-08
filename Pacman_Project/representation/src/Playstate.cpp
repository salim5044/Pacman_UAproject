#include "Playstate.h"
#include "PausedState.h"
#include "StateManager.h"
#include "Gameoverstate.h"
#include "Victorystate.h"
#include "Stopwatch.h"
#include "Event.h"
#include <iostream>

Playstate::Playstate(sf::RenderWindow& win) : window(win) {

    if (!font.loadFromFile("assets/DejaVuSans.ttf")) {
        if (!font.loadFromFile("../assets/DejaVuSans.ttf")) {
            font.loadFromFile("representation/assets/DejaVuSans.ttf");
        }
    }

    camera = std::make_shared<Camera>();
    factory = std::make_shared<ConcreteFactory>(window, views, *camera);
    world = std::make_shared<World>(factory);
    world->initLevel();
}

void Playstate::onEnter() {
}

void Playstate::onExit() {
}

void Playstate::onPause() {
}

void Playstate::onResume() {
}

void Playstate::handleInput(const sf::Event& event) {

    if (event.type == sf::Event::KeyPressed) {

        // ESC to pause
        if (event.key.code == sf::Keyboard::Escape) {
            auto pauseState = std::make_shared<PausedState>(window);
            manager->pushState(pauseState);
            return;
        }

        // Pacman controls
        auto pacman = world->getPacman();
        if (pacman && !pacman->isDying()) {
            if (event.key.code == sf::Keyboard::Left)  {
                pacman->queueDirection(-1.0f, 0.0f);
            }
            if (event.key.code == sf::Keyboard::Right) {
                pacman->queueDirection(1.0f, 0.0f);
            }
            if (event.key.code == sf::Keyboard::Up)    {
                pacman->queueDirection(0.0f, -1.0f);
            }
            if (event.key.code == sf::Keyboard::Down)  {
                pacman->queueDirection(0.0f, 1.0f);
            }
        }
    }
}

void Playstate::update(float dt) {
    GameEvent tickEvent(EventType::GameTick, dt);
    for (auto& view : views) {
        view->onNotify(tickEvent);
    }

    world->update();

    // Check for game over or victory - let THIS state decide transition
    if (world->isGameOver() && !gameOverHandled) {
        gameOverHandled = true;
        int finalScore = world->getScore();
        Score::saveHighscore(finalScore);
        auto gameOverState = std::make_shared<Gameoverstate>(finalScore, window);
        manager->changeState(gameOverState);
    }
    else if (world->isGameWon() && !gameOverHandled) {
        gameOverHandled = true;
        int finalScore = world->getScore();
        Score::saveHighscore(finalScore);
        auto victoryState = std::make_shared<Victorystate>(finalScore, window);
        manager->changeState(victoryState);
    }

    // Reset flag when game is running again
    if (!world->isGameOver() && !world->isGameWon()) {
        gameOverHandled = false;
    }
}

void Playstate::draw(sf::RenderWindow& window) {
    for (auto& view : views) {
        view->draw();
    }

    sf::Text hudText;
    hudText.setFont(font);
    hudText.setCharacterSize(20);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(10, 10);

    std::string hudString = "Score: " + std::to_string(world->getScore()) +
                           "  Lives: " + std::to_string(world->getLives()) +
                           "  Level: " + std::to_string(world->getCurrentLevel());

    if (world->isFearMode()) {
        hudString += "  FEAR: " + std::to_string(static_cast<int>(world->getFearModeTimeLeft())) + "s";
    }

    hudText.setString(hudString);
    window.draw(hudText);
}