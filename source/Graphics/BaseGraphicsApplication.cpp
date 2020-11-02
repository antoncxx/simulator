#include "BaseGraphicsApplication.hpp"
#include <chrono>

namespace {
    constexpr int32_t           sDefaultWidth  = 600;
    constexpr int32_t           sDefaultHeight = 480;
    constexpr const char* const sDefaultTitle  = "Roullete Simulator";
}

BaseGraphicsApplication::BaseGraphicsApplication() {
    CreateApplication();
}

BaseGraphicsApplication::~BaseGraphicsApplication() {
    DestroyApplication();
}

void BaseGraphicsApplication::Run() {
    auto previousFrame = glfwGetTime();
    auto currentFrame  = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);


    while (!glfwWindowShouldClose(renderWindow)) {
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentFrame = glfwGetTime();
        const auto delta = currentFrame - previousFrame;
        previousFrame = currentFrame;

        glfwPollEvents();
        OnEveryFrame(delta);
        glfwSwapBuffers(renderWindow);
    }
}

void BaseGraphicsApplication::Finalize() {

}

void BaseGraphicsApplication::Initialize() {

}

void BaseGraphicsApplication::OnEveryFrame(float delta) {

}

void BaseGraphicsApplication::CreateApplication() {
    isCreated = true;
    
    isCreated &= (glfwInit() == GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    renderWindow = glfwCreateWindow(sDefaultWidth, sDefaultHeight, sDefaultTitle, nullptr, nullptr);
    isCreated &= renderWindow != nullptr;

    glfwMakeContextCurrent(renderWindow);

    glewExperimental = GL_TRUE;
    isCreated &= (glewInit() == GLEW_OK);
}

void BaseGraphicsApplication::DestroyApplication() {
    if (renderWindow != nullptr) {
        glfwDestroyWindow(renderWindow);
        renderWindow = nullptr;
    }

    glfwTerminate();
}