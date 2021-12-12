#version 330 core

layout(location = 0) in vec3 OS_position;
layout(location = 1) in vec3 OS_normal;

uniform mat4 m, v, p;

out vec3 WS_position;
out vec3 WS_normal;

void main() {
    WS_position = (m * vec4(OS_position, 1.0)).xyz;
    WS_normal = (m * vec4(OS_normal, 0.0)).xyz;

    gl_Position = p * v * vec4(WS_position, 1.0);
}
