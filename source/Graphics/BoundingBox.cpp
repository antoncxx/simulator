#include "BoundingBox.hpp"
#include <GL/glew.h>
#include "Mesh.hpp"

BoundingBox::BoundingBox(const glm::vec3& center, const glm::vec3& size)
    : center(center)
    , size(size)
{
    std::vector<Vertex> vertices;
    vertices.reserve(8);

    {
        Vertex v{};
        v.Position = { center.x - size.x / 2, center.y - size.y / 2, center.z + size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + size.x / 2, center.y - size.y / 2, center.z + size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x - size.x / 2, center.y + size.y / 2, center.z + size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + size.x / 2, center.y - size.y / 2, center.z - size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + size.x / 2, center.y + size.y / 2, center.z - size.z / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2 };
        vertices.push_back(v);
    }

    uint32_t indices[] = {
        0, 1, 1, 2, 2, 3, 3, 7, 7, 4, 4, 5, 5, 6, 6, 7, 6, 2, 5 , 1, 4, 0, 3, 0
    };


    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

    glBindVertexArray(0);

}
void BoundingBox::Draw(const std::shared_ptr<Shader>& shader) const {

    glBindVertexArray(vao);
    glLineWidth(2);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BoundingBox::Cleanup() {
    glDeleteVertexArrays(1, &vao);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}