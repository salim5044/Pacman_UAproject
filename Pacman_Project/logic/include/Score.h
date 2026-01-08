#pragma once
#include "Observer.h"
#include "Config.h"
#include "Stopwatch.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

class Score : public Observer {
private:
    int currentScore = 0;
    float timeSinceLastCoin = 0.0f;
    float maxCoinDelay = 5.0f; // Max tijd voor bonus

    //Voor framerate-onafhankelijke score decay
    float decayAccumulator = 0.0f;
    float decayRate = 1.0f; // 1 punt per seconde

public:
    Score() = default;

    void onNotify(const GameEvent& event) override;
    int getScore() const { return currentScore; }
    void reset();

    // Highscore management
    static std::vector<int> loadHighscores();
    static void saveHighscore(int newScore);
    static std::string getHighscoreString();
};