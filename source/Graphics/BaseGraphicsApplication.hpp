#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class BaseGraphicsApplication {
    bool        isCreated{ false };
public:
    inline auto IsCreated() const noexcept { return isCreated; };

    virtual void Run();
    virtual void Finalize();
    virtual void Initialize();
    virtual void OnEveryFrame(float delta);

    explicit BaseGraphicsApplication();
    virtual ~BaseGraphicsApplication();

    BaseGraphicsApplication(const BaseGraphicsApplication&) = delete;
    BaseGraphicsApplication& operator=(const BaseGraphicsApplication&) = delete;

private:
    void CreateApplication();
    void DestroyApplication();

protected:
    GLFWwindow* renderWindow{ nullptr };
};