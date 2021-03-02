#include "BallController.hpp"
#include "Converter.hpp"
#include <glm/gtc/matrix_transform.hpp>

BallController::BallController() {
    UI::Instance().RegisterListener(this);
}

BallController::~BallController() {
    UI::Instance().UnregisterListener(this);
}

void BallController::OnUIUpdate() {
    ImGui::Begin("Ball controller");
    if (ImGui::Button("Reset")) {
        Reset();
    }

    ImGui::Text("Material:");

    bool materialChanged{ false };
    materialChanged |= ImGui::SliderFloat("Static friction", &ballMaterial.StaticFriction, 0.f, 2.f);
    materialChanged |= ImGui::SliderFloat("Dynamic friction", &ballMaterial.DynamicFriction, 0.f, 2.f);
    materialChanged |= ImGui::SliderFloat("Restitution", &ballMaterial.Restitution, 0.f, 1.f);

    if (materialChanged) {
        xMaterial->setStaticFriction(ballMaterial.StaticFriction);
        xMaterial->setDynamicFriction(ballMaterial.DynamicFriction);
        xMaterial->setRestitution(ballMaterial.Restitution);
    }

    ImGui::SliderFloat("Ball speed value: ", &ballSpeedValue, -50.f, 50.f);
    ImGui::End();
}

void BallController::CreatePhysics() {
    using namespace physx;
    auto* scene   = Physics::Instance().GetScene();
    auto* physics = Physics::Instance().GetPhysics();

    xMaterial = CreateMaterial();
    rigidBody = PxCreateDynamic(*physics, PxTransform(33, 16, 20), PxSphereGeometry(GetRadius()), *xMaterial, ballMaterial.Density);
    
    scene->addActor(*rigidBody);
    Reset();
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

void BallController::Reset() {
    rigidBody->setLinearVelocity({ 0.f, 0.f, 0.f });
    rigidBody->setAngularVelocity({ 1.f, 1.f, 1.f });
    rigidBody->setGlobalPose(physx::PxTransform(0.f, 40.f, 0.f));
    rigidBody->clearForce();
    rigidBody->clearTorque();
}

void BallController::ShootBall(glm::vec3 from) {
    Reset();
    rigidBody->setLinearVelocity({ 0, 0, ballSpeedValue });
    rigidBody->setGlobalPose(physx::PxTransform(PXConverter::ConvertVector3(from)));
    // todo: rotate on random angle 
}

