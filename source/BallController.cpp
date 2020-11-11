#include "BallController.hpp"
#include <glm/gtc/matrix_transform.hpp>


BallController::BallController() {
    UI::Instance().RegisterListener(this);
}

BallController::~BallController() {
    UI::Instance().UnregisterListener(this);
}

void BallController::OnUIUpdate() {
    ImGui::Begin("Ball controller");
    ImGui::End();
}

void BallController::CreatePhysics() {
    using namespace physx;
    auto* scene   = Physics::Instance().GetScene();
    auto* physics = Physics::Instance().GetPhysics();

    auto* material = CreateMaterial();

    rigidBody = PxCreateDynamic(*physics, PxTransform(3, 50, 3), PxSphereGeometry(1.f), *material, 3.f);
    scene->addActor(*rigidBody);
}

std::shared_ptr<BallController> BallController::Create() {
    std::shared_ptr instance = std::make_shared<BallController>();
    instance->Initialize();
    instance->CreatePhysics();
    return instance;
}

void BallController::Draw(const std::shared_ptr<Shader>& shader) {
    shader->Use();

    auto pos = rigidBody->getGlobalPose();
    glm::vec3 position = { pos.p.x, pos.p.y, pos.p.z };
    shader->SetUniform("model", glm::translate(glm::mat4(1.f), position));

    model->Draw(shader);
}

void BallController::Initialize() {
    model = ResourceManager::Instance().CreateModel("Ball", "Resources/Models/Sphere.obj");

}

physx::PxMaterial* BallController::CreateMaterial() {
    auto* physics = Physics::Instance().GetPhysics();
    return physics->createMaterial(ballMaterial.StaticFriction, ballMaterial.DynamicFriction, ballMaterial.Restitution);
}

void BallController::Update(float delta) {

}

