#include "Victorystate.h"
#include "StateManager.h"
#include "Menustate.h"
#include <stdexcept>

Victorystate::Victorystate(int score, sf::RenderWindow& win)
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

    victoryText.setFont(font);
    victoryText.setString("YOU WIN!");
    victoryText.setCharacterSize(50);
    victoryText.setFillColor(sf::Color::Green);
    victoryText.setPosition(280, 250);

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

void Victorystate::onEnter() {}
void Victorystate::onExit() {}

void Victorystate::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        // Victory state decides to go back to menu
        auto menuState = std::make_shared<Menustate>(window);
        manager->changeState(menuState);
    }
}

void Victorystate::update(float dt) {}

void Victorystate::draw(sf::RenderWindow& window) {
    window.draw(victoryText);
    window.draw(finalScoreText);
    window.draw(restartText);
}