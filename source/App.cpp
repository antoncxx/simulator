#include "App.hpp"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void App::Initialize() {
    BaseGraphicsApplication::Initialize();
    InputHandler::Instance().Setup(renderWindow);
    UI::Instance().RegisterListener(this);
    Physics::Instance().StartUp();

    shader = ResourceManager::Instance().CreateShader("DefaultShader", "Resources/Shaders/Default.vs", "Resources/Shaders/Default.fs");
    debug_shader = ResourceManager::Instance().CreateShader("DebugShader", "Resources/Shaders/Debug.vs", "Resources/Shaders/Debug.fs");
    
    camera.reset(new Camera({ 10.f, 80.f, 80.f }));

    camera->SetAspectRatio(1.75f);
    camera->SetFov(45.f);
    camera->SetPlanes(0.1f, 300.f);

    roulleteController = RoulleteController::Create();
    ballController     = BallController::Create();
}

void App::Finalize() {
    UI::Instance().UnregisterListener(this);
    ResourceManager::Instance().Cleanup();
    Physics::Instance().ShutDown();
    BaseGraphicsApplication::Finalize();
}

void App::OnEveryFrame(float delta) {
    UpdateViewPort();
    UpdateComponets(delta);
    DrawComponents(delta);
    DrawPhysicsDebugWorld(delta);
}

void App::UpdateViewPort() {
    static int32_t w(-1), h(-1);

    int32_t currentw{}, currenth{};
    glfwGetFramebufferSize(renderWindow, &currentw, &currenth);

    if ((currentw != w) || (currenth != h)) {
        w = currentw;
        h = currenth;
        camera->SetAspectRatio((float)w / h);
        glViewport(0, 0, w, h);
    }

}

void App::UpdateComponets(float delta) {
    camera->Update(delta);
    ballController->Update(delta);
    roulleteController->Update(delta);

    SimulationStep(delta);
}

void App::DrawComponents(float delta) {
    shader->Use();
    shader->SetUniform("view", camera->ViewMatrix());
    shader->SetUniform("projection", camera->ProjectionMatrix());

    roulleteController->Draw(shader);
    ballController->Draw(shader);
}

void App::OnUIUpdate() {
    float framerate = ImGui::GetIO().Framerate;
    std::string title = "Roullete Simulator. Framerate: " + std::to_string(framerate);
    glfwSetWindowTitle(renderWindow, title.c_str());
}

void App::DrawPhysicsDebugWorld(float delta) {
    // todo

}

void App::SimulationStep(float dt) {
    auto* scene = Physics::Instance().GetScene();
    scene->simulate(2.5 * dt);
    scene->fetchResults(true);
}