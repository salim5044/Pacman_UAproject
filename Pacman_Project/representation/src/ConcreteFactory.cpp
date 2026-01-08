#include "ConcreteFactory.h"
#include "PacmanEntityView.h"
#include "GhostEntityView.h"
#include "SimpleEntityView.h"
#include <stdexcept>

ConcreteFactory::ConcreteFactory(sf::RenderWindow& w, std::vector<std::shared_ptr<EntityView>>& v, const Camera& cam)
    : window(w), views(v), camera(cam), ghostCount(0) {

    bool loaded = false;
    if (spriteSheet.loadFromFile("assets/sprites.png")) {
        loaded = true;
    } else if (spriteSheet.loadFromFile("../assets/sprites.png")) {
        loaded = true;
    } else if (spriteSheet.loadFromFile("representation/assets/sprites.png")) {
        loaded = true;
    }

    if (!loaded) {
        throw std::runtime_error(
            "FATAL ERROR: Cannot load sprite sheet!\n"
            "Tried paths:\n"
            "  - assets/sprites.png\n"
            "  - ../assets/sprites.png\n"
            "  - representation/assets/sprites.png\n"
            "Please ensure sprites.png exists in one of these locations."
        );
    }

    sf::Image wallImage;
    wallImage.create(50, 50, sf::Color::Blue);
    wallTexture.loadFromImage(wallImage);
}

std::shared_ptr<Entity> ConcreteFactory::createPacman(float x, float y) {
    auto entity = std::make_shared<Entity>(x, y, EntityType::Pacman);

    auto view = std::make_shared<PacmanEntityView>(entity, window, spriteSheet, camera);
    entity->addObserver(view);
    views.push_back(view);

    return entity;
}

std::shared_ptr<Ghost> ConcreteFactory::createGhost(float x, float y, GhostAI ai, int variant, float delay, float chaseDelay) {
    auto ghost = std::make_shared<Ghost>(x, y, ai, variant, delay, chaseDelay);

    // The constructor receives std::shared_ptr<Ghost>, not Entity
    auto view = std::make_shared<GhostEntityView>(ghost, window, spriteSheet, camera, ghostCount);
    ghostCount++;

    ghost->addObserver(view);
    views.push_back(view);

    return ghost;
}

std::shared_ptr<Entity> ConcreteFactory::createWall(float x, float y) {
    auto entity = std::make_shared<Entity>(x, y, EntityType::Wall);

    auto view = std::make_shared<SimpleEntityView>(entity, window, wallTexture, camera);
    entity->addObserver(view);
    views.push_back(view);

    return entity;
}

std::shared_ptr<Entity> ConcreteFactory::createCoin(float x, float y) {
    auto entity = std::make_shared<Entity>(x, y, EntityType::Coin);

    auto view = std::make_shared<SimpleEntityView>(entity, window, spriteSheet, camera);
    entity->addObserver(view);
    views.push_back(view);

    return entity;
}

std::shared_ptr<Entity> ConcreteFactory::createPowerPellet(float x, float y) {
    auto entity = std::make_shared<Entity>(x, y, EntityType::PowerPellet);

    auto view = std::make_shared<SimpleEntityView>(entity, window, spriteSheet, camera);
    entity->addObserver(view);
    views.push_back(view);

    return entity;
}

std::shared_ptr<Entity> ConcreteFactory::createFruit(float x, float y) {
    auto entity = std::make_shared<Entity>(x, y, EntityType::Fruit);

    auto view = std::make_shared<SimpleEntityView>(entity, window, spriteSheet, camera);
    entity->addObserver(view);
    views.push_back(view);

    return entity;
}

void ConcreteFactory::resetGhostCount() {
    ghostCount = 0;
}