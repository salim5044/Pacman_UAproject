#pragma once
#include "Gamestate.h"
#include <SFML/Graphics.hpp>

class Gameoverstate : public GameState {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text finalScoreText;
    sf::Text restartText;
    int finalScore;
    sf::RenderWindow& window;

public:
    Gameoverstate(int score, sf::RenderWindow& win);
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};