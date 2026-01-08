#include "Stopwatch.h"

Stopwatch::Stopwatch() {
    lastTime = std::chrono::high_resolution_clock::now();
    deltaTime = 0.0f;
}

Stopwatch& Stopwatch::getInstance() {
    static Stopwatch instance;
    return instance;
}

void Stopwatch::tick() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = currentTime - lastTime;
    deltaTime = duration.count();
    lastTime = currentTime;
}

float Stopwatch::getDeltaTime() const {
    return deltaTime;
}