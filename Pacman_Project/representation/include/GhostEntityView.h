#pragma once
#include "EntityView.h"
#include "Ghost.h"
#include "Spritemanager.h"
#include "Config.h"
#include <cmath>

class GhostEntityView : public EntityView {
private:
    std::shared_ptr<Ghost> ghostModel;
    SpriteManager spriteManager;
    int variant;

public:
    GhostEntityView(std::shared_ptr<Ghost> ghost, sf::RenderWindow& w, sf::Texture& tex, const Camera& cam, int v)
        : EntityView(ghost, w, tex, cam), ghostModel(ghost), variant(v) {
        updateTextureRect();
        update();
    }

protected:
    void updateAnimation(float dt) override {
        if (!model->isActive() && !model->isDying()) return;

        float animSpeed = GHOST_ANIMATION_SPEED;

        // Faster flashing in fear mode near end
        if (ghostModel->getFeared() && ghostModel->getFearTimeRemaining() < 2.0f) {
            animSpeed = 0.15f;  // Flash faster
        }

        animationTimer += dt;
        if (animationTimer >= animSpeed) {
            animationTimer = 0.0f;
            animationFrame = (animationFrame + 1) % 2;
        }
    }

    void updateTextureRect() override {
        sprite.setColor(sf::Color::White);


        // EATEN: Show only eyes in movement direction
        if (ghostModel->getEaten()) {
            Sprite_ID eyeSprite;
            if (model->getDirX() > 0) eyeSprite = Sprite_ID::GHOST_EYES_RIGHT;
            else if (model->getDirX() < 0) eyeSprite = Sprite_ID::GHOST_EYES_LEFT;
            else if (model->getDirY() > 0) eyeSprite = Sprite_ID::GHOST_EYES_DOWN;
            else if (model->getDirY() < 0) eyeSprite = Sprite_ID::GHOST_EYES_UP;
            else eyeSprite = Sprite_ID::GHOST_EYES_RIGHT;

            sprite.setTextureRect(spriteManager.getRect(eyeSprite));
            return;
        }

        // FEAR MODE with blue-white flashing
        if (ghostModel->getFeared()) {
            bool shouldFlash = ghostModel->getFearTimeRemaining() < 2.0f;

            if (shouldFlash) {
                // Alternate between BLUE and WHITE
                sprite.setTextureRect(spriteManager.getRect(
                    animationFrame == 0 ? Sprite_ID::GHOST_FEAR_BLUE_1 : Sprite_ID::GHOST_FEAR_FLASH_1
                ));
            } else {
                // Normal blue fear sprites
                sprite.setTextureRect(spriteManager.getRect(
                    animationFrame == 0 ? Sprite_ID::GHOST_FEAR_BLUE_1 : Sprite_ID::GHOST_FEAR_BLUE_2
                ));
            }
            return;
        }

        // LOCKED DIRECTION AI: Show normal ghost with eyes in locked direction
        if (ghostModel->getAIType() == GhostAI::LockedDirection && ghostModel->hasLocked()) {
            updateNormalGhostSprite();
            return;
        }

        // NORMAL: Regular ghost sprites
        updateNormalGhostSprite();
    }

    void updateScale() override {
        sf::Vector2f tileSize = camera.getTileSize();
        sf::IntRect rect = sprite.getTextureRect();

        float scaleX = tileSize.x / rect.width;
        float scaleY = tileSize.y / rect.height;

        sprite.setScale(scaleX, scaleY);
        sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    }

private:
    void updateNormalGhostSprite() {
        int dirOffset = 0;
        if (model->getDirX() > 0) dirOffset = 0;
        else if (model->getDirX() < 0) dirOffset = 2;
        else if (model->getDirY() > 0) dirOffset = 6;
        else if (model->getDirY() < 0) dirOffset = 4;

        dirOffset += animationFrame;

        Sprite_ID ghostSprites[][8] = {
            {
                Sprite_ID::GHOST_RED_RIGHT_1, Sprite_ID::GHOST_RED_RIGHT_2,
                Sprite_ID::GHOST_RED_LEFT_1, Sprite_ID::GHOST_RED_LEFT_2,
                Sprite_ID::GHOST_RED_UP_1, Sprite_ID::GHOST_RED_UP_2,
                Sprite_ID::GHOST_RED_DOWN_1, Sprite_ID::GHOST_RED_DOWN_2
            },
            {
                Sprite_ID::GHOST_BLUE_RIGHT_1, Sprite_ID::GHOST_BLUE_RIGHT_2,
                Sprite_ID::GHOST_BLUE_LEFT_1, Sprite_ID::GHOST_BLUE_LEFT_2,
                Sprite_ID::GHOST_BLUE_UP_1, Sprite_ID::GHOST_BLUE_UP_2,
                Sprite_ID::GHOST_BLUE_DOWN_1, Sprite_ID::GHOST_BLUE_DOWN_2
            },
            {
                Sprite_ID::GHOST_WHITE_RIGHT_1, Sprite_ID::GHOST_WHITE_RIGHT_2,
                Sprite_ID::GHOST_WHITE_LEFT_1, Sprite_ID::GHOST_WHITE_LEFT_2,
                Sprite_ID::GHOST_WHITE_UP_1, Sprite_ID::GHOST_WHITE_UP_2,
                Sprite_ID::GHOST_WHITE_DOWN_1, Sprite_ID::GHOST_WHITE_DOWN_2
            },
            {
                Sprite_ID::GHOST_YELLOW_RIGHT_1, Sprite_ID::GHOST_YELLOW_RIGHT_2,
                Sprite_ID::GHOST_YELLOW_LEFT_1, Sprite_ID::GHOST_YELLOW_LEFT_2,
                Sprite_ID::GHOST_YELLOW_UP_1, Sprite_ID::GHOST_YELLOW_UP_2,
                Sprite_ID::GHOST_YELLOW_DOWN_1, Sprite_ID::GHOST_YELLOW_DOWN_2
            }
        };

        int colorIndex = variant % 4;
        sprite.setTextureRect(spriteManager.getRect(ghostSprites[colorIndex][dirOffset]));
    }
};