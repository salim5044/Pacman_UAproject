#include "Random.h"

Random::Random() {
    std::random_device rd;
    mt.seed(rd());
}

Random& Random::getInstance() {
    static Random instance;
    return instance;
}

int Random::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

float Random::getFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}