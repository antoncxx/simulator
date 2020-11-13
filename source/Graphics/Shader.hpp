#pragma once

#include <map>
#include <cstdint>
#include <string_view>

#include <glm/glm.hpp>

#include "GraphicalResource.hpp"

class ResourceManager;

class Shader final : public GraphicalResource {
    uint32_t                        id{};
    std::map<std::string, uint32_t> chache{};
public:
    void Use() const;
    void SetUniform(const std::string& name, const glm::mat4& matrix);
    void SetUniform(const std::string& name, const glm::vec3& vec3);

    inline auto Id() const noexcept { return id; }

    ~Shader() = default;
    Shader() = default;
private:    

    void Compile(std::string_view vertexSource, std::string_view fragmentSource);
    void Cleanup() override;

    friend class ResourceManager;
};


