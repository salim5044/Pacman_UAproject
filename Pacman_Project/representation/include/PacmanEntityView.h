#pragma once
#include "EntityView.h"
#include "Spritemanager.h"
#include "Config.h"
#include <cmath>

class PacmanEntityView : public EntityView {
private:
    SpriteManager spriteManager;

public:
    PacmanEntityView(std::shared_ptr<Entity> m, sf::RenderWindow& w, sf::Texture& tex, const Camera& cam)
        : EntityView(m, w, tex, cam) {
        updateTextureRect();
        update();
    }

protected:
    void updateAnimation(float dt) override {
        if (!model->isActive() && !model->isDying()) return;

        animationTimer += dt;
        if (animationTimer >= PACMAN_ANIMATION_SPEED) {
            animationTimer = 0.0f;
            animationFrame = (animationFrame + 1) % 2;
        }
    }

    void updateTextureRect() override {
        sprite.setColor(sf::Color::White);

        // Death animation
        if (model->isDying()) {
            int frame = static_cast<int>(model->getDeathTimer() / 0.125f);
            frame = std::min(frame, 11);

            Sprite_ID deathSprites[] = {
                Sprite_ID::PACMAN_DYING_1, Sprite_ID::PACMAN_DYING_2,
                Sprite_ID::PACMAN_DYING_3, Sprite_ID::PACMAN_DYING_4,
                Sprite_ID::PACMAN_DYING_5, Sprite_ID::PACMAN_DYING_6,
                Sprite_ID::PACMAN_DYING_7, Sprite_ID::PACMAN_DYING_8,
                Sprite_ID::PACMAN_DYING_9, Sprite_ID::PACMAN_DYING_10,
                Sprite_ID::PACMAN_DYING_11, Sprite_ID::PACMAN_DYING_12
            };

            sprite.setTextureRect(spriteManager.getRect(deathSprites[frame]));
            return;
        }

        // Idle state
        if (model->getDirX() == 0 && model->getDirY() == 0) {
            sprite.setTextureRect(spriteManager.getRect(Sprite_ID::PACMAN_IDLE));
            return;
        }

        // Movement sprites based on direction
        Sprite_ID sprite1, sprite2;

        if (model->getDirX() > 0) {
            sprite1 = Sprite_ID::PACMAN_RIGHT_1;
            sprite2 = Sprite_ID::PACMAN_RIGHT_2;
        }
        else if (model->getDirX() < 0) {
            sprite1 = Sprite_ID::PACMAN_LEFT_1;
            sprite2 = Sprite_ID::PACMAN_LEFT_2;
        }
        else if (model->getDirY() > 0) {
            sprite1 = Sprite_ID::PACMAN_DOWN_1;
            sprite2 = Sprite_ID::PACMAN_DOWN_2;
        }
        else if (model->getDirY() < 0) {
            sprite1 = Sprite_ID::PACMAN_UP_1;
            sprite2 = Sprite_ID::PACMAN_UP_2;
        }
        else {
            sprite1 = Sprite_ID::PACMAN_IDLE;
            sprite2 = Sprite_ID::PACMAN_IDLE;
        }

        sprite.setTextureRect(spriteManager.getRect(animationFrame == 0 ? sprite1 : sprite2));
    }

    void updateScale() override {
        sf::Vector2f tileSize = camera.getTileSize();
        sf::IntRect rect = sprite.getTextureRect();

        float scaleX = tileSize.x / rect.width;
        float scaleY = tileSize.y / rect.height;

        sprite.setScale(scaleX, scaleY);
        sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    }
};