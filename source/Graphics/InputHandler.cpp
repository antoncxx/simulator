#include "InputHandler.hpp"

#include <cassert>
#include <functional>


static inline MouseButton decodeGLFWMouseButton(int32_t code) {
    switch (code)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return MouseButton::Right;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return MouseButton::Middle;
    default:
        return MouseButton::Unsupported;
    }
}

static inline KeyboardButton decodeGLFWKeyboardButton(int32_t code) {
    switch (code)
    {
    case GLFW_KEY_A:
        return KeyboardButton::A;
    case GLFW_KEY_B:
        return KeyboardButton::B;
    case GLFW_KEY_C:
        return KeyboardButton::C;
    case GLFW_KEY_D:
        return KeyboardButton::D;
    case GLFW_KEY_E:
        return KeyboardButton::E;
    case GLFW_KEY_F:
        return KeyboardButton::F;
    case GLFW_KEY_G:
        return KeyboardButton::G;
    case GLFW_KEY_I:
        return KeyboardButton::I;
    case GLFW_KEY_J:
        return KeyboardButton::J;
    case GLFW_KEY_K:
        return KeyboardButton::K;
    case GLFW_KEY_L:
        return KeyboardButton::L;
    case GLFW_KEY_M:
        return KeyboardButton::M;
    case GLFW_KEY_N:
        return KeyboardButton::N;
    case GLFW_KEY_O:
        return KeyboardButton::O;
    case GLFW_KEY_P:
        return KeyboardButton::P;
    case GLFW_KEY_Q:
        return KeyboardButton::Q;
    case GLFW_KEY_R:
        return KeyboardButton::R;
    case GLFW_KEY_S:
        return KeyboardButton::S;
    case GLFW_KEY_T:
        return KeyboardButton::T;
    case GLFW_KEY_U:
        return KeyboardButton::U;
    case GLFW_KEY_V:
        return KeyboardButton::V;
    case GLFW_KEY_W:
        return KeyboardButton::W;
    case GLFW_KEY_X:
        return KeyboardButton::X;
    case GLFW_KEY_Y:
        return KeyboardButton::Y;
    case GLFW_KEY_Z:
        return KeyboardButton::Z;
    default:
        return KeyboardButton::Unsupported;
    }
}

class InputsCallbacksWrapper {
public:
    static void OnScrollCallback(GLFWwindow*, double xoff, double yoff) {
        auto& instance = InputHandler::Instance();
        instance.mouseState.offset += glm::vec2{ xoff, yoff };
    }

    static void OnMouseMoveCallback(GLFWwindow*, double xpos, double ypos) {
        auto& instance = InputHandler::Instance();
        instance.mouseState.position = glm::vec2{ xpos, ypos };
    }

    static void OnMouseButtonCallback(GLFWwindow*, int32_t button, int32_t action, int32_t mods) {
        auto& instance = InputHandler::Instance();

        MouseButton index = decodeGLFWMouseButton(button);
        if (index == MouseButton::Unsupported) { return; }

        instance.mouseState.buttons[static_cast<size_t>(index)].pressed = (action == GLFW_PRESS);
    }

    static void OnKeyBoardCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
        auto& instance = InputHandler::Instance();
        KeyboardButton index = decodeGLFWKeyboardButton(key);
        if (index == KeyboardButton::Unsupported) { return; }

        if (action == GLFW_PRESS) {
            auto iter = instance.keyboardCallbacks.find(index);
            if (iter != instance.keyboardCallbacks.end()) {
                for (auto callback : iter->second) {
                    callback();
                }
            }
        }

        instance.keyboardState.buttons[static_cast<size_t>(index)].pressed = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
    }
};


InputHandler& InputHandler::Instance() noexcept {
    static InputHandler instance;
    return instance;
}

void InputHandler::Setup(GLFWwindow* window) {
    assert(window != nullptr);
    glfwSetScrollCallback(window, &InputsCallbacksWrapper::OnScrollCallback);
    glfwSetMouseButtonCallback(window, &InputsCallbacksWrapper::OnMouseButtonCallback);
    glfwSetCursorPosCallback(window, &InputsCallbacksWrapper::OnMouseMoveCallback);
    glfwSetKeyCallback(window, &InputsCallbacksWrapper::OnKeyBoardCallback);
}

void InputHandler::RegisterKeyboardCallback(KeyboardButton button, KeyCallback callback) {
    keyboardCallbacks[button].push_back(callback);
}



