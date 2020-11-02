#include "Model.hpp"

namespace {
    inline glm::vec3 ToGLMVector(const aiVector3D& vector) noexcept {
        return { vector.x, vector.y, vector.z };
    }
}

Model::Model(const std::string& name) : name(name) {

}

void Model::Draw(const Shader& shader) const {
    shader.Use();
    for (const auto& mesh : meshes) {
        mesh.Draw(shader);
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

    for (size_t i = 0u; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.Position = ToGLMVector(mesh->mVertices[i]);
        if (mesh->HasNormals()) {
            vertex.Normal = ToGLMVector(mesh->mNormals[i]);
        }

        vertices.emplace_back(std::move(vertex));
    }

    for (size_t i = 0u; i < mesh->mNumFaces; ++i) {
        const auto aiFace = mesh->mFaces[i];

        for (size_t j = 0u; j < aiFace.mNumIndices; ++j) {
            indices.emplace_back(aiFace.mIndices[j]);
        }
    }

    return Mesh(mesh->mName.C_Str(), std::move(vertices), std::move(indices));
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