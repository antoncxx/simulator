#include "Graphics/ResourceManager.hpp"
#include "Graphics/UI.hpp"
#include "Physics/Physics.hpp"
#include "Graphics/Camera.hpp"

#define TILT_AXIS {1.f, 0.f, 0.f}

struct RotatorParametrs {
    double AngulatVelocity{ 0.3 };
    double CurrentAngle{ 0. };
};

struct RoulleteMaterial {
    float StaticFriction{ 0.5f };
    float DynamicFriction{ 0.5f };
    float Restitution{ 0.2f };
};

enum class ModelProcessingFlag {
    STATIC_MODEL,
    DYNAMIC_MODEL,
};

class RoulleteController : public UIListener {
    std::shared_ptr<Model>  staticRoullete{};
    std::shared_ptr<Model>  dynamicRoullete{};
    std::shared_ptr<Shader> roulleteShader{};
    std::weak_ptr<Mesh>     pockets{};

    RotatorParametrs rotatorParameter{};
    RoulleteMaterial roulleteMaterial{};
    physx::PxMaterial* xMaterial{nullptr};

    std::list<physx::PxRigidDynamic*> rotators{};
public:
    RoulleteController() noexcept;
    virtual ~RoulleteController() noexcept;

    static std::shared_ptr<RoulleteController> Create();

    void OnUIUpdate() override;
    void Update(float delta);
    void Draw(const std::shared_ptr<Camera>& viewCamera);

    glm::vec3 GetStartPoint(float ballRadius);
    [[nodiscard]] int32_t GetPocket(physx::PxTransform position, float radius) const;
    [[nodiscard]] constexpr float GetTilt() const noexcept { return glm::radians(0.f); }
private:
    void Initialize();
    void CreatePhysics();

    void ProcessModel(const std::shared_ptr<Model>& model, ModelProcessingFlag flag);
    physx::PxMaterial* CreateMaterial();
};