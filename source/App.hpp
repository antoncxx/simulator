#pragma once

#include "Graphics/BaseGraphicsApplication.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Graphics/InputHandler.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Skybox.hpp"
#include "RoulleteController.hpp"
#include "BallController.hpp"
#include "Physics/Physics.hpp"
#include "State.hpp"

class App final : public BaseGraphicsApplication, public UIListener {
private:
    std::shared_ptr<Camera>             camera;
    std::shared_ptr<RoulleteController> roulleteController{};
    std::shared_ptr<BallController>     ballController{};
    std::shared_ptr<Skybox>             skybox{ };
    std::shared_ptr<Shader>             skyboxShader{};

    StateContext state{};

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
    void SimulationStep(float dt);
    void StartNewRound(bool mathTest);

    void MathTest(uint32_t gamesNumber);
};