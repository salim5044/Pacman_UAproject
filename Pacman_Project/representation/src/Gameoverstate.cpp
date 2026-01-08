#include "Gameoverstate.h"
#include "StateManager.h"
#include "Menustate.h"
#include <stdexcept>

Gameoverstate::Gameoverstate(int score, sf::RenderWindow& win)
    : finalScore(score), window(win) {
    bool loaded = false;
    if (font.loadFromFile("assets/DejaVuSans.ttf")) {
        loaded = true;
    } else if (font.loadFromFile("../assets/DejaVuSans.ttf")) {
        loaded = true;
    } else if (font.loadFromFile("representation/assets/DejaVuSans.ttf")) {
        loaded = true;
    }

    if (!loaded) {
        throw std::runtime_error(
            "FATAL ERROR: Cannot load font file!\n"
            "Tried paths:\n"
            "  - assets/DejaVuSans.ttf\n"
            "  - ../assets/DejaVuSans.ttf\n"
            "  - representation/assets/DejaVuSans.ttf\n"
            "Please ensure DejaVuSans.ttf exists in one of these locations."
        );
    }

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(250, 250);

    finalScoreText.setFont(font);
    finalScoreText.setString("Final Score: " + std::to_string(finalScore));
    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(300, 350);

    restartText.setFont(font);
    restartText.setString("Press SPACE to return to menu");
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(250, 450);
}

void Gameoverstate::onEnter() {}
void Gameoverstate::onExit() {}

void Gameoverstate::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        // Game over state decides to go back to menu
        auto menuState = std::make_shared<Menustate>(window);
        manager->changeState(menuState);
    }
}

void Gameoverstate::update(float dt) {}

void Gameoverstate::draw(sf::RenderWindow& window) {
    window.draw(gameOverText);
    window.draw(finalScoreText);
    window.draw(restartText);
}