#pragma once

#include <cstdint>
// still to do
enum SimulatorState {
    UNINITIALIZED, 
    READY, 
    IN_PROGRESS, 
    IDLE,
    MATH_TEST
};


class StateContext {
    uint8_t currentState{ UNINITIALIZED };

public:
    StateContext() = default;
    ~StateContext() = default;

    [[nodiscard]] inline bool IsInitialized() const noexcept { return currentState != UNINITIALIZED; }
    [[nodiscard]] inline bool IsReady() const noexcept { return currentState == READY; }
    [[nodiscard]] inline bool IsInProgress() const noexcept { return currentState == IN_PROGRESS; }
    [[nodiscard]] inline bool IsIdle() const noexcept { return currentState == IDLE; }
    [[nodiscard]] inline bool IsMathTest() const noexcept { return currentState == MATH_TEST; }

    void SetState(SimulatorState newState) noexcept { currentState = newState; }
};