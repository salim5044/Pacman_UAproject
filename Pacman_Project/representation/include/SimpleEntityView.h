#pragma once
#include "EntityView.h"
#include "Spritemanager.h"
#include "Config.h"

// These don't need complex animation or casting
class SimpleEntityView : public EntityView {
private:
    SpriteManager spriteManager;

public:
    SimpleEntityView(std::shared_ptr<Entity> m, sf::RenderWindow& w, sf::Texture& tex, const Camera& cam)
        : EntityView(m, w, tex, cam) {
        updateTextureRect();
        update();
    }

protected:
    void updateAnimation(float dt) override {
        // Simple entities don't animate
        // (Could add pulsing for power pellets here if desired)
    }

    void updateTextureRect() override {
        EntityType type = model->getType();
        sprite.setColor(sf::Color::White);

        switch (type) {
            case EntityType::Wall:
                sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
                break;

            case EntityType::Coin:
                sprite.setTextureRect(spriteManager.getRect(Sprite_ID::ORB_SMALL));
                break;

            case EntityType::PowerPellet:
                sprite.setTextureRect(spriteManager.getRect(Sprite_ID::ORB_BIG));
                break;

            case EntityType::Fruit:
                sprite.setTextureRect(spriteManager.getRect(Sprite_ID::FRUIT_CHERRY));
                break;

            default:
                break;
        }
    }

    void updateScale() override {
        sf::Vector2f tileSize = camera.getTileSize();
        sf::IntRect rect = sprite.getTextureRect();

        float scaleX = tileSize.x / rect.width;
        float scaleY = tileSize.y / rect.height;

        EntityType type = model->getType();

        // Scale down coins and pellets
        if (type == EntityType::Coin) {
            scaleX *= 0.25f;
            scaleY *= 0.25f;
        }
        else if (type == EntityType::PowerPellet) {
            scaleX *= 0.45f;
            scaleY *= 0.45f;
        }

        sprite.setScale(scaleX, scaleY);
        sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    }
};