#include "RoulleteController.hpp"
#include <glm/gtc/matrix_transform.hpp>

RoulleteController::RoulleteController() noexcept {
    UI::Instance().RegisterListener(this);
}

RoulleteController::~RoulleteController() noexcept {
    UI::Instance().UnregisterListener(this);
}

std::shared_ptr<RoulleteController> RoulleteController::Create() {
    std::shared_ptr instance = std::make_shared<RoulleteController>();
    instance->Initialize();
    return instance;
}

void RoulleteController::OnUIUpdate() {
    ImGui::Begin("Roullete Controller");
    ImGui::Text("3D Models:");
    dynamicRoullete->OnUIUpdate();
    staticRoullete->OnUIUpdate();
    ImGui::Text("Rotator parameters:");
    ImGui::SliderFloat("Angular velocity", &rotatorParameter.AngulatVelocity, -glm::pi<float>(), glm::pi<float>());
    ImGui::End();
}

void RoulleteController::Update(float delta) {
    rotatorParameter.CurrentAngle += rotatorParameter.AngulatVelocity * delta;
}

void RoulleteController::Initialize() {
    dynamicRoullete = ResourceManager::Instance().CreateModel("DynamicWheel", "Resources/Models/RoulleteDynamic.obj");
    staticRoullete  = ResourceManager::Instance().CreateModel("StaticWheel", "Resources/Models/RoulleteStatic.obj");
}

void RoulleteController::Draw(const std::shared_ptr<Shader>& shader) {
    glm::mat4 model = glm::mat4(1.f);
    shader->SetUniform("model", model);
    staticRoullete->Draw(shader);

    model = glm::rotate(glm::mat4(1.f), rotatorParameter.CurrentAngle, glm::vec3(0, 1, 0)); // @TODO: read world UP
    shader->SetUniform("model", model);
    dynamicRoullete->Draw(shader);
}