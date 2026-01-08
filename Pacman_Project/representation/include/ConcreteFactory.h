#pragma once
#include "AbstractFactory.h"
#include "EntityView.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>


class ConcreteFactory : public AbstractFactory {
private:
    sf::RenderWindow& window;
    std::vector<std::shared_ptr<EntityView>>& views;
    const Camera& camera;

    sf::Texture spriteSheet;
    sf::Texture wallTexture;

    int ghostCount;

public:
    ConcreteFactory(sf::RenderWindow& w, std::vector<std::shared_ptr<EntityView>>& v, const Camera& cam);

    std::shared_ptr<Entity> createPacman(float x, float y) override;
    std::shared_ptr<Ghost> createGhost(float x, float y, GhostAI ai, int variant, float delay, float chaseDelay) override;
    std::shared_ptr<Entity> createWall(float x, float y) override;
    std::shared_ptr<Entity> createCoin(float x, float y) override;
    std::shared_ptr<Entity> createPowerPellet(float x, float y) override;
    std::shared_ptr<Entity> createFruit(float x, float y) override;

    void resetGhostCount();
};