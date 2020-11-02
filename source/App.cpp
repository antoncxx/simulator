#include "App.hpp"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void App::Initialize() {
    BaseGraphicsApplication::Initialize();
    dummy1 = ResourceManager::Instance().CreateModel("StaticWheel", "Resources/Models/RoulleteStatic.obj");
    dummy2 = ResourceManager::Instance().CreateModel("DynamicWheel", "Resources/Models/RoulleteDynamic.obj");

    sdummy = ResourceManager::Instance().CreateShader("DefaultShader", "Resources/Shaders/Default.vs", "Resources/Shaders/Default.fs");
    
    camera.reset(new Camera({ 0.f,35.f, 100.f }));

    camera->SetAspectRatio(1.75);
    camera->SetFov(45);
    camera->SetPlanes(0.1, 300);

    InputHandler::Instance().Setup(renderWindow);
}

void App::Finalize() {
    ResourceManager::Instance().Cleanup();
    BaseGraphicsApplication::Finalize();
}

void App::OnEveryFrame(float delta) {
    camera->Update(delta);


    sdummy->Use();

    glm::mat4 model = glm::rotate(glm::mat4(1.f), (float)glfwGetTime(), glm::vec3(0,1,0));
    
    sdummy->SetUniform("model", model);
    sdummy->SetUniform("view", camera->ViewMatrix());
    sdummy->SetUniform("projection", camera->ProjectionMatrix());

    dummy2->Draw(*sdummy);

    sdummy->SetUniform("model", glm::mat4(1.f));

    dummy1->Draw(*sdummy);

}