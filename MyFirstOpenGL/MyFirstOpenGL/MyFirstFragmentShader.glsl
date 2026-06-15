#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;
in vec3 worldPosition;

out vec4 FragColor;

uniform int   objectType;
uniform float time;
uniform sampler2D textureSampler;

// HEAD: iluminación clásica
uniform vec3  ambientColor;
uniform float ambientIntensity;
uniform vec3  flashlightPos;
uniform vec3  flashlightDir;
uniform vec3  flashlightColor;
uniform float flashlightCutoff;
uniform float flashlightOuter;
uniform float flashlightIntensity;
uniform bool  flashlightOn;

// HEAD: tint
uniform vec4  tint;
uniform float tintStreght;

// develop: día/noche
uniform vec3  sunDirection;
uniform vec3  moonDirection;
uniform float sunIntensity;
uniform float moonIntensity;
uniform float ambientStrength;

// develop: linterna extendida
uniform float flashlightInnerCutoff;
uniform float flashlightOuterCutoff;
uniform float flashlightRange;

// Constantes
const vec4  COLOR_YELLOW = vec4(1.0, 1.0, 0.0, 1.0);
const vec4  COLOR_ORANGE = vec4(1.0, 0.5, 0.0, 1.0);
const vec4  COLOR_RED    = vec4(1.0, 0.0, 0.0, 1.0);
const vec4  COLOR_GREEN  = vec4(0.0, 1.0, 0.0, 1.0);
const vec4  COLOR_BLUE   = vec4(0.0, 0.0, 1.0, 1.0);

const float COLOR_CYCLE_DURATION = 2.0;
const float MIN_DIFFUSE = 0.15;

void main()
{
    vec4 baseColor;

    if (objectType == 4)
    {
        baseColor = vec4(tintStreght, 1.0, 0.0, 0.0);
    }
    else if (objectType == 0)
    {
        float tMod = mod(time, 6.0);
        vec3 col = (tMod < 2.0)
            ? vec3(1,0,0)
            : (tMod < 4.0)
                ? vec3(0,1,0)
                : vec3(0,0,1);

        baseColor = vec4(col, 1.0);
    }
    else if (objectType == 1)
    {
        float ndcY = primitivePosition.y / primitivePosition.w;
        baseColor = (ndcY > 0.0)
            ? COLOR_YELLOW
            : COLOR_ORANGE;
    }
    else if (objectType == 2)
    {
        float phase = mod(time, COLOR_CYCLE_DURATION * 3.0);

        if (phase < COLOR_CYCLE_DURATION)
            baseColor = COLOR_RED;
        else if (phase < COLOR_CYCLE_DURATION * 2.0)
            baseColor = COLOR_GREEN;
        else
            baseColor = COLOR_BLUE;
    }
    else if (objectType == 3)
    {
        vec4 texColor = texture(textureSampler, uvsFragmentShader);
        baseColor = texColor * mix(vec4(1.0), tint, tintStreght);
    }
    else
    {
        baseColor = vec4(0.8, 0.8, 0.8, 1.0);
    }

    if (objectType == 4)
    {
        FragColor = baseColor;
        return;
    }

    vec3 norm = normalize(normalsFragmentShader);

    // Ambient HEAD
    vec3 ambient = ambientColor * ambientIntensity;

    // Flashlight HEAD
    vec3 flashContribHEAD = vec3(0.0);

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

        flashContribHEAD =
            flashlightColor *
            flashlightIntensity *
            diff *
            coneBlend *
            atten;
    }

    // Sol y luna
    vec3 sunColor = vec3(1.0, 0.95, 0.7);
    vec3 moonColor = vec3(0.4, 0.5, 0.8);

    float sunDiff =
        max(dot(norm, sunDirection), 0.0) * sunIntensity;

    float moonDiff =
        max(dot(norm, moonDirection), 0.0) * moonIntensity;

    vec3 diffuseLight =
        sunColor * max(sunDiff, MIN_DIFFUSE) * sunIntensity +
        moonColor * max(moonDiff, 0.0) * moonIntensity;

    diffuseLight = max(diffuseLight, vec3(MIN_DIFFUSE));

    // Flashlight develop
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
        flashContribHEAD +
        flashContribDevelop;

    FragColor = vec4(baseColor.rgb * lighting, baseColor.a);
}