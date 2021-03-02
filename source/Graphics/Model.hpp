#pragma once

#include "GraphicalResource.hpp"
#include "Mesh.hpp"
#include "UI.hpp"

#include <optional>
#include <assimp/scene.h>

class ResourceManager;


class Model final : public GraphicalResource, public UIListener {
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string name;

    bool visible{ true };
public:

    explicit Model(const std::string& name);
    virtual ~Model() = default;

    void Draw(const std::shared_ptr<Shader>& shader) const;
    void Cleanup() override;

    [[nodiscard]] bool IsVisible() const noexcept { return visible; }

    void OnUIUpdate() override;

    [[nodiscard]] auto GetMeshes() const noexcept { return meshes; }
    [[nodiscard]] std::optional<std::shared_ptr<Mesh>> GetMeshByName(const std::string& name) const;
private:
    void InitalizeFromScene(const aiScene* scene);
    [[nodiscard]]  void                  ProcessNode(const aiNode* node, const aiScene* scene);
    [[nodiscard]]  std::shared_ptr<Mesh> ProcessMesh(const aiMesh* mesh, const aiScene* scene);

    friend ResourceManager;

};