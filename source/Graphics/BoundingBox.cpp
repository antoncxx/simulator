#include "BoundingBox.hpp"
#include <GL/glew.h>

BoundingBox::BoundingBox(const glm::vec3& center, float w, float l, float h)
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

}
void BoundingBox::Draw(const std::shared_ptr<Shader>& shader) const {
}