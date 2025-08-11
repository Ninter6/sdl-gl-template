#version 410 core

layout (location = 0) in vec2 apos;

layout (binding = 0) uniform UBO {
    mat4 proj;
    mat4 view;
};

flat out vec3 camPos;
out vec3 fragPos;

void main() {
    vec2 p = apos * 2 - 1;
    gl_Position = vec4(p, 0, 1);

    mat4 vp = proj * view;
    mat4 inv_vp = inverse(vp);
    vec4 f = vec4(inv_vp * vec4(p, 1, 1));
    fragPos = f.xyz / f.w;
    camPos = -transpose(mat3(view)) * view[3].xyz;
}