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
    
    camera.reset(new Camera({ 0.f,35.f, 100.f }));

    camera->SetAspectRatio(1.75f);
    camera->SetFov(45.f);
    camera->SetPlanes(0.1f, 300.f);

    roulleteController = RoulleteController::Create();

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
    roulleteController->Update(delta);
    camera->Update(delta);
}

void App::DrawComponents(float delta) {
    shader->Use();
    shader->SetUniform("view", camera->ViewMatrix());
    shader->SetUniform("projection", camera->ProjectionMatrix());

    roulleteController->Draw(shader);
}

void App::OnUIUpdate() {
    float framerate = ImGui::GetIO().Framerate;
    std::string title = "Roullete Simulator. Framerate: " + std::to_string(framerate);
    glfwSetWindowTitle(renderWindow, title.c_str());
}

void App::DrawPhysicsDebugWorld(float delta) {
    const auto& renderBuffer = Physics::Instance().GetScene()->getRenderBuffer();

    std::vector<float> data;
    data.reserve(3 * static_cast<size_t>(renderBuffer.getNbLines()));

    const auto* lines = renderBuffer.getLines();
    for (size_t i = 0u; i < renderBuffer.getNbLines(); ++i) {
        data.push_back(lines[i].pos0.x);
        data.push_back(lines[i].pos0.y);
        data.push_back(lines[i].pos0.z);

        data.push_back(lines[i].pos1.x);
        data.push_back(lines[i].pos1.y);
        data.push_back(lines[i].pos1.z);
    }

    if (!data.empty()) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(10);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, data.data());
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_LINE_SMOOTH);
    }

}