#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class BoundingBox {
    glm::vec3 center;
    float w, l, h;
public:
    explicit BoundingBox(const glm::vec3& center, float w, float l, float h);
    void Draw(const std::shared_ptr<Shader>& shader) const;
};