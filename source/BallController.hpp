#pragma once
#include "Graphics/ResourceManager.hpp"
#include "Graphics/UI.hpp"
#include "Physics/Physics.hpp"

struct BallMaterial {
    float StaticFriction{ 0.1f };
    float DynamicFriction{ 0.1f };
    float Restitution{ 0.6f };
};

class BallController : public UIListener {
    std::shared_ptr<Model>  model{};
    physx::PxRigidDynamic*  rigidBody{};
    physx::PxMaterial*      xMaterial{};
    BallMaterial            ballMaterial{};
public:
    BallController();
    virtual ~BallController();

    void OnUIUpdate() override;
    void Update(float delta);
    void Draw(const std::shared_ptr<Shader>& shader);

    static std::shared_ptr<BallController> Create();
private:
    void Initialize();
    void CreatePhysics();
    physx::PxMaterial* CreateMaterial();

    void Reset();

};