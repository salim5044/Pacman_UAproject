#pragma once
#include <SFML/Graphics.hpp>
#include "Observer.h"
#include "Entity.h"
#include "Camera.h"
#include <memory>


// Each entity type gets its own view subclass
class EntityView : public Observer {
protected:
    std::shared_ptr<Entity> model;
    sf::Sprite sprite;
    sf::RenderWindow& window;
    const Camera& camera;

    float animationTimer = 0.0f;
    int animationFrame = 0;

public:
    EntityView(std::shared_ptr<Entity> m, sf::RenderWindow& w, sf::Texture& tex, const Camera& cam)
        : model(m), window(w), camera(cam) {
        sprite.setTexture(tex);
        updatePosition();
    }

    virtual ~EntityView() = default;

    // Observer interface
    void onNotify(const GameEvent& event) override {
        if (event.type == EventType::EntityMoved ||
            event.type == EventType::DirectionChanged ||
            event.type == EventType::GameTick) {

            if (event.type == EventType::GameTick) {
                updateAnimation(event.value);
            }
            update();
            }
    }

    // Template method pattern - final update sequence
    void update() {
        if (!model->isActive() && !model->isDying()) return;

        updatePosition();
        updateTextureRect();  // Virtual - subclass implements
        updateScale();        // Virtual - subclass implements
    }

    void draw() {
        if (model->isActive() || model->isDying()) {
            window.draw(sprite);
        }
    }

protected:
    virtual void updateAnimation(float dt) = 0;
    virtual void updateTextureRect() = 0;
    virtual void updateScale() = 0;

    // Common functionality
    void updatePosition() {
        sf::Vector2f screenPos = camera.worldToScreen(model->getX(), model->getY());
        sprite.setPosition(screenPos);
    }
};