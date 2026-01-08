#pragma once

enum class EventType {
    EntityMoved,
    CoinCollected,
    PowerPelletCollected,
    FruitCollected,
    GhostEaten,
    PacmanDied,
    DirectionChanged,
    LevelCleared,
    GameTick,
    FearModeStarted,
    FearModeEnded
};

struct GameEvent {
    EventType type;
    float value = 0.0f;
    void* data = nullptr;

    GameEvent(EventType t, float v = 0.0f, void* d = nullptr)
        : type(t), value(v), data(d) {}
};