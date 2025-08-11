//
// Created by Ninter6 on 2025/8/5.
//

#include <regex>
#include <cassert>
#include <fstream>

#include "shader.hpp"

std::string ShaderBinder::process(const std::string& src) {
    static const std::regex p{
        R"((?:layout.+?binding\s*=\s*)(\d+)(?:.+?uniform\s+)(sampler\w*\s+)?([A-z]\w*))",
        std::regex_constants::optimize
    }, r{
        R"((binding\s*=\s*\d+\s*,)|(,\s*binding\s*=\s*\d+)|(layout\s*\(\s*binding[^,]+?\)))",
        std::regex_constants::optimize
    };

    auto begin = std::sregex_iterator(src.begin(), src.end(), p);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        const auto& m = *it;
        if (m[2].str().empty())
            uniforms.emplace_back(m[3].str(), std::stod(m[1]));
        else
            samplers.emplace_back(m[3].str(), std::stod(m[1]));
    }

    return std::regex_replace(src, r, "");
}

void ShaderBinder::bind(GLuint sh) {
    glUseProgram(sh);
    for (const auto& [name, index] : uniforms) {
        auto ubo_id = glGetUniformBlockIndex(sh, name.c_str());
        glUniformBlockBinding(sh, ubo_id, index);
    }
    for (const auto& [name, index] : samplers) {
        auto tex_id = glGetUniformLocation(sh, name.c_str());
        glUniform1i(tex_id, (int)index);
    }
}

Shader::Shader(const std::string& vs, const std::string& fs) {
    ShaderBinder sb{};

    auto vertex = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertex, sb.process(vs));

    auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragment, sb.process(fs));

    handle = glCreateProgram();
    glAttachShader(handle, vertex);
    glAttachShader(handle, fragment);
    glLinkProgram(handle);

    GLint success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if(!success) {
        GLint length;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
        std::string infoLog("\0", length);
        glGetProgramInfoLog(handle, 512, nullptr, infoLog.data());
        throw std::runtime_error("Failed to link shader:\n" + infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    sb.bind(handle);
}

void Shader::compileShader(GLuint shader, std::string_view source) {
    auto src = source.data();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

#ifndef NDEBUG
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string errorLog("\0", length);
        glGetShaderInfoLog(shader, length, nullptr, errorLog.data());
        throw std::runtime_error("Failed to compile shader:\n" + errorLog);
    }
#endif
}

void Shader::use() const {
    glUseProgram(handle);
}

Shader::~Shader() {
    glDeleteProgram(handle);
}

std::unique_ptr<Shader> Shader::from_file(std::string_view vs, std::string_view fs) {
    std::ifstream fvsh{vs}, ffsh{fs};
    assert(fvsh.good() && ffsh.good());
    std::string vsh{std::istreambuf_iterator<char>{fvsh}, std::istreambuf_iterator<char>{}},
        fsh{std::istreambuf_iterator<char>{ffsh}, std::istreambuf_iterator<char>{}};
    return std::make_unique<Shader>(vsh, fsh);
}

