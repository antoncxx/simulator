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
        // todo
    }

    ImGui::End();
}

void RoulleteController::Update(float delta) {
    rotatorParameter.CurrentAngle += rotatorParameter.AngulatVelocity * delta;

    for (auto* mesh : rotators) {
        if (mesh != nullptr) {
            UpdateFloatableMesh(mesh);
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
        auto* xmesh = ConvertMesh(mesh);

        PxTriangleMeshGeometry geometry(xmesh);

        auto* rigidStatic = physics->createRigidStatic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
        auto* shape = physics->createShape(geometry, *xMaterial);
        
        shape->setContactOffset(0.01f);
        shape->setRestOffset(-0.01f);
        rigidStatic->attachShape(*shape);
        scene->addActor(*rigidStatic);
    }

}

physx::PxTriangleMesh* RoulleteController::ConvertMesh(const Mesh& mesh) {
    using namespace physx;

    auto* physics = Physics::Instance().GetPhysics();
    auto* cooking = Physics::Instance().GetCooking();

    auto vertices = mesh.GetVertices();
    auto indicies = mesh.GetIndices();

    PxTriangleMeshDesc description;


    description.points.data   = vertices.data();
    description.points.count  = vertices.size();
    description.points.stride = sizeof(Vertex);

    description.triangles.count  = PxU32(indicies.size() / 3);
    description.triangles.data   = indicies.data();
    description.triangles.stride = 3 * sizeof(uint32_t);

    if (!description.isValid()) {
        // todo
    }

    PxDefaultMemoryOutputStream       writeBuffer{};
    PxTriangleMeshCookingResult::Enum result{};

    if (!cooking->cookTriangleMesh(description, writeBuffer, &result)) {
        // todo
    }

    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    return physics->createTriangleMesh(readBuffer);
}

physx::PxMaterial* RoulleteController::CreateMaterial() {
    auto* physics = Physics::Instance().GetPhysics();
    return physics->createMaterial(roulleteMaterial.StaticFriction, roulleteMaterial.DynamicFriction, roulleteMaterial.Restitution);
}

void RoulleteController::UpdateFloatableMesh(physx::PxRigidStatic* rigid) {
    auto transform = rigid->getGlobalPose();
    transform.rotate({ 0.f, glm::radians<float>(rotatorParameter.CurrentAngle), 0.f });
    rigid->setGlobalPose(transform);
}