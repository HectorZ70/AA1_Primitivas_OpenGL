#version 440 core
in vec3 fragPos;
in vec3 fragNormal;
in vec2 uvsGeometryShader;

out vec4 FragColor;

uniform int   objectType;
uniform float time;
uniform sampler2D textureSampler;

// Dia/noche
uniform vec3  sunDirection;
uniform vec3  moonDirection;
uniform float sunIntensity;
uniform float moonIntensity;
uniform vec3  ambientColor;
uniform float ambientStrength;

// Linterna
uniform bool  flashlightOn;
uniform vec3  flashlightPos;
uniform vec3  flashlightDir;
uniform float flashlightInnerCutoff;
uniform float flashlightOuterCutoff;
uniform float flashlightRange;

const float MIN_DIFFUSE = 0.15;

void main() {
    vec3 norm = normalize(fragNormal);

    vec3 sunColor  = vec3(1.0, 0.95, 0.7);
    vec3 moonColor = vec3(0.4, 0.5,  0.8);

    float sunDiff  = max(dot(norm, sunDirection),  0.0) * sunIntensity;
    float moonDiff = max(dot(norm, moonDirection), 0.0) * moonIntensity;

    vec3 diffuseLight = sunColor  * max(sunDiff,  MIN_DIFFUSE) * sunIntensity
                      + moonColor * max(moonDiff, 0.0)         * moonIntensity;
    diffuseLight = max(diffuseLight, vec3(MIN_DIFFUSE));

    vec3 flashlightContrib = vec3(0.0);
    if (flashlightOn) {
        vec3  toFrag  = fragPos - flashlightPos;
        float dist    = length(toFrag);
        vec3  fragDir = normalize(toFrag);

        float theta   = dot(fragDir, normalize(flashlightDir));
        float epsilon = flashlightInnerCutoff - flashlightOuterCutoff;
        float intensity = clamp((theta - flashlightOuterCutoff) / epsilon, 0.0, 1.0);

        // Atenuacion por distancia
        float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
        attenuation *= clamp(1.0 - (dist / flashlightRange), 0.0, 1.0);

        float diff = max(dot(norm, -fragDir), 0.0);
        flashlightContrib = vec3(1.0, 0.95, 0.8) * diff * intensity * attenuation * 1.5;
    }

    vec3 ambient = ambientColor;

    vec4 baseColor;
    if (objectType == 3) {
        baseColor = texture(textureSampler, uvsGeometryShader);
    } else if (objectType == 0) {
        float t = mod(time, 6.0);
        vec3 col = (t < 2.0) ? vec3(1,0,0) : (t < 4.0) ? vec3(0,1,0) : vec3(0,0,1);
        baseColor = vec4(col, 1.0);
    } else {
        vec3 col = (fragPos.y > 0.0) ? vec3(1,1,0) : vec3(1,0.5,0);
        baseColor = vec4(col, 1.0);
    }

    vec3 lighting = ambient + diffuseLight + flashlightContrib;
    FragColor = vec4(baseColor.rgb * lighting, baseColor.a);
}