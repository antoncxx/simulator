#include "Shader.hpp"

#include <GL/glew.h>

#include <array>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

namespace {
    enum { CHECK_SHADER, CHECK_PROGRAM };

    void Validate(uint8_t type, uint32_t id) {
        std::array<char, 1024> buffer{};
        int success;

        switch (type)
        {
        case CHECK_SHADER: {
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(id, buffer.size(), nullptr, buffer.data());
                throw std::runtime_error(buffer.data());
            }
            break;
        }

        case CHECK_PROGRAM: {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(id, buffer.size(), nullptr, buffer.data());
                throw std::runtime_error(buffer.data());
            }
            break;
        }
        }
    }

    uint32_t CompileShader(uint32_t type, const char* source) {
        uint32_t shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);

        glCompileShader(shader);
        Validate(CHECK_SHADER, shader);

        return shader;
    }

    uint32_t LinkProgram(uint32_t vertex, uint32_t fragment) {
        uint32_t program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        glLinkProgram(program);
        Validate(CHECK_PROGRAM, program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return program;
    }
}


void Shader::Use() const {
    glUseProgram(id);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& matrix) {
    if (auto iter = chache.find(name); iter != chache.end()) {
        glUniformMatrix4fv(iter->second, 1, GL_FALSE, glm::value_ptr(matrix));
    } else {
        const auto location = glGetUniformLocation(id, name.data());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
        chache.insert({ name, location });
    }
}

void Shader::SetUniform(const std::string& name, const glm::vec3& vec3) {
    if (auto iter = chache.find(name); iter != chache.end()) {
        glUniform3fv(iter->second, 1, glm::value_ptr(vec3));
    }
    else {
        const auto location = glGetUniformLocation(id, name.data());
        glUniform3fv(iter->second, 1, glm::value_ptr(vec3));
        chache.insert({ name, location });
    }
}

void Shader::Compile(std::string_view vertexSource, std::string_view fragmentSource) {
    auto vs = CompileShader(GL_VERTEX_SHADER,   vertexSource.data());
    auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource.data());

    id      = LinkProgram(vs, fs);
}

void Shader::Cleanup() {
    chache.clear();
    glDeleteProgram(id);
}
