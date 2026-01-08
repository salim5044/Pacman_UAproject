#pragma once
#include <random>

class Random {
private:
    std::mt19937 mt;

    Random();

public:
    Random(const Random&) = delete;
    void operator=(const Random&) = delete;

    static Random& getInstance();

    int getInt(int min, int max);
    float getFloat(float min, float max);
};