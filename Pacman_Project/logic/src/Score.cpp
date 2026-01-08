#include "Score.h"

void Score::onNotify(const GameEvent& event) {
    switch (event.type) {
        case EventType::CoinCollected: {
            float bonus = 1.0f - (timeSinceLastCoin / maxCoinDelay);
            bonus = std::max(0.5f, std::min(1.0f, bonus));
            currentScore += static_cast<int>(COIN_SCORE * bonus);
            timeSinceLastCoin = 0.0f;
            decayAccumulator = 0.0f;
            break;
        }
        case EventType::PowerPelletCollected:
            currentScore += POWER_PELLET_SCORE;
            break;
        case EventType::FruitCollected:
            currentScore += FRUIT_SCORE;
            break;
        case EventType::GhostEaten:
            currentScore += GHOST_SCORE;
            break;
        case EventType::LevelCleared:
            currentScore += LEVEL_CLEAR_BONUS;
            break;
        case EventType::GameTick:
            timeSinceLastCoin += event.value;


            if (currentScore > 0 && timeSinceLastCoin > maxCoinDelay) {
                decayAccumulator += event.value; // Accumuleer deltaTime

                // Trek 1 punt af per seconde (decayRate)
                while (decayAccumulator >= decayRate && currentScore > 0) {
                    currentScore -= 1;
                    decayAccumulator -= decayRate;
                }
            }
            break;
        default:
            break;
    }
}

void Score::reset() {
    currentScore = 0;
    timeSinceLastCoin = 0.0f;
    decayAccumulator = 0.0f;
}

std::vector<int> Score::loadHighscores() {
    std::vector<int> scores;
    std::ifstream file("scores.txt");
    int s;
    while (file >> s) {
        scores.push_back(s);
    }

    std::sort(scores.rbegin(), scores.rend());

    while (scores.size() < 5) {
        scores.push_back(0);
    }

    return scores;
}

void Score::saveHighscore(int newScore) {
    std::vector<int> scores;
    std::ifstream inFile("scores.txt");
    int s;
    while (inFile >> s) {
        if (s > 0) {
            scores.push_back(s);
        }
    }
    inFile.close();

    scores.push_back(newScore);
    std::sort(scores.rbegin(), scores.rend());

    if (scores.size() > 5) {
        scores.resize(5);
    }

    std::ofstream outFile("scores.txt");
    for (int score : scores) {
        outFile << score << "\n";
    }
    outFile.close();
}

std::string Score::getHighscoreString() {
    std::vector<int> scores = loadHighscores();
    std::string text = "Highscores:\n";
    for (size_t i = 0; i < scores.size() && i < 5; ++i) {
        text += std::to_string(i + 1) + ". " + std::to_string(scores[i]) + "\n";
    }
    return text;
}