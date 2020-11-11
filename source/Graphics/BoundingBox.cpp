#include "BoundingBox.hpp"
#include <GL/glew.h>

explicit BoundingBox::BoundingBox(const glm::vec3& center, float w, float l, float h)
    : center(center)
    , w(w)
    , l(l)
    , h(h)
{
    std::vector<Vertex> vertices;
    vertices.reserve(8);

    {
        Vertex v{};
        v.Position = { center.x - w/2, center.y - l/2, center.z + h/2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + w / 2, center.y - l / 2, center.z + h / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + w / 2, center.y + l / 2, center.z + h / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x - w / 2, center.y + l / 2, center.z + h / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x - w / 2, center.y - l / 2, center.z - h / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + w / 2, center.y - l / 2, center.z - h / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x + w / 2, center.y + l / 2, center.z - h / 2 };
        vertices.push_back(v);
    }

    {
        Vertex v{};
        v.Position = { center.x - w / 2, center.y + l / 2, center.z - h / 2 };
        vertices.push_back(v);
    }

    std::vector<uint32_t> indices =
    { 0, 1, 2, 0, 2, 3, 2, 3, 6, 3, 6, 7, 0, 1, 5, 0, 4, 5, 4, 5, 6, 4, 6, 7, 0, 4, 6, 0, 7, 3, 1, 5, 2, 5, 6, 2 };

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
void BoundingBox::Draw(const std::shared_ptr<Shader>& shader) const {
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}