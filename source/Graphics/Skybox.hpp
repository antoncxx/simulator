#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

#include "GraphicalResource.hpp"
#include "Shader.hpp"

class ResourceManager;

class Skybox : public GraphicalResource {
    uint32_t id{};
    uint32_t vao{}, vbo{};
public:
    void Cleanup() override;
    void Bind() const;
    void Draw(const std::shared_ptr<Shader>& shader) const;
private:
    static std::shared_ptr<Skybox> Create(const std::vector<std::filesystem::path>& faces);
    
    friend ResourceManager;
};