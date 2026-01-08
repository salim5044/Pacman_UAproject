#pragma once
#include "Observer.h"
#include "Config.h"

enum class EntityType { Pacman, Ghost, Wall, Coin, PowerPellet, Fruit };

class Entity : public Subject {
protected:
    float x, y;
    float width = 0.1f;
    float height = 0.1f;
    float speed = BASE_SPEED;

    float directionX = 0.0f;
    float directionY = 0.0f;

    float nextDirX = 0.0f;
    float nextDirY = 0.0f;

    bool active = true;
    bool dying = false;
    float deathTimer = 0.0f;

    EntityType type;

public:
    Entity(float startX, float startY, EntityType t);
    virtual ~Entity() = default;

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getSpeed() const { return speed; }
    EntityType getType() const { return type; }

    float getDirX() const { return directionX; }
    float getDirY() const { return directionY; }

    bool isActive() const { return active; }
    bool isDying() const { return dying; }
    float getDeathTimer() const { return deathTimer; }

    float getNextDirX() const { return nextDirX; }
    float getNextDirY() const { return nextDirY; }

    void setPosition(float newX, float newY);
    void setSize(float w, float h);
    void setSpeed(float s);
    void setActive(bool a);
    void setDying(bool d);
    void setDirection(float dx, float dy);
    void queueDirection(float dx, float dy);

    virtual void update(float deltaTime);
    void updateDeathAnimation(float deltaTime);
    bool collidesWith(const Entity& other) const;
};