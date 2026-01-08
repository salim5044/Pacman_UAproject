#include "Menustate.h"
#include "StateManager.h"
#include "Playstate.h"
#include <stdexcept>

Menustate::Menustate(sf::RenderWindow& win) : window(win) {
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

    titleText.setFont(font);
    titleText.setString("PAC-MAN");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(250, 50);

    playButton.setSize(sf::Vector2f(200, 60));
    playButton.setFillColor(sf::Color::Green);
    playButton.setPosition(300, 450);

    playButtonText.setFont(font);
    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(30);
    playButtonText.setFillColor(sf::Color::Black);
    playButtonText.setPosition(360, 460);

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(350, 200);
}

void Menustate::onEnter() {
    scoreText.setString(Score::getHighscoreString());
}

void Menustate::onExit() {}

void Menustate::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (isPlayButtonClicked(mousePos)) {
            // Menu state decides to transition to play state
            auto playState = std::make_shared<Playstate>(window);
            manager->changeState(playState);
        }
        }
}

void Menustate::update(float dt) {}

void Menustate::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(scoreText);
    window.draw(playButton);
    window.draw(playButtonText);
}

bool Menustate::isPlayButtonClicked(sf::Vector2f mousePos) const {
    return playButton.getGlobalBounds().contains(mousePos);
}