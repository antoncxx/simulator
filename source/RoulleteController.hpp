#include "Graphics/ResourceManager.hpp"
#include "Graphics/UI.hpp"

struct RotatorParametrs {
    float AngulatVelocity{ 1.f };
    float CurrentAngle{ 0.f };
};

class RoulleteController : public UIListener {
    std::shared_ptr<Model>  staticRoullete{};
    std::shared_ptr<Model>  dynamicRoullete{};

    RotatorParametrs rotatorParameter{};
public:
    RoulleteController() noexcept;
    virtual ~RoulleteController() noexcept;

    static std::shared_ptr<RoulleteController> Create();

    void OnUIUpdate() override;
    void Update(float delta);
    void Draw(const std::shared_ptr<Shader>& shader);
private:
    void Initialize();
};