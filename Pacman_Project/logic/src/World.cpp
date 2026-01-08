#include "World.h"
#include "Stopwatch.h"
#include "Random.h"
#include <cmath>
#include <fstream>
#include <iostream>

World::World(std::shared_ptr<AbstractFactory> fact) : factory(fact) {
    score = std::make_shared<Score>();
    lives = 3;
    currentLevel = 1;
    gameOver = false;
    gameWon = false;
    loadMapFromFile("map.txt");
}

void World::loadMapFromFile(const std::string& filename) {
    baseMap.clear();
    std::ifstream file(filename);
    bool loaded = false;

    if (file.is_open()) {
        loaded = true;
    } else {
        file.open("../" + filename);
        if (file.is_open()) {
            loaded = true;
        } else {
            file.open("assets/" + filename);
            if (file.is_open()) {
                loaded = true;
            }
        }
    }

    if (!loaded) {
        std::cerr << "WARNING: Map file '" << filename << "' not found!\n";
        std::cerr << "Tried paths:\n";
        std::cerr << "  - " << filename << "\n";
        std::cerr << "  - ../" << filename << "\n";
        std::cerr << "  - assets/" << filename << "\n";
        std::cerr << "Using default hardcoded map instead.\n\n";
        initMap();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line.pop_back();
        }
        if (!line.empty()) {
            baseMap.push_back(line);
        }
    }
    file.close();
    if (!baseMap.empty()) {
        MAP_HEIGHT = static_cast<int>(baseMap.size());
        MAP_WIDTH = static_cast<int>(baseMap[0].length());
    }
}

void World::initMap() {
    baseMap = {
        "###################",
        "#P...............F#",
        "#.##.###.#.###.##.#",
        "#F................#",
        "#.##.#.#####.#.##.#",
        "#....#...#...#....#",
        "####.###.#.###.####",
        "####.#...R...#.####",
        "####.#.##-##.#.####",
        "####.#.#BIA#.#.####",
        "####.#.#####.#.####",
        "####.#.......#.####",
        "####.#.#####.#.####",
        "####.#.......#.####",
        "####.###.#.###.####",
        "####.###.#.###.####",
        "#....#...#...#....#",
        "#.##.#.#####.#.##.#",
        "#.................#",
        "#.##.###.#.###.##.#",
        "#F...............F#",
        "###################"
    };
    MAP_HEIGHT = static_cast<int>(baseMap.size());
    MAP_WIDTH = static_cast<int>(baseMap[0].length());
}

void World::resetGame() {
    lives = 3; currentLevel = 1; gameOver = false; gameWon = false;
    fearModeActive = false; fearModeTimer = 0.0f; fearModeDuration = FEAR_DURATION_BASE;
    score->reset();
    walls.clear(); coins.clear(); powerPellets.clear(); fruits.clear(); ghosts.clear(); pacman = nullptr;
    initLevel();
}

void World::initLevel() {
    coins.clear(); powerPellets.clear(); fruits.clear();
    fearModeActive = false; fearModeTimer = 0.0f;
    fearModeDuration = FEAR_DURATION_BASE - (currentLevel - 1) * FEAR_DURATION_DECREMENT;
    fearModeDuration = std::max(3.0f, fearModeDuration);
    float tileWidth = WORLD_SIZE / MAP_WIDTH;
    float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    if (currentLevel == 1) {
        walls.clear(); ghosts.clear(); pacman = nullptr;
    }

    // STAP 1: Zoek ghost spawn posities (R, I, B, A) en deur
    struct GhostSpawnData {
        float x = 0.0f;
        float y = 0.0f;
        bool found = false;
    };

    GhostSpawnData ghostSpawns[4]; // R=0, I=1, B=2, A=3
    float doorX = 0.0f, doorY = 0.0f;
    bool doorFound = false;

    // Voor ghost house bounds
    float ghostHouseMinX = 999.0f, ghostHouseMaxX = -999.0f;
    float ghostHouseMinY = 999.0f, ghostHouseMaxY = -999.0f;
    bool foundGhostArea = false;

    for (int row = 0; row < static_cast<int>(baseMap.size()); ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            char type = (col < static_cast<int>(baseMap[row].size())) ? baseMap[row][col] : ' ';
            float x = WORLD_MIN + (col * tileWidth) + (tileWidth / 2.0f);
            float y = WORLD_MIN + (row * tileHeight) + (tileHeight / 2.0f);

            // Vind ghost posities
            if (type == 'R') {
                ghostSpawns[0].x = x;
                ghostSpawns[0].y = y;
                ghostSpawns[0].found = true;
            }
            else if (type == 'I') {
                ghostSpawns[1].x = x;
                ghostSpawns[1].y = y;
                ghostSpawns[1].found = true;
            }
            else if (type == 'B') {
                ghostSpawns[2].x = x;
                ghostSpawns[2].y = y;
                ghostSpawns[2].found = true;
            }
            else if (type == 'A') {
                ghostSpawns[3].x = x;
                ghostSpawns[3].y = y;
                ghostSpawns[3].found = true;
            }

            // Vind deur positie
            if (type == '-') {
                doorX = x;
                doorY = y;
                doorFound = true;
            }

            // Bereken ghost house bounds (voor I, B, A posities - niet R want die staat buiten)
            if (type == 'I' || type == 'B' || type == 'A') {
                ghostHouseMinX = std::min(ghostHouseMinX, x - tileWidth);
                ghostHouseMaxX = std::max(ghostHouseMaxX, x + tileWidth);
                ghostHouseMinY = std::min(ghostHouseMinY, y - tileHeight);
                ghostHouseMaxY = std::max(ghostHouseMaxY, y + tileHeight * 2);
                foundGhostArea = true;
            }
        }
    }

    // Als geen deur gevonden, gebruik center als fallback
    if (!doorFound) {
        doorX = WORLD_MIN + ((MAP_WIDTH / 2.0f) * tileWidth);
        doorY = WORLD_MIN + ((MAP_HEIGHT / 2.0f) * tileHeight);
    }

    // STAP 2: Plaats alle entities BEHALVE ghosts
    for (int row = 0; row < static_cast<int>(baseMap.size()); ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            float x = WORLD_MIN + (col * tileWidth) + (tileWidth / 2.0f);
            float y = WORLD_MIN + (row * tileHeight) + (tileHeight / 2.0f);
            char type = (col < static_cast<int>(baseMap[row].size())) ? baseMap[row][col] : ' ';

            if (type == '#' && currentLevel == 1) {
                auto wall = factory->createWall(x, y);
                wall->setSize(tileWidth, tileHeight);
                walls.push_back(wall);
            }
            else if (type == 'P' && currentLevel == 1) {
                pacman = factory->createPacman(x, y);
                pacman->setSize(tileWidth * 0.8f, tileHeight * 0.8f);
                pacman->addObserver(score);
                pacmanStartX = x;
                pacmanStartY = y;
            }
            else if (type == '.') {
                auto coin = factory->createCoin(x, y);
                coin->setSize(tileWidth * 0.25f, tileHeight * 0.25f);
                coins.push_back(coin);
                coin->addObserver(score);
            }
            else if (type == 'O') {
                auto pellet = factory->createPowerPellet(x, y);
                pellet->setSize(tileWidth * 0.5f, tileHeight * 0.5f);
                powerPellets.push_back(pellet);
                pellet->addObserver(score);
            }
            else if (type == 'F') {
                auto fruit = factory->createFruit(x, y);
                fruit->setSize(tileWidth * 0.6f, tileHeight * 0.6f);
                fruits.push_back(fruit);
                fruit->addObserver(score);
            }
            // Skip R, I, B, A - die worden hieronder gedaan
        }
    }

    // STAP 3: Spawn ghosts op hun individuele posities (alleen bij level 1)
    if (currentLevel == 1) {
        // AI verdeling volgens opdracht:
        // R (Ghost 0): LockedDirection - spawn 0s, chase 0s - BUITEN ghost house
        // I (Ghost 1): Ambush - spawn 0s, chase 0s
        // B (Ghost 2): Ambush - spawn 5s, chase 5s
        // A (Ghost 3): Chase - spawn 10s, chase 10s

        struct GhostConfig {
            GhostAI ai;
            int variant;
            float spawnDelay;
            float chaseDelay;
        };

        GhostConfig configs[4] = {
            {GhostAI::LockedDirection, 0, 0.0f, 0.0f},  // R: LockedDirection (rood)
            {GhostAI::Ambush, 1, 0.0f, 0.0f},           // I: Ambush (pink)
            {GhostAI::Ambush, 2, 5.0f, 5.0f},           // B: Ambush (blauw)
            {GhostAI::Chase, 3, 10.0f, 10.0f}           // A: Chase (oranje)
        };

        for (int i = 0; i < 4; i++) {
            if (!ghostSpawns[i].found) continue; // Skip als deze ghost niet in map staat

            auto ghost = factory->createGhost(
                ghostSpawns[i].x,
                ghostSpawns[i].y,
                configs[i].ai,
                configs[i].variant,
                configs[i].spawnDelay,
                configs[i].chaseDelay
            );

            ghost->setSize(tileWidth * 0.8f, tileHeight * 0.8f);
            ghost->setSpeed(BASE_SPEED + (currentLevel - 1) * GHOST_SPEED_INCREMENT);
            ghost->setExitTarget(doorX, doorY);

            // Ghost house bounds ALLEEN voor ghosts die binnen starten (I, B, A)
            // Ghost house bounds voor ALLE ghosts, maar R kan er niet in tijdens normaal spel
            if (foundGhostArea) {
                ghost->setGhostHouseBounds(ghostHouseMinX, ghostHouseMaxX, ghostHouseMinY, ghostHouseMaxY);
                // R (index 0) is al fully exited, dus barrier geldt voor hem ook
            }

            // R (ghost 0) is al buiten, dus direct fully exited
            if (i == 0) {
                ghost->setFullyExited(true);
                ghost->setState(GhostState::Roaming);
            }

            // Zet initiÃƒÂ«le richting naar deur (voor ghosts in ghost house)
            if (i > 0) {
                if (ghostSpawns[i].y > doorY) {
                    ghost->setDirection(0.0f, -1.0f);
                }
                else if (ghostSpawns[i].x < doorX) {
                    ghost->setDirection(1.0f, 0.0f);
                }
                else if (ghostSpawns[i].x > doorX) {
                    ghost->setDirection(-1.0f, 0.0f);
                }
                else {
                    ghost->setDirection(0.0f, -1.0f);
                }
            }

            ghost->addObserver(score);
            ghosts.push_back(ghost);
        }
    }

    // STAP 4: Reset ghosts voor nieuwe levels (level > 1)
    if (currentLevel > 1) {
        for (auto& ghost : ghosts) {
            ghost->resetForNewLevel();
            ghost->setSpeed(BASE_SPEED + (currentLevel - 1) * GHOST_SPEED_INCREMENT);
        }
        if (pacman) {
            pacman->setPosition(pacmanStartX, pacmanStartY);
            pacman->setDirection(0, 0);
            pacman->queueDirection(0, 0);
        }
    }
}

void World::update() {
    float dt = Stopwatch::getInstance().getDeltaTime();

    // âœ… FIX: Stuur GameTick ALLEEN naar Score (1x per frame)
    GameEvent tickEvent(EventType::GameTick, dt);
    if (score) {
        score->onNotify(tickEvent);
    }

    // Update spawn/chasing/fear timers
    for (auto& ghost : ghosts) {
        ghost->updateSpawnTimer(dt);
        ghost->updateChasingTimer(dt);
        if (ghost->getFeared()) {
            ghost->updateFearTimer(dt);
        }
    }

    // Check win condition
    if (coins.empty() && fruits.empty() && pacman && !pacman->isDying()) {
        if (pacman) { pacman->notifyObservers(GameEvent(EventType::LevelCleared)); }
        nextLevel();
        return;
    }

    // Handle Pacman death animation
    if (pacman && pacman->isDying()) {
        pacman->updateDeathAnimation(dt);
        if (!pacman->isActive() && lives > 0) {
            pacman->setPosition(pacmanStartX, pacmanStartY);
            pacman->setDirection(0, 0);
            pacman->queueDirection(0, 0);
            pacman->setActive(true);
            pacman->setDying(false);
for (auto& ghost : ghosts) { ghost->respawn(currentLevel, true); }
            fearModeActive = false;
        }
        return;
    }

    // Update fear mode
    if (fearModeActive) {
        fearModeTimer -= dt;
        if (fearModeTimer <= 0.0f) {
            fearModeTimer = 0.0f;
            fearModeActive = false;
            for (auto& ghost : ghosts) {
                if (!ghost->getEaten()) { ghost->setFeared(false); }
            }
            if (pacman) { pacman->notifyObservers(GameEvent(EventType::FearModeEnded)); }
        } else {
            for (auto& ghost : ghosts) {
                if (ghost->getFeared()) { ghost->setFearTimeRemaining(fearModeTimer); }
            }
        }
    }

    updatePacman(dt);
    updateGhosts(dt);
    checkCollisions();
}

void World::nextLevel() {
    currentLevel++;
    if (currentLevel > 3) { gameWon = true; } else { initLevel(); }
}

void World::updatePacman(float dt) {
    if (!pacman) return;
    float oldX = pacman->getX(); float oldY = pacman->getY();
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    float gridX = (pacman->getX() - WORLD_MIN) / tileWidth;
    float gridY = (pacman->getY() - WORLD_MIN) / tileHeight;
    float diffX = std::abs(gridX - std::floor(gridX) - 0.5f);
    float diffY = std::abs(gridY - std::floor(gridY) - 0.5f);
    bool hasInput = (pacman->getNextDirX() != 0 || pacman->getNextDirY() != 0);
    if (hasInput) {
        bool canTurnX = (pacman->getNextDirX() != 0) && (diffY < GRID_TOLERANCE);
        bool canTurnY = (pacman->getNextDirY() != 0) && (diffX < GRID_TOLERANCE);
        if (canTurnX || canTurnY) {
            float testX = pacman->getX() + pacman->getNextDirX() * tileWidth * 0.6f;
            float testY = pacman->getY() + pacman->getNextDirY() * tileHeight * 0.6f;
            Entity ghostCheck(testX, testY, EntityType::Pacman);
            ghostCheck.setSize(pacman->getWidth(), pacman->getHeight());
            bool wallInWay = false;
            for (const auto& wall : walls) {
                if (ghostCheck.collidesWith(*wall)) { wallInWay = true; break; }
            }
            if (!wallInWay) {
                if (canTurnX) { pacman->setPosition(pacman->getX(), ((std::floor(gridY) + 0.5f) * tileHeight) + WORLD_MIN); }
                if (canTurnY) { pacman->setPosition(((std::floor(gridX) + 0.5f) * tileWidth) + WORLD_MIN, pacman->getY()); }
                pacman->setDirection(pacman->getNextDirX(), pacman->getNextDirY());
            }
        }
    }
    pacman->update(dt);
    for (const auto& wall : walls) {
        if (pacman->collidesWith(*wall)) { pacman->setPosition(oldX, oldY); break; }
    }
}

bool World::isIntersection(float x, float y) const {
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    int validDirCount = 0; float possibleDirs[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (int i = 0; i < 4; i++) {
        float dx = possibleDirs[i][0]; float dy = possibleDirs[i][1];
        float testX = x + dx * tileWidth * 0.5f; float testY = y + dy * tileHeight * 0.5f;
        Entity check(testX, testY, EntityType::Ghost);
        check.setSize(tileWidth * 0.8f, tileHeight * 0.8f);
        bool blocked = false;
        for (const auto& wall : walls) {
            if (check.collidesWith(*wall)) { blocked = true; break; }
        }
        if (!blocked) { validDirCount++; }
    }
    return validDirCount > 2;
}

void World::updateGhosts(float dt) {
    float tileWidth = WORLD_SIZE / MAP_WIDTH;
    float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    for (auto& ghost : ghosts) {
        if (!ghost->canMove()) continue;
        float oldX = ghost->getX(); float oldY = ghost->getY();
        float gridX = (ghost->getX() - WORLD_MIN) / tileWidth;
        float gridY = (ghost->getY() - WORLD_MIN) / tileHeight;
        int tileX = static_cast<int>(std::floor(gridX));
        int tileY = static_cast<int>(std::floor(gridY));
        bool atCenter = (std::abs(gridX - std::floor(gridX) - 0.5f) < GRID_TOLERANCE &&
                        std::abs(gridY - std::floor(gridY) - 0.5f) < GRID_TOLERANCE);
        updateGhostState(ghost);
        bool needsNewDirection = false;
        if (ghost->getState() == GhostState::Exiting || ghost->getState() == GhostState::Returning ||
            (ghost->getDirX() == 0 && ghost->getDirY() == 0)) {
            needsNewDirection = true;
        }
        else if (atCenter && isIntersection(ghost->getX(), ghost->getY())) {
            if (ghost->shouldRecalculateDirection(tileX, tileY)) {
                needsNewDirection = true;
                ghost->markIntersection(tileX, tileY);
            }
        }
        if (needsNewDirection) {
            updateGhostAI(ghost, dt);
            if (ghost->getState() == GhostState::Returning && atCenter) {
                ghost->markIntersection(tileX, tileY);
            }
        }
        ghost->update(dt);
        if (ghost->getState() == GhostState::Returning) continue;
        bool hitWall = false; bool hitGhostHouseBoundary = false;
        for (const auto& wall : walls) {
            if (ghost->collidesWith(*wall)) {
                hitWall = true;
                ghost->setPosition(oldX, oldY);
                break;
            }
        }
        if (!hitWall && !ghost->canEnterPosition(ghost->getX(), ghost->getY())) {
            hitGhostHouseBoundary = true;
            ghost->setPosition(oldX, oldY);
        }
        if (hitWall || hitGhostHouseBoundary) {
            ghost->clearIntersection();
            auto validDirs = getValidDirections(ghost);
            if (!validDirs.empty()) {
                std::vector<std::pair<float, float>> preferredDirs;
                for (const auto& dir : validDirs) {
                    if (!(dir.first == -ghost->getDirX() && dir.second == -ghost->getDirY())) {
                        preferredDirs.push_back(dir);
                    }
                }
                if (preferredDirs.empty()) { preferredDirs = validDirs; }
                int r = Random::getInstance().getInt(0, preferredDirs.size() - 1);
                ghost->setDirection(preferredDirs[r].first, preferredDirs[r].second);
                float snapX = (std::floor(gridX) + 0.5f) * tileWidth + WORLD_MIN;
                float snapY = (std::floor(gridY) + 0.5f) * tileHeight + WORLD_MIN;
                ghost->setPosition(snapX, snapY);
            } else {
                ghost->setDirection(-ghost->getDirX(), -ghost->getDirY());
            }
        }
    }
}

void World::updateGhostState(std::shared_ptr<Ghost> ghost) {
    if (!pacman) return;
    GhostState currentState = ghost->getState();
    switch (currentState) {
        case GhostState::Exiting:
            if (ghost->hasFullyExited()) {
                ghost->setState(ghost->shouldBeChasing() ? GhostState::Chasing : GhostState::Roaming);
            }
            break;
        case GhostState::Roaming:
            if (ghost->shouldBeChasing()) { ghost->setState(GhostState::Chasing); }
            if (fearModeActive && ghost->getFeared()) { ghost->setState(GhostState::Fleeing); }
            break;
        case GhostState::Chasing:
            if (fearModeActive && ghost->getFeared()) { ghost->setState(GhostState::Fleeing); }
            break;
        case GhostState::Fleeing:
            if (!fearModeActive || !ghost->getFeared()) {
                ghost->setState(ghost->shouldBeChasing() ? GhostState::Chasing : GhostState::Roaming);
            }
            break;
        case GhostState::Returning: {
            float dx = ghost->getStartX() - ghost->getX();
            float dy = ghost->getStartY() - ghost->getY();
            float distToHome = std::sqrt(dx * dx + dy * dy);
            if (distToHome < 0.15f) {
                ghost->respawn(currentLevel, false);
                ghost->setState(GhostState::Exiting);
            }
            break;
        }
    }
}

void World::updateGhostAI(std::shared_ptr<Ghost> ghost, float dt) {
    std::pair<float, float> newDir;
    GhostState state = ghost->getState();
    switch (state) {
        case GhostState::Exiting: newDir = getExitHouseDirection(ghost); break;
        case GhostState::Roaming: newDir = getRoamingDirection(ghost); break;
        case GhostState::Chasing: newDir = getChaseDirectionByAI(ghost); break;
        case GhostState::Fleeing: newDir = getFearDirection(ghost); break;
        case GhostState::Returning: newDir = getReturnHomeDirection(ghost); break;
    }
    if (newDir.first != ghost->getDirX() || newDir.second != ghost->getDirY()) {
        ghost->setDirection(newDir.first, newDir.second);
    }
}

std::pair<float, float> World::getChaseDirectionByAI(std::shared_ptr<Ghost> ghost) {
    GhostAI aiType = ghost->getAIType();
    switch (aiType) {
        case GhostAI::Chase: return getChaseDirection(ghost);
        case GhostAI::Ambush: return getAmbushDirection(ghost);
        case GhostAI::Random: return getRandomDirection(ghost);
        case GhostAI::LockedDirection: return getLockedDirection(ghost);
        default: return getChaseDirection(ghost);
    }
}

std::pair<float, float> World::getRoamingDirection(std::shared_ptr<Ghost> ghost) {
    auto validDirs = getValidDirections(ghost);
    if (validDirs.empty()) { return {-ghost->getDirX(), -ghost->getDirY()}; }
    float currentDirX = ghost->getDirX(); float currentDirY = ghost->getDirY();
    if (currentDirX != 0 || currentDirY != 0) {
        for (const auto& dir : validDirs) {
            if (dir.first == currentDirX && dir.second == currentDirY) {
                int randomChoice = Random::getInstance().getInt(0, 100);
                if (randomChoice < 80) { return {currentDirX, currentDirY}; }
                break;
            }
        }
    }
    std::vector<std::pair<float, float>> preferredDirs;
    for (const auto& dir : validDirs) {
        if (!(dir.first == -currentDirX && dir.second == -currentDirY)) {
            preferredDirs.push_back(dir);
        }
    }
    if (preferredDirs.empty()) { preferredDirs = validDirs; }
    int r = Random::getInstance().getInt(0, preferredDirs.size() - 1);
    return preferredDirs[r];
}

std::pair<float, float> World::getExitHouseDirection(std::shared_ptr<Ghost> ghost) {
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    float doorX = ghost->getExitTargetX(); float doorY = ghost->getExitTargetY();
    float dx = doorX - ghost->getX(); float dy = doorY - ghost->getY();
    float exitThreshold = tileHeight * 0.5f;
    if (ghost->getY() < (doorY - exitThreshold)) {
        ghost->setFullyExited(true);
        return {0.0f, -1.0f};
    }
    if (ghost->getY() > doorY) {
        if (std::abs(dx) > tileWidth * 0.3f) { return {(dx > 0) ? 1.0f : -1.0f, 0.0f}; }
        else { return {0.0f, -1.0f}; }
    }
    return {0.0f, -1.0f};
}

std::pair<float, float> World::getChaseDirection(std::shared_ptr<Ghost> ghost) {
    if (!pacman) return getRoamingDirection(ghost);
    auto validDirs = getValidDirections(ghost);
    if (validDirs.empty()) return {ghost->getDirX(), ghost->getDirY()};
    float minDist = 999999.0f; std::pair<float, float> bestDir = validDirs[0];
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    for (const auto& dir : validDirs) {
        float nextX = ghost->getX() + dir.first * tileWidth;
        float nextY = ghost->getY() + dir.second * tileHeight;
        float dist = Ghost::manhattanDistance(nextX, nextY, pacman->getX(), pacman->getY());
        if (dist < minDist) { minDist = dist; bestDir = dir; }
    }
    return bestDir;
}

std::pair<float, float> World::getAmbushDirection(std::shared_ptr<Ghost> ghost) {
    if (!pacman) return getRoamingDirection(ghost);
    auto validDirs = getValidDirections(ghost);
    if (validDirs.empty()) return {ghost->getDirX(), ghost->getDirY()};
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    float targetX = pacman->getX() + pacman->getDirX() * 4 * tileWidth;
    float targetY = pacman->getY() + pacman->getDirY() * 4 * tileHeight;
    float minDist = 999999.0f; std::pair<float, float> bestDir = validDirs[0];
    for (const auto& dir : validDirs) {
        float nextX = ghost->getX() + dir.first * tileWidth;
        float nextY = ghost->getY() + dir.second * tileHeight;
        float dist = Ghost::manhattanDistance(nextX, nextY, targetX, targetY);
        if (dist < minDist) { minDist = dist; bestDir = dir; }
    }
    return bestDir;
}

std::pair<float, float> World::getRandomDirection(std::shared_ptr<Ghost> ghost) {
    auto validDirs = getValidDirections(ghost);
    if (validDirs.empty()) { return {-ghost->getDirX(), -ghost->getDirY()}; }
    std::vector<std::pair<float, float>> preferredDirs;
    for (const auto& dir : validDirs) {
        if (!(dir.first == -ghost->getDirX() && dir.second == -ghost->getDirY())) {
            preferredDirs.push_back(dir);
        }
    }
    if (preferredDirs.empty()) { preferredDirs = validDirs; }
    int r = Random::getInstance().getInt(0, preferredDirs.size() - 1);
    return preferredDirs[r];
}

std::pair<float, float> World::getLockedDirection(std::shared_ptr<Ghost> ghost) {
    if (ghost->hasLocked()) {
        float lockedX = ghost->getLockedDirX(); float lockedY = ghost->getLockedDirY();
        float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
        float nextX = ghost->getX() + lockedX * tileWidth * 0.5f;
        float nextY = ghost->getY() + lockedY * tileHeight * 0.5f;
        Entity wallCheck(nextX, nextY, EntityType::Ghost);
        wallCheck.setSize(ghost->getWidth(), ghost->getHeight());
        bool wallAhead = false;
        for (const auto& wall : walls) {
            if (wallCheck.collidesWith(*wall)) { wallAhead = true; break; }
        }
        if (!wallAhead) {
            if (isIntersection(ghost->getX(), ghost->getY())) {
                int chance = Random::getInstance().getInt(0, 1);
                if (chance == 0) {
                    auto validDirs = getValidDirections(ghost);
                    if (!validDirs.empty()) {
                        std::vector<std::pair<float, float>> preferredDirs;
                        for (const auto& dir : validDirs) {
                            if (!(dir.first == -lockedX && dir.second == -lockedY)) {
                                preferredDirs.push_back(dir);
                            }
                        }
                        if (preferredDirs.empty()) { preferredDirs = validDirs; }
                        int r = Random::getInstance().getInt(0, preferredDirs.size() - 1);
                        ghost->setLockedDirection(preferredDirs[r].first, preferredDirs[r].second);
                        return preferredDirs[r];
                    }
                } else {
                    return {lockedX, lockedY};
                }
            } else {
                return {lockedX, lockedY};
            }
        } else {
            ghost->clearLockedDirection();
        }
    }
    auto validDirs = getValidDirections(ghost);
    if (validDirs.empty()) { return {-ghost->getDirX(), -ghost->getDirY()}; }
    std::vector<std::pair<float, float>> preferredDirs;
    for (const auto& dir : validDirs) {
        if (!(dir.first == -ghost->getDirX() && dir.second == -ghost->getDirY())) {
            preferredDirs.push_back(dir);
        }
    }
    if (preferredDirs.empty()) { preferredDirs = validDirs; }
    int r = Random::getInstance().getInt(0, preferredDirs.size() - 1);
    ghost->setLockedDirection(preferredDirs[r].first, preferredDirs[r].second);
    return preferredDirs[r];
}

std::pair<float, float> World::getFearDirection(std::shared_ptr<Ghost> ghost) {
    if (!pacman) return getRoamingDirection(ghost);
    auto validDirs = getValidDirections(ghost);
    if (validDirs.empty()) return {-ghost->getDirX(), -ghost->getDirY()};
    float maxDist = -1.0f; std::pair<float, float> bestDir = validDirs[0];
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    for (const auto& dir : validDirs) {
        float nextX = ghost->getX() + dir.first * tileWidth;
        float nextY = ghost->getY() + dir.second * tileHeight;
        float dist = Ghost::manhattanDistance(nextX, nextY, pacman->getX(), pacman->getY());
        if (dist > maxDist) { maxDist = dist; bestDir = dir; }
    }
    return bestDir;
}

std::pair<float, float> World::getReturnHomeDirection(std::shared_ptr<Ghost> ghost) {
    float targetX = ghost->getStartX(); float targetY = ghost->getStartY();
    float dx = targetX - ghost->getX(); float dy = targetY - ghost->getY();
    float distToHome = std::sqrt(dx * dx + dy * dy);
    if (distToHome < 0.1f) {
        ghost->respawn(currentLevel);
        ghost->setState(GhostState::Exiting);
        return {0.0f, 0.0f};
    }
    if (std::abs(dx) > std::abs(dy)) { return {(dx > 0) ? 1.0f : -1.0f, 0.0f}; }
    else { return {0.0f, (dy > 0) ? 1.0f : -1.0f}; }
}

std::vector<std::pair<float, float>> World::getValidDirections(std::shared_ptr<Ghost> ghost) {
    std::vector<std::pair<float, float>> validDirs;
    float possibleDirs[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    for (int i = 0; i < 4; i++) {
        float dx = possibleDirs[i][0]; float dy = possibleDirs[i][1];
        float testX = ghost->getX() + dx * tileWidth;
        float testY = ghost->getY() + dy * tileHeight;
        if (ghost->getState() != GhostState::Returning && !ghost->canEnterPosition(testX, testY)) { continue; }
        Entity check(testX, testY, EntityType::Ghost);
        check.setSize(ghost->getWidth(), ghost->getHeight());
        bool blocked = false;
        for (const auto& wall : walls) {
            if (check.collidesWith(*wall)) { blocked = true; break; }
        }
        if (!blocked) { validDirs.push_back({dx, dy}); }
    }
    if (validDirs.empty() && (ghost->getDirX() != 0 || ghost->getDirY() != 0)) {
        float test180X = ghost->getX() + (-ghost->getDirX()) * tileWidth;
        float test180Y = ghost->getY() + (-ghost->getDirY()) * tileHeight;
        if (ghost->getState() == GhostState::Returning || ghost->canEnterPosition(test180X, test180Y)) {
            validDirs.push_back({-ghost->getDirX(), -ghost->getDirY()});
        }
    }
    if (validDirs.empty()) { validDirs.push_back({0.0f, -1.0f}); }
    return validDirs;
}

void World::checkCollisions() {
    if (!pacman || pacman->isDying()) return;
    for (auto it = coins.begin(); it != coins.end(); ) {
        if ((*it)->isActive() && pacman->collidesWith(**it)) {
            (*it)->setActive(false);
            (*it)->notifyObservers(GameEvent(EventType::CoinCollected));
            it = coins.erase(it);
        } else { ++it; }
    }
    for (auto it = powerPellets.begin(); it != powerPellets.end(); ) {
        if ((*it)->isActive() && pacman->collidesWith(**it)) {
            (*it)->setActive(false);
            (*it)->notifyObservers(GameEvent(EventType::PowerPelletCollected));
            it = powerPellets.erase(it);
        } else { ++it; }
    }
    for (auto it = fruits.begin(); it != fruits.end(); ) {
        if ((*it)->isActive() && pacman->collidesWith(**it)) {
            (*it)->setActive(false);
            (*it)->notifyObservers(GameEvent(EventType::FruitCollected));
            fearModeActive = true;
            fearModeTimer = fearModeDuration;
            for (auto& ghost : ghosts) {
                if (!ghost->getEaten()) {
                    ghost->setFeared(true);
                    ghost->setFearTimeRemaining(fearModeTimer);
                }
            }
            if (pacman) { pacman->notifyObservers(GameEvent(EventType::FearModeStarted)); }
            it = fruits.erase(it);
        } else { ++it; }
    }
    for (auto& ghost : ghosts) {
        if (ghost->canMove() && ghost->collidesWith(*pacman)) {
            if (ghost->getEaten()) { continue; }
            else if (fearModeActive && ghost->getFeared()) {
                ghost->notifyObservers(GameEvent(EventType::GhostEaten));
                ghost->setFeared(false);
                ghost->setEaten(true);
            } else {
                handlePacmanDeath();
                break;
            }
        }
    }
}

void World::handlePacmanDeath() {
    lives--;
    if (pacman) { pacman->notifyObservers(GameEvent(EventType::PacmanDied)); }
    if (pacman) { pacman->setDying(true); }
    for (auto& ghost : ghosts) {
        if (!ghost->getEaten()) { ghost->setSpeed(0.0f); }
    }
    if (lives <= 0) { gameOver = true; }
}

bool World::isWallInDirection(float x, float y, float dx, float dy) {
    float tileWidth = WORLD_SIZE / MAP_WIDTH; float tileHeight = WORLD_SIZE / MAP_HEIGHT;
    float testX = x + dx * tileWidth * 0.5f; float testY = y + dy * tileHeight * 0.5f;
    Entity check(testX, testY, EntityType::Ghost);
    check.setSize(tileWidth * 0.8f, tileHeight * 0.8f);
    for (const auto& wall : walls) {
        if (check.collidesWith(*wall)) { return true; }
    }
    return false;
}