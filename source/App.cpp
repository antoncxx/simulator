#include "App.hpp"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Converter.hpp"
#include <fstream>


void App::Initialize() {
    BaseGraphicsApplication::Initialize();
    InputHandler::Instance().Setup(renderWindow);
    UI::Instance().RegisterListener(this);
    Physics::Instance().StartUp();

    skyboxShader = ResourceManager::Instance().CreateShader("SkyboxShader", "Resources/Shaders/Skybox.vs", "Resources/Shaders/Skybox.fs");
    
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
    camera->SetPlanes(0.1f, 1000.f);

    roulleteController = RoulleteController::Create();
    ballController     = BallController::Create();

    InputHandler::Instance().RegisterKeyboardCallback(KeyboardButton::N, std::bind(&App::StartNewRound, this, false));
    InputHandler::Instance().RegisterKeyboardCallback(KeyboardButton::M, [this]() {
        if (!state.IsMathTest()) {
            MathTest(1'000'000);
        }
    });
    
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

    if (!state.IsMathTest()) {
        UpdateComponets(delta);
        DrawComponents(delta);
    }


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
    const static float s_StepTime = 1.f / 60.f;
    static float s_Accumulated = 0.f;

    s_Accumulated += delta;

    if (s_Accumulated >= s_StepTime) {
        s_Accumulated -= s_StepTime;
        camera->Update(s_StepTime);
        ballController->Update(s_StepTime);
        roulleteController->Update(s_StepTime);
        SimulationStep(s_StepTime);
    }
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

void App::SimulationStep(float dt) {
    auto* scene = Physics::Instance().GetScene();
    scene->simulate(dt);
    scene->fetchResults(true);
}

void App::StartNewRound(bool mathTest) {
    if (!mathTest) {
        state.SetState(StateContext::SimulatorState::IN_PROGRESS);
    }

    ballController->EnableSimulation(true);
    auto position = roulleteController->GetStartPoint(ballController->GetRadius());
    ballController->ShootBall(position, TILT_AXIS, roulleteController->GetTilt());
}

// TODO: refactor function and add additional end round criterias
void App::MathTest(uint32_t gamesNumber) {
    glfwHideWindow(renderWindow);

    state.SetState(StateContext::SimulatorState::MATH_TEST);
    std::cout << "MATH TEST: Start.\n\n";

    std::array<int32_t, 38> result;
    std::fill(result.begin(), result.end(), 0);

    const float dt = 1.f / 60.f;

    int32_t failed{0};
    for (uint32_t i = 0u; i < gamesNumber; ++i) {
        std::cout << "MATH TEST: New round ID=" << i << "\n";
        StartNewRound(true);

        int32_t roundResult = {-1};
        float accumulatedTime = 0.f;
        float total = 0.f;

        while (true) {
            total += dt;

            ballController->Update(dt);
            roulleteController->Update(dt);
            SimulationStep(dt);

            int32_t currentPocket = roulleteController->GetPocket(ballController->GetBallTransform(), ballController->GetRadius());
 
            if (currentPocket == roundResult && currentPocket != -1) {
                accumulatedTime += dt;

                if (roundResult != -1 && accumulatedTime > 10.f) {
                    result[roundResult]++;
                    std::cout << "MATH TEST: Round finished in pocket: " << roundResult << ".{Failed: " << failed << "}\n\n";
                    break;
                }

                continue;
            } 

            roundResult = currentPocket;
            accumulatedTime = 0.f;


            if (ballController->GetBallTransform().p.y < 0) {
                std::cerr << "MATH TEST: Round failed as ball escaped the wheel\n\n";
                break;
            }

            if (ballController->IsSleeping()) {
                std::cerr << "MATH TEST: Round failed as ball fall asleep\n\n";
                break;
            }

            if (total >= 300.f) {
                ++failed;
                std::cerr << "MATH TEST: Timeout exceeded\n\n";
                break;
            }
        }

        glfwPollEvents();
    }

    std::cout << "MATH TEST: Finish. Failed " << failed <<"\n\n";
    state.SetState(StateContext::SimulatorState::IDLE);

    std::ofstream file("MathTest.log");

    for (const auto r : result) {
        file << r << " ";
    }

    file.close();
    glfwShowWindow(renderWindow);
}