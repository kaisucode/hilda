#version 330 core

in vec3 WS_position;
in vec3 WS_normal;
in vec3 baseColor;

out vec3 fragColor;

uniform vec4 lightDir;
uniform vec4 WS_camPosition;
uniform vec3 backgroundColor;
uniform vec4 shadowTint;
uniform vec4 highlightTint;
uniform vec3 outlineColor;
uniform bool useOutlines;
uniform float outlineWeight;

void main() {
    float vertToEyeDist = length(WS_position - WS_camPosition.xyz);

    float maxOutlineWeight = outlineWeight + 0.025;
    float minOutlineWeight = outlineWeight - 0.025;

    vec3 WS_toLight = normalize(lightDir.xyz);
    float luminosity = 0.1 + 0.9 * max(0.0, dot(normalize(WS_normal), WS_toLight));

    fragColor = baseColor;

    if (useOutlines && dot(normalize(WS_camPosition.xyz - WS_position), WS_normal)
                   < mix(maxOutlineWeight, minOutlineWeight,
                   max(0.0, dot(WS_normal, WS_toLight)))) {
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
