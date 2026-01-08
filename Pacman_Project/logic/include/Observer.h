#pragma once
#include "Event.h"
#include <memory>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(const GameEvent& event) = 0;
};

class Subject {
private:
    std::vector<std::weak_ptr<Observer>> observers;

public:
    void addObserver(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }

    void notifyObservers(const GameEvent& event) {
        // Clean up expired observers
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [](const std::weak_ptr<Observer>& wp) { return wp.expired(); }),
            observers.end()
        );

        // Notify all active observers
        for (auto& weakObs : observers) {
            if (auto obs = weakObs.lock()) {
                obs->onNotify(event);
            }
        }
    }
};