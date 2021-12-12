#version 330 core

in vec3 WS_position;
in vec3 WS_normal;

out vec3 fragColor;

uniform vec3 lightDir;
uniform vec4 WS_camPosition;
uniform vec3 backgroundColor;
uniform vec3 objectColor;
uniform vec4 shadowTint;
uniform vec4 highlightTint;
uniform vec3 outlineColor;

void main() {
    float vertToEyeDist = length(WS_position.xz - WS_camPosition.xz);

    float maxOutlineThick = 0.3;
    float minOutlineThick = 0.2;

    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    float luminosity = 0.2 + 0.8 * max(0.0, dot(normalize(WS_normal), WS_toLight));

    fragColor = objectColor;

    if (dot(normalize(WS_camPosition.xyz - WS_position), WS_normal)
                   < mix(maxOutlineThick, minOutlineThick,
                   max(0.0, dot(WS_normal, normalize(lightDir))))) {
        fragColor = outlineColor;
    }
    else if (luminosity < 0.4) {
        fragColor = mix(fragColor, shadowTint.xyz, shadowTint.w);
    }
    else if (luminosity > 0.9) {
        fragColor = mix(fragColor, highlightTint.xyz, highlightTint.w);
    }

    fragColor = mix(fragColor, backgroundColor, 0.05 * vertToEyeDist);
}
