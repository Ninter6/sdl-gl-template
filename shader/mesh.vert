#version 410 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anor;

out vec3 frag_normal;

layout (binding = 0) uniform UBO {
    mat4 proj;
    mat4 view;
};

uniform mat4 m;

void main() {
    gl_Position = proj * view * m * vec4(apos, 1.0);
    frag_normal = anor;
}