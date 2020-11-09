#include "ResourceManager.hpp"

#include <fstream>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace {
    std::string ReadFileContent(const std::filesystem::path& path) {
        std::ifstream file(path);
        assert(file.is_open());

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        return content;
    }
}

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

[[nodiscard]] std::shared_ptr<Shader>
ResourceManager::CreateShader(
    const std::string& name, 
    const std::filesystem::path& vertexSource, 
    const std::filesystem::path& fragmentSource) {

    auto vertexShaderSourceCode   = ReadFileContent(vertexSource);
    auto fragmentShaderSourceCode = ReadFileContent(fragmentSource);

    auto instance = std::make_shared<Shader>();
    instance->Compile(vertexShaderSourceCode, fragmentShaderSourceCode);

    storage.emplace(name, instance);
    return instance;
}

[[nodiscard]] std::shared_ptr<Model> 
ResourceManager::CreateModel(const std::string& name,
    const std::filesystem::path& modelSource) {
    Assimp::Importer importer;
    const auto* scene = importer.ReadFile(modelSource.string(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }
    
    std::shared_ptr instance = std::make_shared<Model>(name);
    instance->InitalizeFromScene(scene);
    
    storage.emplace(name, instance);
    return instance;
}

[[nodiscard]] std::optional<ResourceManager::ResourcePtr>
ResourceManager::GetResource(const std::string& name) {
    if (auto iter = storage.find(name); iter != storage.end()) {
        return { iter->second };
    } else {
        return std::nullopt;
    }
}

void ResourceManager::RemoveResource(const std::string& name) {
    if (auto iter = storage.find(name); iter != storage.end()) {
        iter->second->Cleanup();
        storage.erase(iter);
    }
}

void ResourceManager::Cleanup() {
    for (const auto& [name, resource] : storage) {
        resource->Cleanup();
    }

    storage.clear();
}
