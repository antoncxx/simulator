#pragma once
#include <memory>
#include "Shader.hpp"

struct BoundingBox {
private:
    glm::vec3 size;
    glm::vec3 center;

    uint32_t vbo{}, ibo{}, vao{};
public:
    BoundingBox() = default;
    BoundingBox(const glm::vec3& center, const glm::vec3& size);

    void Draw(const std::shared_ptr<Shader>& shader) const;
    void Cleanup();
};