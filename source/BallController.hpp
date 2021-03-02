#pragma once
#include "Graphics/ResourceManager.hpp"
#include "Graphics/UI.hpp"
#include "Physics/Physics.hpp"

struct BallMaterial {
    float StaticFriction{ 0.1f };
    float DynamicFriction{ 0.1f };
    float Restitution{ 0.1f };
    float Density{ 3.f };
};

class BallController : public UIListener {
    std::shared_ptr<Model>  model{};
    physx::PxRigidDynamic*  rigidBody{};
    physx::PxMaterial*      xMaterial{};
    BallMaterial            ballMaterial{};
    float                   ballSpeedValue{13.f};
public:
    BallController();
    virtual ~BallController();

    void OnUIUpdate() override;
    void Update(float delta);
    void Draw(const std::shared_ptr<Shader>& shader);

    void ShootBall(glm::vec3 from);
    [[nodiscrard]] inline float GetRadius() const noexcept { return 1.f; }
    static std::shared_ptr<BallController> Create();
private:
    void Initialize();
    void CreatePhysics();
    physx::PxMaterial* CreateMaterial();

    void Reset();

};