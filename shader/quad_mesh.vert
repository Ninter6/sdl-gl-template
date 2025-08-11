#version 410 core

layout (location = 0) in vec2 apos;

layout (binding = 0, std140) uniform UBO {
    mat4 proj;
    mat4 view;
};

out vec2 TexCoord;

void main() {
    TexCoord = apos * 2000 - 1000;
    gl_Position = proj * view * vec4(TexCoord.x, 0.0f, TexCoord.y, 1.0f);
}