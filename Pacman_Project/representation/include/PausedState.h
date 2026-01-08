#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>

class PausedState : public GameState {
private:
    sf::Font font;
    sf::Text pausedText;
    sf::Text resumeText;
    sf::Text menuText;
    sf::RectangleShape overlay;
    sf::RenderWindow& window;

public:
    PausedState(sf::RenderWindow& win);
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};