#pragma once

#include "../ImGui/imgui.h"


#include <list>


class UIListener {
public:
    UIListener() = default;
    virtual ~UIListener() = default;

    virtual void OnUIUpdate() = 0;
};

class UI {
    explicit UI() = default;
    std::list<UIListener*> listeners;
public:
    static UI& Instance();

    void Initialize(void* wnd);
    void Finalize();

    void RenderFrame();

    void RegisterListener(UIListener* listener);
    void UnregisterListener(UIListener* listener);
};