#pragma once
#include <chrono>

class Stopwatch {
private:
    std::chrono::high_resolution_clock::time_point lastTime;
    float deltaTime;

    // Private constructor voor Singleton
    Stopwatch();

public:
    // Delete copy constructors
    Stopwatch(const Stopwatch&) = delete;
    void operator=(const Stopwatch&) = delete;

    static Stopwatch& getInstance();

    // Wordt elke frame aangeroepen om tijdverschil te berekenen
    void tick();

    float getDeltaTime() const;
};