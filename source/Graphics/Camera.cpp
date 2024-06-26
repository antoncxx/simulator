#include "Camera.hpp"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front) {
    data.position = position;
    data.up       = up;
    data.front    = front;
    data.worldUp  = up;

    CalculateViewMatrix();
}


void Camera::CalculateViewMatrix() {
    glm::vec3 direction = {
        cos(glm::radians(data.yaw)) * cos(glm::radians(data.pitch)),
        sin(glm::radians(data.pitch)),
        sin(glm::radians(data.yaw)) * cos(glm::radians(data.pitch))
    };

    data.front = glm::normalize(direction);
    data.right = glm::normalize(glm::cross(data.front, data.worldUp)); 
    data.up    = glm::normalize(glm::cross(data.right, data.front));

    view = glm::lookAt(data.position, data.position + data.front, data.up);
}

void Camera::Update(float delta) {
    ProcessMouse(delta);
    ProcessKeyboard(delta);
}

void Camera::ProcessMouse(float delta) {
    static glm::vec2 previousPos = {};
    const auto& ms = InputHandler::Instance().MouseState();

    if (ms.buttons[static_cast<size_t>(MouseButton::Right)].pressed) {

        auto currentPos = ms.position;
        auto mouseOffset = currentPos - previousPos;

        data.yaw   += data.mouseSensitivity * delta * mouseOffset.x;
        data.pitch -= data.mouseSensitivity * delta * mouseOffset.y;

        data.pitch = std::clamp(data.pitch, -89.f, 89.f);
        CalculateViewMatrix();
    } 

    previousPos = ms.position;
}

void Camera::ProcessKeyboard(float delta) {
    const auto& ks = InputHandler::Instance().KeyboardState();
    const auto velocity = data.movementSpeed * delta;
    bool dirty{ false };

    if (ks.buttons[static_cast<size_t>(KeyboardButton::W)].pressed) {
        data.position += data.front * velocity;
        dirty = true;
    }

    if (ks.buttons[static_cast<size_t>(KeyboardButton::S)].pressed) {
        data.position -= data.front * velocity;
        dirty = true;
    }

    if (ks.buttons[static_cast<size_t>(KeyboardButton::A)].pressed) {
        data.position -= data.right * velocity;
        dirty = true;
    }

    if (ks.buttons[static_cast<size_t>(KeyboardButton::D)].pressed) {
        data.position += data.right * velocity;
        dirty = true;
    }

    if (dirty) {
        CalculateViewMatrix();
    }
}

void Camera::SetFov(float fov) {
    if (data.fov != fov) {
        data.fov = fov;
        CalculateProjectionMatrix();
    }
}

void Camera::SetPlanes(float near, float far) {
    if (data.far != far || data.near != near) {
        data.far = far;
        data.near = near;
        CalculateProjectionMatrix();
    }
}

void Camera::SetAspectRatio(float as) {
    if (data.as != as) {
        data.as = as;
        CalculateProjectionMatrix();
    }
}

void Camera::CalculateProjectionMatrix() {
    projection = glm::perspective(glm::radians(data.fov), data.as, data.near, data.far);
}