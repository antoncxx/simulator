#include "App.hpp"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Converter.hpp"

void App::Initialize() {
    BaseGraphicsApplication::Initialize();
    InputHandler::Instance().Setup(renderWindow);
    UI::Instance().RegisterListener(this);
    Physics::Instance().StartUp();

    skyboxShader   = ResourceManager::Instance().CreateShader("SkyboxShader", "Resources/Shaders/Skybox.vs", "Resources/Shaders/Skybox.fs");
    
    std::vector<std::filesystem::path> faces
    {
        "Resources\\Textures\\right.jpg",
        "Resources\\Textures\\left.jpg",
        "Resources\\Textures\\top.jpg",
        "Resources\\Textures\\bottom.jpg",
        "Resources\\Textures\\front.jpg",
        "Resources\\Textures\\back.jpg"
    };
    skybox = ResourceManager::Instance().CreateSkybox("Sky", faces);

    camera.reset(new Camera({ 10.f, 80.f, 80.f }));

    camera->SetAspectRatio(1.75f);
    camera->SetFov(45.f);
    camera->SetPlanes(0.1f, 300.f);

    roulleteController = RoulleteController::Create();
    ballController     = BallController::Create();

    InputHandler::Instance().RegisterKeyboardCallback(KeyboardButton::N, std::bind(&App::StartNewRound, this));
    
    state.SetState(StateContext::SimulatorState::IDLE);
    auto defaultPosition = roulleteController->GetStartPoint(ballController->GetRadius());
    ballController->SetResetPosition(PXConverter::ConvertVector3(defaultPosition));
    ballController->Reset(false);
    ballController->EnableSimulation(false);

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
    roulleteController->Draw(camera);
    ballController->Draw(camera);

    skyboxShader->Use();
    skybox->Bind();

    skyboxShader->SetUniform("view", glm::mat4(glm::mat3(camera->ViewMatrix())));
    skyboxShader->SetUniform("projection", camera->ProjectionMatrix());

    {
        auto scale = std::sqrt(std::pow(camera->GetFarPlane(), 2) * 2) / (2 * std::sqrt(2));
        auto model = glm::scale(glm::mat4(1.f), glm::vec3{ scale, scale, scale });
        skyboxShader->SetUniform("model", model);
    }

    skybox->Draw(skyboxShader);
}

void App::OnUIUpdate() {
    float framerate = ImGui::GetIO().Framerate;
    std::string title = "Roullete Simulator. Framerate: " + std::to_string(framerate);
    glfwSetWindowTitle(renderWindow, title.c_str());
}

void App::DrawPhysicsDebugWorld(float delta) {
    
}

void App::SimulationStep(float dt) {
    const static float s_StepTime = 1.f / 60.f;
    static float s_Accumulated = 0.f;

    s_Accumulated += dt;
    if (s_Accumulated >= s_StepTime) {
        s_Accumulated -= dt;
        auto* scene = Physics::Instance().GetScene();
        scene->simulate(s_StepTime);
        scene->fetchResults(true);
    }
}

void App::StartNewRound() {
    state.SetState(StateContext::SimulatorState::IN_PROGRESS);
    ballController->EnableSimulation(true);
    auto position = roulleteController->GetStartPoint(ballController->GetRadius());
    ballController->ShootBall(position);
}