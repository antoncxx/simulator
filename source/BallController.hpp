#pragma once
#include "Graphics/ResourceManager.hpp"
#include "Graphics/UI.hpp"
#include "Physics/Physics.hpp"
#include "Graphics/Camera.hpp"


struct BallMaterial {
    float StaticFriction{ 0.4f };
    float DynamicFriction{ 0.6f };
    float Restitution{ 0.2f };
    float Density{ 3.f };
};

class BallController : public UIListener {
    std::shared_ptr<Model>  model{};
    std::shared_ptr<Shader> ballShader{};
    physx::PxRigidDynamic*  rigidBody{};
    physx::PxMaterial*      xMaterial{};
    BallMaterial            ballMaterial{};
    float                   ballSpeedValue{13.f};
    physx::PxVec3           resetPosition{0.f, 0.f, 0.f};
public:
    BallController();
    virtual ~BallController();

    void OnUIUpdate() override;
    void Update(float delta);
    void Draw(const std::shared_ptr<Camera>& viewCamera);
    void EnableSimulation(bool enable);
    void ShootBall(glm::vec3 from);
    void Reset(bool awake);

    inline void SetResetPosition(const physx::PxVec3& pos) noexcept { resetPosition = pos; }

    [[nodiscard]] inline float GetRadius() const noexcept { return 1.f; }
    [[nodiscard]] inline physx::PxTransform GetBallTransform() const { return rigidBody->getGlobalPose(); }

    static std::shared_ptr<BallController> Create();
private:
    void Initialize();
    void CreatePhysics();
    physx::PxMaterial* CreateMaterial();


};