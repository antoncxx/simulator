#include "UI.hpp"

#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

UI& UI::Instance() {
    static UI instance;
    return instance;
}

void UI::Initialize(void* wnd) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)wnd, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::Finalize() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::RenderFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    for (auto* listener : listeners) {
        listener->OnUIUpdate();
    }
    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void UI::RegisterListener(UIListener* listener) {
    auto iter = std::find(listeners.begin(), listeners.end(), listener);
    if (iter == listeners.end()) {
        listeners.push_back(listener);
    }
}

void UI::UnregisterListener(UIListener* listener) {
    auto iter = std::find(listeners.begin(), listeners.end(), listener);
    if (iter != listeners.end()) {
        listeners.erase(iter);
    }
}