#pragma 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "InputHandler.hpp"

namespace {
    struct CameraData {
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 up;

        float yaw{ -90.f };
        float pitch{ 0.f };

        float as{};
        float fov{};
        float far{};
        float near{};

        float movementSpeed{ 30.25f };
        float mouseSensitivity{ 10.25f };

    };
}

// Floatable camera with perspective projection
class Camera {
    glm::mat4  view{};
    glm::mat4  projection{};
    CameraData data{};

public:
    explicit Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
    ~Camera() = default;

    inline auto ViewMatrix() const noexcept { return view; };
    inline auto ProjectionMatrix() const noexcept { return projection; }

    void Update(float delta);

    void SetFov(float fov);
    void SetPlanes(float near, float far);
    void SetAspectRatio(float as);
private:
    void CalculateViewMatrix();
    void CalculateProjectionMatrix();

    void ProcessMouse(float delta);
    void ProcessKeyboard(float delta);
};