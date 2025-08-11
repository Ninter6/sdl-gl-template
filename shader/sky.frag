#version 410 core

flat in vec3 camPos;
in vec3 fragPos;

out vec4 color;

void main() {
    vec3 camDir = normalize(fragPos - camPos);

    float t = camDir.y * .5 + .5;
    color = mix(vec4(.3, .4, .5, 1.), vec4(.7, .8, 1., 1.), (3-2*t)*t*t);
}