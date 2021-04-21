#include "RoulleteController.hpp"
#include "Converter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "State.hpp"

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
#ifdef DEBUG
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
#endif // RELEASE
}

void RoulleteController::Update(float delta) {
    using namespace physx;

    rotatorParameter.CurrentAngle += rotatorParameter.AngulatVelocity * delta;
    
    glm::mat4 model = glm::rotate(glm::mat4(1.f), GetTilt(), TILT_AXIS);
    model = glm::rotate(model, rotatorParameter.CurrentAngle, glm::vec3(0, 1, 0));
    glm::quat q = model;
    PxTransform transform(PxQuat(q.x, q.y, q.z, q.w));
    for (auto* mesh : rotators) {
        if (mesh != nullptr) {
            mesh->setKinematicTarget(transform);      
            // !Incorrect way: 
            //mesh->setGlobalPose(transform);
        }
    }
}

void RoulleteController::Initialize() {
    dynamicRoullete = ResourceManager::Instance().CreateModel("DynamicWheel", "Resources/Models/RoulleteDynamic.obj");
    staticRoullete = ResourceManager::Instance().CreateModel("StaticWheel", "Resources/Models/RoulleteStatic.obj");
    roulleteShader = ResourceManager::Instance().CreateShader("DefaultShader", "Resources/Shaders/Default.vs", "Resources/Shaders/Default.fs");

    pockets = dynamicRoullete->GetMeshByName("WheelDividers").value();
}

void RoulleteController::Draw(const std::shared_ptr<Camera>& viewCamera) {
    roulleteShader->Use();

    roulleteShader->SetUniform("view",       viewCamera->ViewMatrix());
    roulleteShader->SetUniform("cameraPos",  viewCamera->GetCameraPosition());
    roulleteShader->SetUniform("projection", viewCamera->ProjectionMatrix());
    
    glm::mat4 model = glm::rotate(glm::mat4(1.f), GetTilt(), TILT_AXIS);
    roulleteShader->SetUniform("model", model);
    staticRoullete->Draw(roulleteShader);

    model = glm::rotate(model, rotatorParameter.CurrentAngle, glm::vec3(0, 1, 0)); // @TODO: read world UP
    
    
    roulleteShader->SetUniform("model", model);
    dynamicRoullete->Draw(roulleteShader);
}

void RoulleteController::CreatePhysics() {
    using namespace physx;

    xMaterial = CreateMaterial();

    ProcessModel(staticRoullete,  ModelProcessingFlag::STATIC_MODEL);
    ProcessModel(dynamicRoullete, ModelProcessingFlag::DYNAMIC_MODEL);
}

void RoulleteController::ProcessModel(const std::shared_ptr<Model>& model, ModelProcessingFlag flag) {
    using namespace physx;

    auto* physics = Physics::Instance().GetPhysics();
    auto* cooking = Physics::Instance().GetCooking();
    auto* scene   = Physics::Instance().GetScene();

    for (auto& mesh : model->GetMeshes()) {
        auto* xmesh = PXConverter::ConvertMesh(mesh, physics, cooking);

        auto* actor = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
        actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        PxTriangleMeshGeometry geometry;
        geometry.triangleMesh = xmesh;

        auto* shape = physx::PxRigidActorExt::createExclusiveShape(*actor, geometry, *xMaterial);
        scene->addActor(*actor);

        PxQuat quaternion = PxQuat(GetTilt(), TILT_AXIS);
        actor->setGlobalPose(PxTransform(quaternion));

        if (flag == ModelProcessingFlag::DYNAMIC_MODEL) {
            rotators.push_back(actor);
        } 
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

    auto outerBox = outerWheel.value()->GetBoundingBox();
    auto innerBox = innerWheel.value()->GetBoundingBox();

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

    return glm::rotate(glm::vec3{ xstart, ystart, zstart }, GetTilt(), TILT_AXIS);
}


int32_t RoulleteController::GetPocket(physx::PxTransform position, float radius) const {
    constexpr static size_t pocketsNumber = 38;
    constexpr static float dAngle = glm::two_pi<float>() / pocketsNumber;

    if (auto ptr = pockets.lock()) {
        const auto& box = ptr->GetBoundingBox();
        const auto& ballPos = PXConverter::ConvertVector3(position.p);
        const auto& boxPos = box.GetCenterPoint();

        if (glm::length(ballPos - boxPos) <= (box.GetSize().x / 2 - radius)) {
            glm::vec2 v1 = { 1.f, 0 };
            glm::vec2 v2{ ballPos.x, ballPos.z };

            float angle = glm::orientedAngle(glm::normalize(v1), glm::normalize(v2));
            angle += rotatorParameter.CurrentAngle;
            angle = glm::mod(angle, glm::two_pi<float>());

            int32_t offsetIndex = static_cast<int>(angle / dAngle + 0.5);
            return (pocketsNumber + offsetIndex) % pocketsNumber;
        }
    }

    return -1;
}
