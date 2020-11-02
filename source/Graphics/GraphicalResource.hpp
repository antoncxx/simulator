#pragma once
#include <string>

class GraphicalResource {

public:
    GraphicalResource(const GraphicalResource&) = delete;
    GraphicalResource& operator=(const GraphicalResource&) = delete;

    GraphicalResource() = default;
    ~GraphicalResource() = default;

    virtual void Cleanup() = 0;
};