#include "Ghost.h"

Ghost::Ghost(float x, float y, GhostAI ai, int v, float delay, float chaseDelay)
    : Entity(x, y, EntityType::Ghost),
      aiType(ai), variant(v), spawnDelay(delay), chasingDelay(chaseDelay),
      startX(x), startY(y) {
    active = true;  // ALTIJD zichtbaar
    hasSpawned = (delay == 0.0f);
    currentState = GhostState::Exiting;

    isInChasingMode = (chaseDelay == 0.0f);


    normalSpeed = BASE_SPEED;
    speed = BASE_SPEED;
}

void Ghost::setState(GhostState newState) {
    currentState = newState;
}

void Ghost::setFeared(bool feared) {
    if (isFeared != feared && !isEaten) {
        isFeared = feared;
        if (feared) {
            currentState = GhostState::Fleeing;
            setDirection(-directionX, -directionY);

            if (speed > 0.0f) {
                normalSpeed = speed;
            }
            speed = normalSpeed * FEAR_SPEED_MULTIPLIER;
            // Fear time will be set by World when activating fear mode
        } else {
            fearTimeRemaining = 0.0f;
            if (isInChasingMode) {
                currentState = GhostState::Chasing;
            } else {
                currentState = GhostState::Roaming;
            }

            speed = normalSpeed;
        }
    }
}

void Ghost::setEaten(bool eaten) {
    if (isEaten != eaten) {
        isEaten = eaten;
        isFeared = false;
        fearTimeRemaining = 0.0f;

        if (eaten) {
            currentState = GhostState::Returning;
            speed = normalSpeed * 2.0f;

            lastIntersectionTileX = -999;
            lastIntersectionTileY = -999;
            atIntersection = false;
        } else {
            if (isInChasingMode) {
                currentState = GhostState::Chasing;
            } else {
                currentState = GhostState::Roaming;
            }

            speed = normalSpeed;
        }
    }
}

void Ghost::updateSpawnTimer(float dt) {
    if (!hasSpawned) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            hasSpawned = true;
            active = true;
            currentState = GhostState::Exiting;
        }
    }
}

void Ghost::updateChasingTimer(float dt) {
    if (hasSpawned && active && !isFeared && !isEaten && !isInChasingMode) {
        chasingTimer += dt;

        if (chasingTimer >= chasingDelay) {
            isInChasingMode = true;

            if (currentState == GhostState::Roaming) {
                currentState = GhostState::Chasing;
            }
        }
    }
}

bool Ghost::canMove() const {
    return hasSpawned && active;
}

void Ghost::respawn(int currentLevel, bool withDelays) {
    x = startX;
    y = startY;
    directionX = 0;
    directionY = 0;
    nextDirX = 0;
    nextDirY = 0;
    active = true;
    isFeared = false;
    isEaten = false;
    fearTimeRemaining = 0.0f;
    hasExitedHouse = false;
    currentState = GhostState::Exiting;

    lastIntersectionTileX = -999;
    lastIntersectionTileY = -999;
    atIntersection = false;

    hasLockedDirection = false;

    if (withDelays) {

        spawnTimer = 0.0f;
        hasSpawned = (spawnDelay == 0.0f);  // Alleen R+I (0s delay) spawnen meteen

        // Reset chasing delays
        chasingTimer = 0.0f;
        isInChasingMode = (chasingDelay == 0.0f);  // Alleen R+I starten in chase mode

        // Ghost R (variant 0) is altijd buiten het huis
        if (variant == 0) {
            fullyExited = true;
            if (hasSpawned) {
                currentState = GhostState::Roaming;
            }
        } else {
            fullyExited = false;
        }
    } else {

        hasSpawned = true;  // Spawnt meteen

        // Behoud chasing mode status (als hij al in chasing mode was, blijft dat zo)
        // chasingTimer en isInChasingMode NIET resetten

        // Ghost R (variant 0) is altijd buiten
        if (variant == 0) {
            fullyExited = true;
            currentState = GhostState::Roaming;
        } else {
            fullyExited = false;
        }
    }


    normalSpeed = BASE_SPEED + (currentLevel - 1) * GHOST_SPEED_INCREMENT;
    speed = normalSpeed;
}

void Ghost::resetForNewLevel() {
    x = startX;
    y = startY;
    directionX = 0;
    directionY = 0;
    nextDirX = 0;
    nextDirY = 0;

    active = true;
    isFeared = false;
    isEaten = false;
    fearTimeRemaining = 0.0f;
    hasExitedHouse = false;
    currentState = GhostState::Exiting;

    lastIntersectionTileX = -999;
    lastIntersectionTileY = -999;
    atIntersection = false;

    hasLockedDirection = false;

    spawnTimer = 0.0f;
    hasSpawned = (spawnDelay == 0.0f);  // Alleen ghosts met 0s delay spawnen meteen

    chasingTimer = 0.0f;
    isInChasingMode = (chasingDelay == 0.0f);  // Alleen ghosts met 0s chase delay starten in chase mode

    // Andere ghosts (variant 1, 2, 3) moeten eerst uit het huis
    if (variant == 0) {
        fullyExited = true;
        if (hasSpawned) {
            currentState = GhostState::Roaming;
        }
    } else {
        fullyExited = false;
    }
    normalSpeed = BASE_SPEED;
    speed = BASE_SPEED;
}