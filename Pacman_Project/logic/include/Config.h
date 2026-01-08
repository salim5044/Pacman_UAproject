#pragma once

extern int MAP_WIDTH;
extern int MAP_HEIGHT;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SPRITE_SIZE = 16;
const int SPRITE_SHEET_WIDTH = 936;
const int SPRITE_SHEET_HEIGHT = 939;

const float WORLD_MIN = -1.0f;
const float WORLD_MAX = 1.0f;
const float WORLD_SIZE = WORLD_MAX - WORLD_MIN;

const float BASE_SPEED = 0.35f;
const float GHOST_SPEED_INCREMENT = 0.06f;
const float FEAR_DURATION_BASE = 8.0f;
const float FEAR_DURATION_DECREMENT = 1.0f;
const float FEAR_SPEED_MULTIPLIER = 0.5f;

const float PACMAN_ANIMATION_SPEED = 0.25f;
const float GHOST_ANIMATION_SPEED = 0.35f;

const int COIN_SCORE = 10;
const int POWER_PELLET_SCORE = 50;
const int FRUIT_SCORE = 100;
const int GHOST_SCORE = 200;
const int LEVEL_CLEAR_BONUS = 500;

// Ghost spawn delays (wanneer ze uit het huis komen)
const float GHOST_SPAWN_DELAYS[4] = {0.0f, 0.0f, 5.0f, 10.0f};

// Ghost chasing mode delays (wanneer ze in chasing mode gaan)
// Eerste twee: meteen (0.0f), laatste twee: na 5 en 10 seconden
const float GHOST_CHASING_DELAYS[4] = {0.0f, 0.0f, 5.0f, 10.0f};

const float GRID_TOLERANCE = 0.15f;