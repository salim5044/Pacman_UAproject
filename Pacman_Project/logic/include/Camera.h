#pragma once
#include "Config.h"
#include <SFML/Graphics.hpp>

class Camera {
private:
    int screenWidth;
    int screenHeight;
    int mapWidth;
    int mapHeight;

public:
    Camera(int sw = SCREEN_WIDTH, int sh = SCREEN_HEIGHT,
           int mw = MAP_WIDTH, int mh = MAP_HEIGHT)
        : screenWidth(sw), screenHeight(sh), mapWidth(mw), mapHeight(mh) {}

    sf::Vector2f worldToScreen(float worldX, float worldY) const {
        float normalizedX = (worldX - WORLD_MIN) / WORLD_SIZE;
        float normalizedY = (worldY - WORLD_MIN) / WORLD_SIZE;

        return sf::Vector2f(
            normalizedX * screenWidth,
            normalizedY * screenHeight
        );
    }

    sf::Vector2f getTileSize() const {
        float tileWidth = static_cast<float>(screenWidth) / mapWidth;
        float tileHeight = static_cast<float>(screenHeight) / mapHeight;
        return sf::Vector2f(tileWidth, tileHeight);
    }

    sf::Vector2f getSpriteScale(int spriteWidth, int spriteHeight, float sizeFactor = 1.0f) const {
        sf::Vector2f tileSize = getTileSize();
        return sf::Vector2f(
            (tileSize.x / spriteWidth) * sizeFactor,
            (tileSize.y / spriteHeight) * sizeFactor
        );
    }

    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
};