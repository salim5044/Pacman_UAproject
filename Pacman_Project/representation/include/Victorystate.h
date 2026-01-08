#pragma once
#include "Gamestate.h"
#include <SFML/Graphics.hpp>

class Victorystate : public GameState {
private:
    sf::Font font;
    sf::Text victoryText;
    sf::Text finalScoreText;
    sf::Text restartText;
    int finalScore;
    sf::RenderWindow& window;

public:
    Victorystate(int score, sf::RenderWindow& win);
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};