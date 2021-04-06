#pragma once
#include <cstdint>

class StateContext {
public:
    enum class SimulatorState {
        UNINITIALIZED, // initial state
        IDLE,          // ready to start simulation
        IN_PROGRESS,   // simulation in progress
        MATH_TEST      // math test 
    };

private:
    SimulatorState currentState{ SimulatorState::UNINITIALIZED };

public:
    StateContext() = default;
    ~StateContext() = default;

    [[nodiscard]] inline bool IsInitialized() const noexcept { return currentState != SimulatorState::UNINITIALIZED; }
    [[nodiscard]] inline bool IsInProgress()  const noexcept { return currentState == SimulatorState::IN_PROGRESS; }
    [[nodiscard]] inline bool IsIdle()        const noexcept { return currentState == SimulatorState::IDLE; }
    [[nodiscard]] inline bool IsMathTest()    const noexcept { return currentState == SimulatorState::MATH_TEST; } //todo:!!

    void SetState(SimulatorState newState) noexcept { currentState = newState; }
};