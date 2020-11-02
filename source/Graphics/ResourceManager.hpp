#pragma once

#include <unordered_map>
#include <memory>
#include <filesystem>
#include <optional>

#include "Shader.hpp"
#include "Model.hpp"

class ResourceManager {
    using ResourcePtr = std::shared_ptr<GraphicalResource>;
private:
    std::unordered_map<std::string, std::shared_ptr<GraphicalResource>> storage;
public:
    static ResourceManager& Instance();

    [[nodiscard]] std::shared_ptr<Shader> CreateShader(const std::string& name, 
        const std::filesystem::path& vertexSource, 
        const std::filesystem::path& fragmentSource);


    [[nodiscard]] std::shared_ptr<Model> CreateModel(const std::string& name,
        const std::filesystem::path& modelSource);

    [[nodiscard]] std::optional<ResourcePtr> GetResource(const std::string& name);
    void RemoveResource(const std::string& name);
    void Cleanup();   
private:
    ResourceManager() = default;
};