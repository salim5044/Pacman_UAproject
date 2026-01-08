#pragma once
#include "Gamestate.h"
#include "Score.h"
#include <SFML/Graphics.hpp>

class Menustate : public GameState {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text playButtonText;
    sf::Text scoreText;
    sf::RectangleShape playButton;
    sf::RenderWindow& window;

public:
    Menustate(sf::RenderWindow& win);
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    bool isPlayButtonClicked(sf::Vector2f mousePos) const;
};