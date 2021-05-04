#include "BallController.hpp"
#include "Converter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "State.hpp"

BallController::BallController() {
    UI::Instance().RegisterListener(this);
}

BallController::~BallController() {
    UI::Instance().UnregisterListener(this);
}

void BallController::OnUIUpdate() {
#if 1

    ImGui::Begin("Ball controller");
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
#endif // DEBUG
}

void BallController::CreatePhysics() {
    using namespace physx;
    auto* scene   = Physics::Instance().GetScene();
    auto* physics = Physics::Instance().GetPhysics();

    xMaterial = CreateMaterial();
    rigidBody = PxCreateDynamic(
        *physics, 
        PxTransform(physx::PxIDENTITY::PxIdentity), 
        PxSphereGeometry(GetRadius()), 
        *xMaterial, 
        ballMaterial.Density
    );
    
    scene->addActor(*rigidBody);
}

std::shared_ptr<BallController> BallController::Create() {
    std::shared_ptr instance = std::make_shared<BallController>();
    instance->Initialize();
    instance->CreatePhysics();
    return instance;
}

void BallController::Draw(const std::shared_ptr<Camera>& viewCamera) {
    ballShader->Use();
    ballShader->SetUniform("view",       viewCamera->ViewMatrix());
    ballShader->SetUniform("projection", viewCamera->ProjectionMatrix());

    auto transform = rigidBody->getGlobalPose();

    auto modelTransform = glm::mat4(1.f);
    modelTransform = glm::translate(modelTransform, PXConverter::ConvertVector3(transform.p));
    modelTransform = glm::rotate(
        modelTransform, 
        transform.q.getAngle(), 
        PXConverter::ConvertVector3(transform.q.getImaginaryPart())
    );

    ballShader->SetUniform("model", modelTransform);
    model->Draw(ballShader);
}

void BallController::Initialize() {
    model      = ResourceManager::Instance().CreateModel("Ball", "Resources/Models/Sphere.obj");
    ballShader = ResourceManager::Instance().CreateShader(
        "BallShader", 
        "Resources/Shaders/Ball.vs", 
        "Resources/Shaders/Ball.fs"
    );
}

physx::PxMaterial* BallController::CreateMaterial() {
    auto* physics = Physics::Instance().GetPhysics();
    return physics->createMaterial(
        ballMaterial.StaticFriction, 
        ballMaterial.DynamicFriction, 
        ballMaterial.Restitution
    );
}

void BallController::Update(float delta) {

}

void BallController::Reset(bool awake) {
    rigidBody->setLinearVelocity({ 0.f, 0.f, 0.f });
    rigidBody->setAngularVelocity({ 0.f, 0.f, 0.f });
    rigidBody->clearForce();
    rigidBody->clearTorque();
    rigidBody->setGlobalPose(physx::PxTransform(resetPosition), awake);
}

void BallController::ShootBall(const glm::vec3& from, const glm::vec3& tilt, const float tiltAngle) {
    Reset(true);

    glm::vec3 velocity = glm::rotate({ 0, 0, ballSpeedValue }, tiltAngle, tilt);

    rigidBody->setLinearVelocity(PXConverter::ConvertVector3(velocity)); // todo: randomize
    rigidBody->setGlobalPose(physx::PxTransform(PXConverter::ConvertVector3(from)));
}

void BallController::EnableSimulation(bool enable) {
    rigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !enable);

}


