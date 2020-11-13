#include "Model.hpp"
#include "ResourceManager.hpp"
#include <GL/glew.h>

namespace {
    inline glm::vec3 ToGLMVector3(const aiVector3D& vector) noexcept {
        return { vector.x, vector.y, vector.z };
    }

    inline glm::vec2 ToGLMVector2(const aiVector2D& vector) noexcept {
        return { vector.x, vector.y };
    }

    void ProcessMeshVerticies(const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
        for (size_t i = 0u; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            vertex.Position = ToGLMVector3(mesh->mVertices[i]);

            if (mesh->HasNormals()) {
                vertex.Normal = ToGLMVector3(mesh->mNormals[i]);
            }

            if (mesh->HasTextureCoords(0)) {
                vertex.TextureCoordinate = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            }

            vertices.emplace_back(std::move(vertex));
        }

        for (size_t i = 0u; i < mesh->mNumFaces; ++i) {
            const auto aiFace = mesh->mFaces[i];

            for (size_t j = 0u; j < aiFace.mNumIndices; ++j) {
                indices.emplace_back(aiFace.mIndices[j]);
            }
        }
    }

    void ProcessMeshTexture(const aiMesh* mesh, const aiMaterial* material, std::shared_ptr<Texture>& texture) {
        uint32_t type{ 0 };
        for (type = aiTextureType::aiTextureType_NONE; type < aiTextureType::aiTextureType_UNKNOWN; ++type) {
            if (material->GetTextureCount(static_cast<aiTextureType>(type)) > 0) {
                break;
            }
        }

        if (type != aiTextureType::aiTextureType_UNKNOWN) {
            aiString path{};
            material->GetTexture(static_cast<aiTextureType>(type), 0, &path);
            std::filesystem::path texturePath = path.C_Str();
            std::string name = texturePath.filename().string();

            if (auto resource = ResourceManager::Instance().GetResource(name); resource.has_value()) {
                texture = std::dynamic_pointer_cast<Texture>(resource.value());
            }
            else {
                texture = ResourceManager::Instance().CreateTexture(name, texturePath);
            }
        }
    }
}

Model::Model(const std::string& name) : name(name) {
}

void Model::Draw(const std::shared_ptr<Shader>& shader) const {
    if (!IsVisible()) {
        return;
    }

    shader->Use();

    for (const auto& mesh : meshes) {
        if (mesh.IsVisible()) {
            mesh.Draw(shader);
        }
    }
}

void Model::Cleanup() {
    for (auto& mesh : meshes) {
        mesh.Cleanup();
    }

    meshes.clear();
}

void Model::InitalizeFromScene(const aiScene* scene) {
    meshes.reserve(scene->mNumMeshes);
    ProcessNode(scene->mRootNode, scene);
}

Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    std::vector<uint32_t> indices;
    indices.reserve(mesh->mNumFaces);

    ProcessMeshVerticies(mesh, vertices, indices);

    std::shared_ptr<Texture> texture{};
    auto* material = scene->mMaterials[mesh->mMaterialIndex];

    ProcessMeshTexture(mesh, material, texture);
    return Mesh(mesh->mName.C_Str(), std::move(vertices), std::move(indices), texture);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene) {
    for (size_t i = 0u; i < node->mNumMeshes; ++i) {
        const auto* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(std::move(ProcessMesh(mesh, scene)));
    }

    for (size_t i = 0u; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::OnUIUpdate() {
    std::string checkboxTitle = "Model: " + name;
    ImGui::Checkbox(checkboxTitle.c_str(), &visible);

    std::string headerTitle = "Meshes (" + std::to_string(meshes.size()) + ")";
    if (ImGui::CollapsingHeader(headerTitle.c_str())) {
        for (auto& mesh : meshes) {
            mesh.OnUIUpdate();
        }
    }

    ImGui::Separator();
}