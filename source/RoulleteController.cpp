#include "RoulleteController.hpp"
#include "Converter.hpp"
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
    instance->CreatePhysics();
    return instance;
}

void RoulleteController::OnUIUpdate() {
    ImGui::Begin("Roullete Controller");
    ImGui::Text("3D Models:");
    dynamicRoullete->OnUIUpdate();
    staticRoullete->OnUIUpdate();
    ImGui::Text("Rotator parameters:");
    ImGui::SliderFloat("Angular velocity", &rotatorParameter.AngulatVelocity, -glm::pi<float>(), glm::pi<float>());
    ImGui::Text("Material:");

    bool materialChanged{ false };
    materialChanged |= ImGui::SliderFloat("Static friction", &roulleteMaterial.StaticFriction, 0.f, 2.f);
    materialChanged |= ImGui::SliderFloat("Dynamic friction", &roulleteMaterial.DynamicFriction, 0.f, 2.f);
    materialChanged |= ImGui::SliderFloat("Restitution", &roulleteMaterial.Restitution, 0.f, 1.f);

    if (materialChanged) {
        xMaterial->setStaticFriction(roulleteMaterial.StaticFriction);
        xMaterial->setDynamicFriction(roulleteMaterial.DynamicFriction);
        xMaterial->setRestitution(roulleteMaterial.Restitution);
    }

    ImGui::End();
}

void RoulleteController::Update(float delta) {
    using namespace physx;

    rotatorParameter.CurrentAngle += rotatorParameter.AngulatVelocity * delta;

    PxQuat quaternion(rotatorParameter.CurrentAngle, { 0.f, 1.f, 0.f });
    auto transform = PxTransform(0.f, 0.f, 0.f, quaternion);

    for (auto* mesh : rotators) {
        if (mesh != nullptr) {
            mesh->setGlobalPose(transform);
        }
    }
}

void RoulleteController::Initialize() {
    dynamicRoullete = ResourceManager::Instance().CreateModel("DynamicWheel", "Resources/Models/RoulleteDynamic.obj");
    staticRoullete = ResourceManager::Instance().CreateModel("StaticWheel", "Resources/Models/RoulleteStatic.obj");
}

void RoulleteController::Draw(const std::shared_ptr<Shader>& shader) {
    glm::mat4 model = glm::mat4(1.f);
    shader->SetUniform("model", model);
    staticRoullete->Draw(shader);

    model = glm::rotate(glm::mat4(1.f), rotatorParameter.CurrentAngle, glm::vec3(0, 1, 0)); // @TODO: read world UP
    shader->SetUniform("model", model);
    dynamicRoullete->Draw(shader);
}

void RoulleteController::CreatePhysics() {
    using namespace physx;

    xMaterial = CreateMaterial();

    ProcessModel(staticRoullete, ModelProcessingFlag::STATIC_MODEL);
    ProcessModel(dynamicRoullete, ModelProcessingFlag::DYNAMIC_MODEL);
}

void RoulleteController::ProcessModel(const std::shared_ptr<Model>& model, ModelProcessingFlag flag) {
    using namespace physx;

    auto* physics = Physics::Instance().GetPhysics();
    auto* cooking = Physics::Instance().GetCooking();
    auto* scene   = Physics::Instance().GetScene();

    for (auto& mesh : model->GetMeshes()) {
        auto* xmesh = PXConverter::ConvertMesh(mesh, physics, cooking);

        PxTriangleMeshGeometry geometry(xmesh);

        auto* rigidStatic = physics->createRigidStatic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
        auto* shape = physics->createShape(geometry, *xMaterial);
        rigidStatic->attachShape(*shape);

        if (flag == ModelProcessingFlag::DYNAMIC_MODEL) {
            rotators.push_back(rigidStatic);
        }

        scene->addActor(*rigidStatic);
    }

}

physx::PxMaterial* RoulleteController::CreateMaterial() {
    auto* physics = Physics::Instance().GetPhysics();
    return physics->createMaterial(roulleteMaterial.StaticFriction, roulleteMaterial.DynamicFriction, roulleteMaterial.Restitution);
}

glm::vec3 RoulleteController::GetStartPoint(float ballRadius) {
    auto outerWheel = staticRoullete->GetMeshByName("OuterWheelBottom");
    auto innerWheel = dynamicRoullete->GetMeshByName("WheelDynamic");

    if (!outerWheel.has_value() || !innerWheel.has_value()) {
        throw std::runtime_error("Failed to get model by name");
    }

    auto outerBox = outerWheel.value().GetBoundingBox();
    auto innerBox = innerWheel.value().GetBoundingBox();

    auto innerSize   = innerBox.GetSize();
    auto innerCenter = innerBox.GetCenterPoint();

    auto outerSize   = outerBox.GetSize();
    auto outerCenter = outerBox.GetCenterPoint();

    auto xi = innerCenter.x + innerSize.x / 2;
    auto yi = innerCenter.y + innerSize.y / 2;

    auto xo = outerCenter.x + outerSize.x / 2;
    auto yo = outerCenter.y + outerSize.y / 2;

    auto angle = std::atan((yo - yi) / (xo - xi));

    auto xstart = xo - ballRadius;
    auto ystart = yo + ballRadius * (1 - std::tan(angle));
    auto zstart = outerCenter.z;

    return { xstart, ystart, zstart };
}