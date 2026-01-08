#pragma once
#include "Gamestate.h"
#include "World.h"
#include "ConcreteFactory.h"
#include "EntityView.h"
#include "Camera.h"
#include "Score.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Playstate : public GameState {
private:
    std::vector<std::shared_ptr<EntityView>> views;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<ConcreteFactory> factory;
    std::shared_ptr<World> world;
    sf::Font font;
    sf::RenderWindow& window;
    bool gameOverHandled = false;

public:
    Playstate(sf::RenderWindow& win);
    void onEnter() override;
    void onExit() override;
    void onPause() override;
    void onResume() override;
    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    std::shared_ptr<World> getWorld() { return world; }
};