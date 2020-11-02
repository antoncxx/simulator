#pragma once

#include <list>
#include <array>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class MouseButton {
    Left, Right, Middle, Unsupported
};

enum class KeyboardButton {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Unsupported
};

struct KeyState {
    bool pressed;
};

struct MouseState {
    glm::vec2 offset{};
    glm::vec2 position{};

    std::array<KeyState, 3> buttons = {};
};

struct KeyboardState {
    std::array<KeyState, 26> buttons = {};
};

class InputsCallbacksWrapper;

class InputHandler {
    explicit InputHandler() = default;

    MouseState              mouseState{};
    KeyboardState           keyboardState{};
public:
    static InputHandler& Instance() noexcept;
    void Setup(GLFWwindow* window);

    inline const auto MouseState() const noexcept { return mouseState; }
    inline const auto KeyboardState() const noexcept { return keyboardState; }

    friend InputsCallbacksWrapper;
};