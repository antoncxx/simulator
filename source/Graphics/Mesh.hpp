#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Shader.hpp"
#include "UI.hpp"
#include "BoundingBox.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh final : public UIListener { 
    std::string name;

    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;

    uint32_t VertexArrayObject, VertexBufferObject, ElementBufferObject;

    bool visible;
    bool drawBox;
    BoundingBox box;
public:
    explicit Mesh(const std::string& name, std::vector<Vertex>&& verticies, std::vector<uint32_t>&& indicies) noexcept;
    Mesh(const Mesh&) = default;
    Mesh(Mesh&&) = default;
    ~Mesh() = default;

    void Draw(const std::shared_ptr<Shader> &shader) const;
    void Cleanup();
    bool IsVisible() const noexcept { return visible; }
    void OnUIUpdate() override;

    auto GetVertices() const noexcept { return vertices; }
    auto GetIndices() const noexcept { return indices; }

private:
    void SetupMesh();
    void SetupBoundingBox();
};