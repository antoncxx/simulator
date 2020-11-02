#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
    std::string name;

    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;

    uint32_t VertexArrayObject, VertexBufferObject, ElementBufferObject;

public:
    explicit Mesh(const std::string& name, std::vector<Vertex>&& verticies, std::vector<uint32_t>&& indicies) noexcept;
    Mesh(Mesh&&) = default;
    ~Mesh() = default;

    void Draw(Shader const& shader) const;
    void Cleanup();
private:
    void SetupMesh();
};