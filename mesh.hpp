//
// Created by Ninter6 on 2025/8/5.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <cstdlib>

#include "glad/glad.h"

struct VAO {
    VAO();
    ~VAO();

    VAO(VAO&&) = delete;

    void bind() const;

    GLuint id = 0;
};

struct Buffer {
    Buffer(GLint type, bool dynamic);
    ~Buffer();

    Buffer(Buffer&&) = delete;

    void bind() const;
    void buffer(void *data, size_t offset, size_t size);
    void subdata(void *data, size_t offset, size_t length) const;

    void* map();
    void unmap();

    size_t size;

    GLint type;
    GLuint id;
    bool dynamic;

    void* mem_map = nullptr;
};

struct QuadVBO : Buffer {
    QuadVBO();
    void bind_attrib() const;
};

struct CubeVBO : Buffer {
    CubeVBO();
    void bind_attrib() const;
};

#endif //MESH_HPP
