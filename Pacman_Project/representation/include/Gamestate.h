#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class StateManager;

class GameState {
protected:
    StateManager* manager;

public:
    virtual ~GameState() = default;

    void setManager(StateManager* mgr) { manager = mgr; }

    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void onPause() {}
    virtual void onResume() {}
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};