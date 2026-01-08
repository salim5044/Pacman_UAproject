#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class Sprite_ID {
    // PACMAN
    PACMAN_IDLE,
    PACMAN_RIGHT_1,
    PACMAN_RIGHT_2,
    PACMAN_LEFT_1,
    PACMAN_LEFT_2,
    PACMAN_UP_1,
    PACMAN_UP_2,
    PACMAN_DOWN_1,
    PACMAN_DOWN_2,
    // -----------
    PACMAN_DYING_1,
    PACMAN_DYING_2,
    PACMAN_DYING_3,
    PACMAN_DYING_4,
    PACMAN_DYING_5,
    PACMAN_DYING_6,
    PACMAN_DYING_7,
    PACMAN_DYING_8,
    PACMAN_DYING_9,
    PACMAN_DYING_10,
    PACMAN_DYING_11,
    PACMAN_DYING_12,

    // RED GHOST
    GHOST_RED_RIGHT_1,
    GHOST_RED_RIGHT_2,
    GHOST_RED_LEFT_1,
    GHOST_RED_LEFT_2,
    GHOST_RED_UP_1,
    GHOST_RED_UP_2,
    GHOST_RED_DOWN_1,
    GHOST_RED_DOWN_2,

    // BLUE GHOST
    GHOST_BLUE_RIGHT_1,
    GHOST_BLUE_RIGHT_2,
    GHOST_BLUE_LEFT_1,
    GHOST_BLUE_LEFT_2,
    GHOST_BLUE_UP_1,
    GHOST_BLUE_UP_2,
    GHOST_BLUE_DOWN_1,
    GHOST_BLUE_DOWN_2,

    // WHITE GHOST
    GHOST_WHITE_RIGHT_1,
    GHOST_WHITE_RIGHT_2,
    GHOST_WHITE_LEFT_1,
    GHOST_WHITE_LEFT_2,
    GHOST_WHITE_UP_1,
    GHOST_WHITE_UP_2,
    GHOST_WHITE_DOWN_1,
    GHOST_WHITE_DOWN_2,

    // YELLOW GHOST
    GHOST_YELLOW_RIGHT_1,
    GHOST_YELLOW_RIGHT_2,
    GHOST_YELLOW_LEFT_1,
    GHOST_YELLOW_LEFT_2,
    GHOST_YELLOW_UP_1,
    GHOST_YELLOW_UP_2,
    GHOST_YELLOW_DOWN_1,
    GHOST_YELLOW_DOWN_2,

    // FEAR MODE
    GHOST_FEAR_BLUE_1,
    GHOST_FEAR_BLUE_2,
    GHOST_FEAR_FLASH_1,
    GHOST_FEAR_FLASH_2,

    // GHOST EYES
    GHOST_EYES_RIGHT,
    GHOST_EYES_LEFT,
    GHOST_EYES_UP,
    GHOST_EYES_DOWN,

    // ORBS
    ORB_SMALL,
    ORB_BIG,

    // FRUITS
    FRUIT_CHERRY,
    FRUIT_STRAWBERRY,
    FRUIT_ORANGE,
    FRUIT_APPLE,
    FRUIT_MELON,
    FRUIT_GALAXIAN,
    FRUIT_BELL,
    FRUIT_KEY,

    // WHITE SCORES
    WHITE_200,
    WHITE_400,
    WHITE_800,
    WHITE_1600,

    // BLUE SCORES
    BLUE_100,
    BLUE_300,
    BLUE_500,
    BLUE_700,
    BLUE_1000,
    BLUE_2000,
    BLUE_3000,
    BLUE_5000,

    // COUNT
    COUNT
};

class SpriteManager {
private:
    std::vector<sf::IntRect> spriteRects;

public:
    SpriteManager() {
        spriteRects.resize(static_cast<int>(Sprite_ID::COUNT));

        // PACMAN
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_IDLE)] = sf::IntRect(488, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_RIGHT_1)] = sf::IntRect(456, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_RIGHT_2)] = sf::IntRect(472, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_LEFT_1)] = sf::IntRect(456, 16, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_LEFT_2)] = sf::IntRect(472, 16, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_UP_1)] = sf::IntRect(456, 32, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_UP_2)] = sf::IntRect(472, 32, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DOWN_1)] = sf::IntRect(456, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DOWN_2)] = sf::IntRect(472, 48, 16, 16);

        // Death animation
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_1)] = sf::IntRect(488, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_2)] = sf::IntRect(504, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_3)] = sf::IntRect(520, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_4)] = sf::IntRect(536, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_5)] = sf::IntRect(552, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_6)] = sf::IntRect(568, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_7)] = sf::IntRect(584, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_8)] = sf::IntRect(600, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_9)] = sf::IntRect(616, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_10)] = sf::IntRect(632, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_11)] = sf::IntRect(648, 0, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::PACMAN_DYING_12)] = sf::IntRect(664, 0, 16, 16);

        // RED GHOST
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_RIGHT_1)] = sf::IntRect(456, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_RIGHT_2)] = sf::IntRect(472, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_LEFT_1)] = sf::IntRect(488, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_LEFT_2)] = sf::IntRect(504, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_UP_1)] = sf::IntRect(520, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_UP_2)] = sf::IntRect(536, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_DOWN_1)] = sf::IntRect(552, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_RED_DOWN_2)] = sf::IntRect(568, 64, 16, 16);

        // BLUE GHOST
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_RIGHT_1)] = sf::IntRect(456, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_RIGHT_2)] = sf::IntRect(472, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_LEFT_1)] = sf::IntRect(488, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_LEFT_2)] = sf::IntRect(504, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_UP_1)] = sf::IntRect(520, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_UP_2)] = sf::IntRect(536, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_DOWN_1)] = sf::IntRect(552, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_BLUE_DOWN_2)] = sf::IntRect(568, 80, 16, 16);

        // WHITE GHOST
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_RIGHT_1)] = sf::IntRect(456, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_RIGHT_2)] = sf::IntRect(472, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_LEFT_1)] = sf::IntRect(488, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_LEFT_2)] = sf::IntRect(504, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_UP_1)] = sf::IntRect(520, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_UP_2)] = sf::IntRect(536, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_DOWN_1)] = sf::IntRect(552, 96, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_WHITE_DOWN_2)] = sf::IntRect(568, 96, 16, 16);

        // YELLOW GHOST
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_RIGHT_1)] = sf::IntRect(456, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_RIGHT_2)] = sf::IntRect(472, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_LEFT_1)] = sf::IntRect(488, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_LEFT_2)] = sf::IntRect(504, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_UP_1)] = sf::IntRect(520, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_UP_2)] = sf::IntRect(536, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_DOWN_1)] = sf::IntRect(552, 112, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_YELLOW_DOWN_2)] = sf::IntRect(568, 112, 16, 16);

        // FEAR MODE
        spriteRects[static_cast<int>(Sprite_ID::GHOST_FEAR_BLUE_1)] = sf::IntRect(584, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_FEAR_BLUE_2)] = sf::IntRect(600, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_FEAR_FLASH_1)] = sf::IntRect(616, 64, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_FEAR_FLASH_2)] = sf::IntRect(632, 64, 16, 16);

        // GHOST EYES
        spriteRects[static_cast<int>(Sprite_ID::GHOST_EYES_RIGHT)] = sf::IntRect(584, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_EYES_LEFT)] = sf::IntRect(600, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_EYES_UP)] = sf::IntRect(616, 80, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::GHOST_EYES_DOWN)] = sf::IntRect(632, 80, 16, 16);

        // ORBS
        spriteRects[static_cast<int>(Sprite_ID::ORB_SMALL)] = sf::IntRect(11, 11, 2, 2);
        spriteRects[static_cast<int>(Sprite_ID::ORB_BIG)] = sf::IntRect(8, 24, 8, 8);

        // FRUITS
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_CHERRY)] = sf::IntRect(488, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_STRAWBERRY)] = sf::IntRect(504, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_MELON)] = sf::IntRect(520, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_APPLE)] = sf::IntRect(536, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_ORANGE)] = sf::IntRect(552, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_GALAXIAN)] = sf::IntRect(568, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_BELL)] = sf::IntRect(584, 48, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::FRUIT_KEY)] = sf::IntRect(600, 48, 16, 16);

        // WHITE SCORES
        spriteRects[static_cast<int>(Sprite_ID::WHITE_200)] = sf::IntRect(456, 128, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::WHITE_400)] = sf::IntRect(472, 128, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::WHITE_800)] = sf::IntRect(488, 128, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::WHITE_1600)] = sf::IntRect(504, 128, 16, 16);

        // BLUE SCORES
        spriteRects[static_cast<int>(Sprite_ID::BLUE_100)] = sf::IntRect(456, 144, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_300)] = sf::IntRect(472, 144, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_500)] = sf::IntRect(488, 144, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_700)] = sf::IntRect(504, 144, 16, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_1000)] = sf::IntRect(520, 144, 18, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_2000)] = sf::IntRect(518, 160, 20, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_3000)] = sf::IntRect(518, 176, 20, 16);
        spriteRects[static_cast<int>(Sprite_ID::BLUE_5000)] = sf::IntRect(518, 192, 20, 16);
    }

    sf::IntRect getRect(Sprite_ID id) const {
        return spriteRects[static_cast<int>(id)];
    }
};