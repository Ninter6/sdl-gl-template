#version 410 core

in vec3 frag_normal;
out vec4 color;

layout(binding = 0) uniform sampler2D tex;

uniform vec3 light_dir;
const float ambient = .5f;

void main() {
    color = vec4(1.0, 0.5, 0.2, 1.0) * (max(dot(frag_normal,-light_dir), 0) + ambient);
    color.a = 1.0;
}