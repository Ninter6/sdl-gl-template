//
// Created by Ninter6 on 2025/8/5.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "glad/glad.h"

struct ShaderBinder {
    ShaderBinder() = default;

    std::string process(const std::string& source);

    void bind(GLuint sh);

    std::vector<std::pair<std::string, uint32_t>> uniforms;
    std::vector<std::pair<std::string, uint32_t>> samplers;
};

class Shader {
public:
    Shader(const std::string& vs, const std::string& fs);
    ~Shader();

    Shader(Shader&&) = delete;

    static std::unique_ptr<Shader> from_file(std::string_view vs, std::string_view fs);

    void use() const;

    [[nodiscard]] GLuint ID() const {return handle;}

private:
    GLuint handle;

    static void compileShader(GLuint shader, std::string_view source);
};

#endif //SHADER_HPP
