#pragma once
#include "Entity.h"
#include "Random.h"
#include <cmath>
#include <algorithm>

enum class GhostAI {
    Chase,
    Ambush,
    Random,
    LockedDirection
};

enum class GhostState {
    Exiting,
    Roaming,
    Chasing,
    Fleeing,
    Returning
};

class Ghost : public Entity {
private:
    GhostAI aiType;
    int variant;
    GhostState currentState = GhostState::Exiting;

    bool isFeared = false;
    bool isEaten = false;
    float fearTimeRemaining = 0.0f;  // Track fear time for flashing

    //  Bewaar de normale (level-based) speed apart
    float normalSpeed = BASE_SPEED;

    float spawnDelay;
    float spawnTimer = 0.0f;
    bool hasSpawned = false;

    // Time-based chasing mode
    float chasingDelay;
    float chasingTimer = 0.0f;
    bool isInChasingMode = false;

    // For LockedDirection AI
    float lockedDirX = 0.0f;
    float lockedDirY = 0.0f;
    bool hasLockedDirection = false;

    bool hasExitedHouse = false;
    bool fullyExited = false;
    float exitTargetX = 0.0f;
    float exitTargetY = 0.0f;

    float ghostHouseMinX = 0.0f;
    float ghostHouseMaxX = 0.0f;
    float ghostHouseMinY = 0.0f;
    float ghostHouseMaxY = 0.0f;
    bool ghostHouseBoundsSet = false;

    float startX, startY;

    bool atIntersection = false;
    int lastIntersectionTileX = -999;
    int lastIntersectionTileY = -999;

public:
    Ghost(float x, float y, GhostAI ai, int v, float delay, float chaseDelay);

    GhostAI getAIType() const { return aiType; }
    int getVariant() const { return variant; }
    bool getFeared() const { return isFeared; }
    bool getEaten() const { return isEaten; }
    GhostState getState() const { return currentState; }
    float getFearTimeRemaining() const { return fearTimeRemaining; }

    void setState(GhostState newState);
    void setFeared(bool feared);
    void setEaten(bool eaten);
    void setFearTimeRemaining(float time) { fearTimeRemaining = time; }
    void updateFearTimer(float dt) { if (isFeared) fearTimeRemaining -= dt; }

    void updateSpawnTimer(float dt);
    void updateChasingTimer(float dt);
    bool canMove() const;
    bool shouldBeChasing() const { return isInChasingMode; }

    // respawn() heeft nu een parameter voor spawn delays
    // withDelays = false: Ghost komt meteen terug (na opgegeten te zijn)
    // withDelays = true: Ghost spawnt met delays (na Pacman death of nieuwe level)
    void respawn(int currentLevel, bool withDelays = false);
    void resetForNewLevel();

    // Override setSpeed om normalSpeed ook bij te werken
    void setSpeed(float s) {
        normalSpeed = s;
        if (!isFeared && !isEaten) {
            speed = s;
        }
        // Als in fear mode of eaten, pas alleen normalSpeed aan
        // speed blijft op de speciale waarde
    }

    float getStartX() const { return startX; }
    float getStartY() const { return startY; }

    // Locked direction methods
    void setLockedDirection(float dx, float dy) {
        lockedDirX = dx;
        lockedDirY = dy;
        hasLockedDirection = true;
    }

    float getLockedDirX() const { return lockedDirX; }
    float getLockedDirY() const { return lockedDirY; }
    bool hasLocked() const { return hasLockedDirection; }
    void clearLockedDirection() { hasLockedDirection = false; }

    bool needsToExitHouse() const {
        return currentState == GhostState::Exiting && hasSpawned && !fullyExited;
    }

    void setExitedHouse(bool exited) { hasExitedHouse = exited; }
    void setExitTarget(float targetX, float targetY) {
        exitTargetX = targetX;
        exitTargetY = targetY;
    }

    float getExitTargetX() const { return exitTargetX; }
    float getExitTargetY() const { return exitTargetY; }

    bool hasFullyExited() const { return fullyExited; }
    void setFullyExited(bool exited) {
        fullyExited = exited;
        if (exited && currentState == GhostState::Exiting) {
            currentState = GhostState::Roaming;
        }
    }

    void setGhostHouseBounds(float minX, float maxX, float minY, float maxY) {
        ghostHouseMinX = minX;
        ghostHouseMaxX = maxX;
        ghostHouseMinY = minY;
        ghostHouseMaxY = maxY;
        ghostHouseBoundsSet = true;
    }

    bool isPositionInGhostHouse(float posX, float posY) const {
        if (!ghostHouseBoundsSet) return false;
        return (posX >= ghostHouseMinX && posX <= ghostHouseMaxX &&
                posY >= ghostHouseMinY && posY <= ghostHouseMaxY);
    }

    bool canEnterPosition(float posX, float posY) const {
        if (currentState == GhostState::Returning) {
            return true;
        }
        if (fullyExited) {
            return !isPositionInGhostHouse(posX, posY);
        }
        return true;
    }

    bool isInsideGhostHouse() const {
        return isPositionInGhostHouse(x, y);
    }

    bool shouldRecalculateDirection(int tileX, int tileY) const {
        return (tileX != lastIntersectionTileX || tileY != lastIntersectionTileY);
    }

    void markIntersection(int tileX, int tileY) {
        lastIntersectionTileX = tileX;
        lastIntersectionTileY = tileY;
        atIntersection = true;
    }

    void clearIntersection() {
        atIntersection = false;
    }

    bool isAtIntersection() const {
        return atIntersection;
    }

    static float manhattanDistance(float x1, float y1, float x2, float y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }
};