#version 330 core

layout(location = 0) in vec3 OS_position;
layout(location = 1) in vec3 OS_normal;

uniform mat4 m, v, p;
uniform bool terrain;
uniform vec3 objectColor;

out vec3 WS_position;
out vec3 WS_normal;
out vec3 baseColor;

void main() {
    WS_position = (m * vec4(OS_position, 1.0)).xyz;
    WS_normal = (m * vec4(OS_normal, 0.0)).xyz;

    gl_Position = p * v * vec4(WS_position, 1.0);

    if (!terrain) {
        baseColor = objectColor;
    }
    else {
        float height = WS_position.y;
        if (height > 1) {
            baseColor = vec3(0.952, 0.949, 0.949);
        }
        else if (height > 0) {
            baseColor = vec3(0.745, 0.717, 0.674);
        }
        else if (height > -0.7) {
            baseColor = vec3(0.3, 0.6, 0.4);
        }
        else if (height > -0.8) {
            baseColor = vec3(0.847, 0.796, 0.552);
        }
        else {
            baseColor = vec3(0.294, 0.537, 0.823);
        }
    }
}
