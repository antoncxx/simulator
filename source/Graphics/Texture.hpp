#pragma once

#include <cstdint>
#include <memory>
#include <filesystem>

#include "GraphicalResource.hpp"

class ResourceManager;

class Texture final : public GraphicalResource {
    uint32_t m_id{};
public:
    void Bind(uint32_t slot) const;
    virtual ~Texture() = default;
private:
    Texture(uint32_t* data, int32_t width, int32_t height);
    static std::shared_ptr<Texture> Create(const std::filesystem::path& imagePath);

    void Cleanup() override;

    friend ResourceManager;
};


