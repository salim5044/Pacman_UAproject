#pragma once
#include "Gamestate.h"
#include "PausedState.h"
#include <stack>
#include <memory>
#include <vector>

class StateManager {
private:
    std::stack<std::shared_ptr<GameState>> states;

public:
    void pushState(std::shared_ptr<GameState> state) {
        // Pause current state if exists
        if (!states.empty()) {
            states.top()->onPause();
        }

        state->setManager(this);
        states.push(state);
        state->onEnter();
    }

    void popState() {
        if (!states.empty()) {
            states.top()->onExit();
            states.pop();

            // Resume previous state if exists
            if (!states.empty()) {
                states.top()->onResume();
            }
        }
    }

    void changeState(std::shared_ptr<GameState> state) {
        if (!states.empty()) {
            states.top()->onExit();
            states.pop();
        }

        state->setManager(this);
        states.push(state);
        state->onEnter();
    }

    // Dit cleared ALLE states en pusht dan de nieuwe state
    // Veilig om aan te roepen vanuit een state zelf
    void replaceAllStates(std::shared_ptr<GameState> newState) {
        // Exit alle states
        while (!states.empty()) {
            states.top()->onExit();
            states.pop();
        }

        // Push nieuwe state
        newState->setManager(this);
        states.push(newState);
        newState->onEnter();
    }

    void handleInput(const sf::Event& event) {
        if (!states.empty()) {
            states.top()->handleInput(event);
        }
    }

    void update(float dt) {
        if (!states.empty()) {
            states.top()->update(dt);
        }
    }

    void draw(sf::RenderWindow& window) {
        // Draw all states from bottom to top (so paused game shows below pause menu)
        std::vector<std::shared_ptr<GameState>> stateList;
        auto tempStack = states;

        while (!tempStack.empty()) {
            stateList.push_back(tempStack.top());
            tempStack.pop();
        }

        // Draw in reverse order (bottom first)
        for (auto it = stateList.rbegin(); it != stateList.rend(); ++it) {
            (*it)->draw(window);
        }
    }

    bool isEmpty() const {
        return states.empty();
    }
};