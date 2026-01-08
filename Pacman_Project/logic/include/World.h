#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "AbstractFactory.h"
#include "Score.h"
#include "Entity.h"
#include "Ghost.h"
#include "Config.h"

class World {
private:
    std::shared_ptr<AbstractFactory> factory;

    std::shared_ptr<Entity> pacman;
    std::vector<std::shared_ptr<Ghost>> ghosts;
    std::vector<std::shared_ptr<Entity>> walls;
    std::vector<std::shared_ptr<Entity>> coins;
    std::vector<std::shared_ptr<Entity>> powerPellets;
    std::vector<std::shared_ptr<Entity>> fruits;

    std::shared_ptr<Score> score;

    // Game state
    int lives = 3;
    int currentLevel = 1;
    bool gameOver = false;
    bool gameWon = false;

    // Fear mode
    bool fearModeActive = false;
    float fearModeTimer = 0.0f;
    float fearModeDuration = FEAR_DURATION_BASE;

    // Map template (constant)
    std::vector<std::string> baseMap;

    // NEW: Remember Pacman's original spawn position
    float pacmanStartX = 0.0f;
    float pacmanStartY = 0.0f;

    void loadMapFromFile(const std::string& filename);
    void initMap();
    void spawnCollectibles();
    void updatePacman(float dt);
    void updateGhosts(float dt);
    void updateGhostAI(std::shared_ptr<Ghost> ghost, float dt);
    void updateGhostState(std::shared_ptr<Ghost> ghost);
    void checkCollisions();
    void handlePacmanDeath();

    // AI helpers
    std::pair<float, float> getChaseDirectionByAI(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getRoamingDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getChaseDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getAmbushDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getRandomDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getLockedDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getFearDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getReturnHomeDirection(std::shared_ptr<Ghost> ghost);
    std::pair<float, float> getExitHouseDirection(std::shared_ptr<Ghost> ghost);

    std::vector<std::pair<float, float>> getValidDirections(std::shared_ptr<Ghost> ghost);
    bool isWallInDirection(float x, float y, float dx, float dy);
    bool isIntersection(float x, float y) const;

public:
    World(std::shared_ptr<AbstractFactory> fact);

    void initLevel();
    void update();
    void resetGame();
    void nextLevel();

    // Getters
    std::shared_ptr<Entity> getPacman() { return pacman; }
    const std::vector<std::shared_ptr<Ghost>>& getGhosts() const { return ghosts; }
    int getScore() const { return score ? score->getScore() : 0; }
    int getLives() const { return lives; }
    int getCurrentLevel() const { return currentLevel; }
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }
    bool isFearMode() const { return fearModeActive; }
    float getFearModeTimeLeft() const { return fearModeTimer; }
};