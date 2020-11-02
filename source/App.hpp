#pragma once

#include "Graphics/BaseGraphicsApplication.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Graphics/InputHandler.hpp"
#include "Graphics/Camera.hpp"

class App final : public BaseGraphicsApplication {
private:
    std::shared_ptr<Shader> sdummy{};
    std::shared_ptr<Model>  dummy1{};
    std::shared_ptr<Model>  dummy2{};

    std::shared_ptr<Camera> camera;
public:
    explicit App() = default;
    virtual ~App() = default;

    void OnEveryFrame(float delta) override;
    void Initialize() override;
    void Finalize() override;
};