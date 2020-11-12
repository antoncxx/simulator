#include "Mesh.hpp"

#include <GL/glew.h>
#include <algorithm>
#include <iostream>

Mesh::Mesh(const std::string& name,
    std::vector<Vertex>&& vertices, 
    std::vector<uint32_t>&& indices,
    const std::shared_ptr<Texture>& texture) noexcept
    : name(name)
    , vertices(std::move(vertices))
    , indices(std::move(indices))
    , visible(true)
    , drawBox(false)
    , texture(texture) {
    SetupMesh();
    SetupBoundingBox();
}

void Mesh::Cleanup() {
    box.Cleanup();

    glDeleteVertexArrays(1, &VertexArrayObject);

    glDeleteBuffers(1, &VertexBufferObject);
    glDeleteBuffers(1, &ElementBufferObject);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VertexArrayObject);

    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    glBindVertexArray(VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordinate));

    glBindVertexArray(0);
}

void Mesh::SetupBoundingBox() {
    const auto fx = [](const Vertex& v1, const Vertex& v2) {
        return v1.Position.x < v2.Position.x;
    };

    const auto fy = [](const Vertex& v1, const Vertex& v2) {
        return v1.Position.y < v2.Position.y;
    };

    const auto fz = [](const Vertex& v1, const Vertex& v2) {
        return v1.Position.z < v2.Position.z;
    };

    auto xpair = std::minmax_element(vertices.cbegin(), vertices.cend(), fx);
    auto ypair = std::minmax_element(vertices.cbegin(), vertices.cend(), fy);
    auto zpair = std::minmax_element(vertices.cbegin(), vertices.cend(), fz);

    // todo: validate results

    glm::vec3 center{
        (xpair.second->Position.x + xpair.first->Position.x) / 2,
        (ypair.second->Position.y + ypair.first->Position.y) / 2,
        (zpair.second->Position.z + zpair.first->Position.z) / 2
    };
    
    glm::vec3 size{
        (xpair.second->Position.x - xpair.first->Position.x),
        (ypair.second->Position.y - ypair.first->Position.y),
        (zpair.second->Position.z - zpair.first->Position.z)
    };

    box = BoundingBox(center, size);
}

void Mesh::Draw(const std::shared_ptr<Shader>& shader) const {
    if (texture != nullptr) {
        texture->Bind(0);
    }

    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (drawBox) {
        box.Draw(shader);
    }
}

void Mesh::OnUIUpdate() {
    std::string title = "Mesh " + name;
    ImGui::PushID(title.c_str());
    ImGui::Text(title.c_str());
    ImGui::Checkbox("Visible", &visible);
    ImGui::Checkbox("Draw bounding box", &drawBox);
    ImGui::Separator();
    ImGui::PopID();
}
