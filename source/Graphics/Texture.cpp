#include "Texture.hpp"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../STBI/stb_image.h"

std::shared_ptr<Texture> Texture::Create(const std::filesystem::path& imagePath) {
    if (!std::filesystem::exists(imagePath)) {
        throw std::runtime_error("File does not exist!");
    }

    int32_t width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    auto* image = stbi_load(imagePath.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (image == nullptr) {
        throw std::runtime_error("Failed to load texture!");
    }

    auto instance = std::shared_ptr<Texture>(new Texture(reinterpret_cast<uint32_t*>(image), width, height));
    stbi_image_free(image);

    return instance;
}

void Texture::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture::Texture(uint32_t* data, int32_t width, int32_t height) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Cleanup() {
    glDeleteTextures(1, &m_id);
}

