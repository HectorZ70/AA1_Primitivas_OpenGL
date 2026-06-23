#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec3 worldPosition;

out vec4 FragColor;

uniform int objectType;
uniform float time;
uniform sampler2D textureSampler;

uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform vec3 flashlightColor;
uniform float flashlightCutoff;
uniform float flashlightOuter;
uniform float flashlightIntensity;
uniform bool flashlightOn;

uniform vec4 tint;
uniform float tintStrength;

uniform vec3 sunDirection;
uniform vec3 moonDirection;
uniform float sunIntensity;
uniform float moonIntensity;
uniform float ambientStrength;

uniform float flashlightInnerCutoff;
uniform float flashlightOuterCutoff;
uniform float flashlightRange;

const int OBJECT_TYPE_MODEL = 3; // troll / roca / perro con textura
const int OBJECT_TYPE_TINTED_SOLID = 4;

const float MIN_DIFFUSE = 0.15;

void main()
{
    vec4 baseColor;

    if (objectType == OBJECT_TYPE_TINTED_SOLID)
    {
        baseColor = vec4(tintStrength, 1.0, 0.0, 0.0);
    }
    else if (objectType == OBJECT_TYPE_MODEL)
    {
        vec4 texColor = texture(textureSampler, uvsFragmentShader);
        baseColor = texColor * mix(vec4(1.0), tint, tintStrength);
    }
    else
    {
        baseColor = vec4(0.8, 0.8, 0.8, 1.0);
    }

    if (objectType == OBJECT_TYPE_TINTED_SOLID)
    {
        FragColor = baseColor;
        return;
    }

    vec3 norm = normalize(normalsFragmentShader);

    vec3 ambient = ambientColor * ambientIntensity;

    vec3 classicFlashlightContribution = vec3(0.0);

    if (flashlightOn)
    {
        vec3 toFrag = normalize(worldPosition - flashlightPos);

        float theta = dot(toFrag, normalize(flashlightDir));
        float epsilon = flashlightCutoff - flashlightOuter;

        float coneBlend =
            clamp((theta - flashlightOuter) / epsilon, 0.0, 1.0);

        float diff = max(dot(norm, -toFrag), 0.0);

        float dist = length(worldPosition - flashlightPos);

        float atten =
            1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

        classicFlashlightContribution =
            flashlightColor *
            flashlightIntensity *
            diff *
            coneBlend *
            atten;
    }

    // Sol y luna
    vec3 sunColor = vec3(1.0, 0.95, 0.7);
    vec3 moonColor = vec3(0.4, 0.5, 0.8);

    float sunDiff = max(dot(norm, sunDirection), 0.0) * sunIntensity;

    float moonDiff = max(dot(norm, moonDirection), 0.0) * moonIntensity;

    vec3 diffuseLight = sunColor * max(sunDiff, MIN_DIFFUSE) * sunIntensity + moonColor * max(moonDiff, 0.0) * moonIntensity;

    diffuseLight = max(diffuseLight, vec3(MIN_DIFFUSE));

    // Flashlight 
    vec3 flashContribDevelop = vec3(0.0);

    if (flashlightOn)
    {
        vec3 toFrag2 = worldPosition - flashlightPos;

        float dist2 = length(toFrag2);

        vec3 fragDir = normalize(toFrag2);

        float theta2 =
            dot(fragDir, normalize(flashlightDir));

        float epsilon2 =
            flashlightInnerCutoff -
            flashlightOuterCutoff;

        float intensity =
            clamp(
                (theta2 - flashlightOuterCutoff) /
                epsilon2,
                0.0,
                1.0
            );

        float atten2 =
            1.0 / (1.0 + 0.09 * dist2 + 0.032 * dist2 * dist2);

        atten2 *=
            clamp(
                1.0 - (dist2 / flashlightRange),
                0.0,
                1.0
            );

        float diff2 =
            max(dot(norm, -fragDir), 0.0);

        flashContribDevelop =
            vec3(1.0, 0.95, 0.8) *
            diff2 *
            intensity *
            atten2 *
            1.5;
    }

    vec3 lighting =
        ambient +
        ambientColor * ambientStrength +
        diffuseLight +
        classicFlashlightContribution +
        flashContribDevelop;

    FragColor = vec4(baseColor.rgb * lighting, baseColor.a);
}
