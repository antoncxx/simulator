#include "Graphics/ResourceManager.hpp"
#include "Graphics/UI.hpp"
#include "Physics/Physics.hpp"


struct RotatorParametrs {
    float AngulatVelocity{ .2f };
    float CurrentAngle{ 0.f };
};

struct RoulleteMaterial {
    float StaticFriction{ 0.5f };
    float DynamicFriction{ 0.5f };
    float Restitution{ 0.6f };
};

enum class ModelProcessingFlag {
    STATIC_MODEL,
    DYNAMIC_MODEL,
};

class RoulleteController : public UIListener {
    std::shared_ptr<Model>  staticRoullete{};
    std::shared_ptr<Model>  dynamicRoullete{};

    RotatorParametrs rotatorParameter{};
    RoulleteMaterial roulleteMaterial{};
    physx::PxMaterial* xMaterial{nullptr};

    std::list<physx::PxRigidStatic*> rotators{};
public:
    RoulleteController() noexcept;
    virtual ~RoulleteController() noexcept;

    static std::shared_ptr<RoulleteController> Create();

    void OnUIUpdate() override;
    void Update(float delta);
    void Draw(const std::shared_ptr<Shader>& shader);
private:
    void Initialize();
    void CreatePhysics();

    void ProcessModel(const std::shared_ptr<Model>& model, ModelProcessingFlag flag);
    physx::PxMaterial* CreateMaterial();
};