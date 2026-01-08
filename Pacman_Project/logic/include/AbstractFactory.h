#pragma once
#include <memory>
#include "Entity.h"
#include "Ghost.h"

class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;
    virtual std::shared_ptr<Entity> createPacman(float x, float y) = 0;
    virtual std::shared_ptr<Ghost> createGhost(float x, float y, GhostAI ai, int variant, float delay, float chaseDelay) = 0;
    virtual std::shared_ptr<Entity> createWall(float x, float y) = 0;
    virtual std::shared_ptr<Entity> createCoin(float x, float y) = 0;
    virtual std::shared_ptr<Entity> createPowerPellet(float x, float y) = 0;
    virtual std::shared_ptr<Entity> createFruit(float x, float y) = 0;
};