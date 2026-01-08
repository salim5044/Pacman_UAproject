#include "Entity.h"

Entity::Entity(float startX, float startY, EntityType t)
    : x(startX), y(startY), type(t) {
    nextDirX = 0.0f;
    nextDirY = 0.0f;
}

void Entity::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
    notifyObservers(GameEvent(EventType::EntityMoved));
}

void Entity::setSize(float w, float h) {
    width = w;
    height = h;
}

void Entity::setSpeed(float s) {
    speed = s;
}

void Entity::setActive(bool a) {
    active = a;
    notifyObservers(GameEvent(EventType::EntityMoved));
}

void Entity::setDying(bool d) {
    dying = d;
    if (d) {
        deathTimer = 0.0f;
        directionX = 0.0f;
        directionY = 0.0f;
    }
    notifyObservers(GameEvent(EventType::EntityMoved));
}

void Entity::setDirection(float dx, float dy) {
    if (!dying) {
        directionX = dx;
        directionY = dy;
        nextDirX = dx;
        nextDirY = dy;
        notifyObservers(GameEvent(EventType::DirectionChanged));
    }
}

void Entity::queueDirection(float dx, float dy) {
    if (!dying) {
        nextDirX = dx;
        nextDirY = dy;
    }
}

void Entity::update(float deltaTime) {
    if (dying) {
        updateDeathAnimation(deltaTime);
        return;
    }

    if (directionX != 0 || directionY != 0) {
        x += directionX * speed * deltaTime;
        y += directionY * speed * deltaTime;
        notifyObservers(GameEvent(EventType::EntityMoved));
    }
}

void Entity::updateDeathAnimation(float deltaTime) {
    deathTimer += deltaTime;
    notifyObservers(GameEvent(EventType::EntityMoved));

    if (deathTimer >= 1.5f) {
        active = false;
        dying = false;
    }
}

bool Entity::collidesWith(const Entity& other) const {
    if (!active || !other.isActive()) return false;
    if (dying || other.isDying()) return false;

    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float otherHalfW = other.getWidth() / 2.0f;
    float otherHalfH = other.getHeight() / 2.0f;

    return (x - halfW < other.getX() + otherHalfW &&
            x + halfW > other.getX() - otherHalfW &&
            y - halfH < other.getY() + otherHalfH &&
            y + halfH > other.getY() - otherHalfH);
}