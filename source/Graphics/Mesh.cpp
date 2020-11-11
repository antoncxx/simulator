#include "Mesh.hpp"

#include <GL/glew.h>
#include <algorithm>

Mesh::Mesh(const std::string& name,
    std::vector<Vertex>&& vertices, 
    std::vector<uint32_t>&& indices) noexcept
    : name(name)
    , vertices(std::move(vertices))
    , indices(std::move(indices))
    , visible(true) {
    SetupMesh();
}

void Mesh::Cleanup() {
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

    glBindVertexArray(0);
}

void Mesh::SetupBoundingBox() {
    const auto fx = [](const Vertex& v1, const Vertex& v2) {
        return v1.Position.x < v1.Position.x;
    };

    const auto fy = [](const Vertex& v1, const Vertex& v2) {
        return v1.Position.x < v1.Position.x;
    };

    const auto fz = [](const Vertex& v1, const Vertex& v2) {
        return v1.Position.x < v1.Position.x;
    };

    auto xpair = std::minmax_element(vertices.cbegin(), vertices.cend(), fx);
    auto ypair = std::minmax_element(vertices.cbegin(), vertices.cend(), fy);
    auto zpair = std::minmax_element(vertices.cbegin(), vertices.cend(), fz);

    // todo: validate results

    glm::vec3 size{
        xpair.second->Position.x - xpair.first->Position.x,
        xpair.second->Position.y - xpair.first->Position.y,
        xpair.second->Position.z - xpair.first->Position.z
    };
    

}

void Mesh::Draw(const std::shared_ptr<Shader>& shader) const {
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::OnUIUpdate() {
    std::string title = "Mesh " + name;
    ImGui::Checkbox(title.c_str(), &visible);
}
