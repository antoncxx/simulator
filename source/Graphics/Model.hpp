#pragma once

#include "GraphicalResource.hpp"
#include "Mesh.hpp"
#include "UI.hpp"

#include <assimp/scene.h>

class ResourceManager;

class Model final : public GraphicalResource, public UIListener {
    std::vector<Mesh> meshes;
    std::string name;

    bool visible{ true };
public:

    explicit Model(const std::string& name);
    virtual ~Model() = default;

    void Draw(const std::shared_ptr<Shader>& shader) const;
    void Cleanup() override;

    bool IsVisible() const noexcept { return visible; }

    void OnUIUpdate() override;

    auto GetMeshes() const noexcept { return meshes; }
private:
    void InitalizeFromScene(const aiScene* scene);
    void ProcessNode(const aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);

    friend ResourceManager;

};