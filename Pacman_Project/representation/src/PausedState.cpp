#include "PausedState.h"
#include "StateManager.h"
#include "Menustate.h"
#include "Config.h"
#include <stdexcept>

PausedState::PausedState(sf::RenderWindow& win) : window(win) {
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

    // Semi-transparent overlay
    overlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // "PAUSED" tekst
    pausedText.setFont(font);
    pausedText.setString("PAUSED");
    pausedText.setCharacterSize(60);
    pausedText.setFillColor(sf::Color::Yellow);
    pausedText.setStyle(sf::Text::Bold);

    // Center de PAUSED tekst
    sf::FloatRect bounds = pausedText.getLocalBounds();
    pausedText.setOrigin(bounds.width / 2, bounds.height / 2);
    pausedText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 80);

    // "Press ESC to resume" tekst
    resumeText.setFont(font);
    resumeText.setString("Press ESC to resume");
    resumeText.setCharacterSize(24);
    resumeText.setFillColor(sf::Color::White);

    bounds = resumeText.getLocalBounds();
    resumeText.setOrigin(bounds.width / 2, bounds.height / 2);
    resumeText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20);

    // "Press M for menu" tekst
    menuText.setFont(font);
    menuText.setString("Press M to return to menu");
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::White);

    bounds = menuText.getLocalBounds();
    menuText.setOrigin(bounds.width / 2, bounds.height / 2);
    menuText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 60);
}

void PausedState::onEnter() {}

void PausedState::onExit() {}

void PausedState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // ESC: Resume game (pop pause state)
        if (event.key.code == sf::Keyboard::Escape) {
            manager->popState();
        }
        // M: Return to menu
        else if (event.key.code == sf::Keyboard::M) {
            auto menuState = std::make_shared<Menustate>(window);
            manager->replaceAllStates(menuState);
        }
    }
}

void PausedState::update(float dt) {
    // Nothing to update - game is paused
}

void PausedState::draw(sf::RenderWindow& window) {
    // Draw overlay and pause text on top of game
    window.draw(overlay);
    window.draw(pausedText);
    window.draw(resumeText);
    window.draw(menuText);
}