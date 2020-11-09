#pragma once

#include "Graphics/BaseGraphicsApplication.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Graphics/InputHandler.hpp"
#include "Graphics/Camera.hpp"
#include "RoulleteController.hpp"
#include "Physics/Physics.hpp"

class App final : public BaseGraphicsApplication, public UIListener {
private:
    std::shared_ptr<Shader> shader{};
    std::shared_ptr<Camera> camera;
    std::shared_ptr<RoulleteController> roulleteController{};

public:
    explicit App() = default;
    virtual ~App() = default;

    void OnEveryFrame(float delta) override;
    void Initialize() override;
    void Finalize() override;

    void OnUIUpdate() override;

private:
    void UpdateViewPort();
    void UpdateComponets(float delta);
    void DrawComponents(float delta);
    void DrawPhysicsDebugWorld(float delta);
};