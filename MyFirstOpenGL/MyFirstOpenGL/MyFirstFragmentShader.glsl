#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;
in vec3 worldPosition;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 uvsGeometryShader;

out vec4 FragColor;

uniform int   objectType;
uniform float time;
uniform sampler2D textureSampler;

// Lighting (from HEAD)
uniform vec3  ambientColor;
uniform float ambientIntensity;
uniform vec3  flashlightPos;
uniform vec3  flashlightDir;
uniform vec3  flashlightColor;
uniform float flashlightCutoff;
uniform float flashlightOuter;
uniform float flashlightIntensity;
uniform bool  flashlightOn;

// Tint (from feature/CameraComponent)
uniform vec4  tint;
uniform float tintStreght;

const vec4  COLOR_YELLOW = vec4(1.0, 1.0, 0.0, 1.0);
const vec4  COLOR_ORANGE = vec4(1.0, 0.5, 0.0, 1.0);
const vec4  COLOR_RED    = vec4(1.0, 0.0, 0.0, 1.0);
const vec4  COLOR_GREEN  = vec4(0.0, 1.0, 0.0, 1.0);
const vec4  COLOR_BLUE   = vec4(0.0, 0.0, 1.0, 1.0);
const float COLOR_CYCLE_DURATION = 2.0;

void main()
{
    // Step 1: base colour
    vec4 baseColor;

    if (objectType == 4)        // Cloud / special
    {
        baseColor = vec4(tintStreght, 1.0, 0.0, 0.0);
    }
    else if (objectType == 0)   // Cube
    {
        float ndcY = primitivePosition.y / primitivePosition.w;
        baseColor  = (ndcY > 0.0) ? COLOR_YELLOW : COLOR_ORANGE;
    }
    else if (objectType == 1)   // Ortho
    {
        float ndcY = primitivePosition.y / primitivePosition.w;
        baseColor  = (ndcY > 0.0) ? COLOR_YELLOW : COLOR_ORANGE;
    }
    else if (objectType == 2)   // Pyramid – colour cycle
    {
        float phase = mod(time, COLOR_CYCLE_DURATION * 3.0);
        if      (phase < COLOR_CYCLE_DURATION)       baseColor = COLOR_RED;
        else if (phase < COLOR_CYCLE_DURATION * 2.0) baseColor = COLOR_GREEN;
        else                                         baseColor = COLOR_BLUE;
    }
    else if (objectType == 3)   // Textured OBJ + tint
    {
        vec4 texColor = texture(textureSampler, uvsFragmentShader);
        baseColor = texColor * mix(vec4(1.0), tint, tintStreght);
    }
    else
    {
        baseColor = vec4(0.8, 0.8, 0.8, 1.0);
    }
// Dia/noche
uniform vec3  sunDirection;
uniform vec3  moonDirection;
uniform float sunIntensity;
uniform float moonIntensity;
uniform vec3  ambientColor;
uniform float ambientStrength;

    // Step 2: lighting (skip for type 4 — already set above)
    if (objectType != 4)
    {
        vec3 normal = normalize(normalsFragmentShader);
// Linterna
uniform bool  flashlightOn;
uniform vec3  flashlightPos;
uniform vec3  flashlightDir;
uniform float flashlightInnerCutoff;
uniform float flashlightOuterCutoff;
uniform float flashlightRange;

        vec3 ambient = ambientColor * ambientIntensity;
const float MIN_DIFFUSE = 0.15;

        vec3 flashlight = vec3(0.0);
        if (flashlightOn)
        {
            vec3  toFrag    = normalize(worldPosition - flashlightPos);
            float theta     = dot(toFrag, normalize(flashlightDir));
            float epsilon   = flashlightCutoff - flashlightOuter;
            float coneBlend = clamp((theta - flashlightOuter) / epsilon, 0.0, 1.0);
            float diff      = max(dot(normal, -toFrag), 0.0);
            float dist      = length(worldPosition - flashlightPos);
            float atten     = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
            flashlight = flashlightColor * flashlightIntensity * diff * coneBlend * atten;
        }
void main() {
    vec3 norm = normalize(fragNormal);

        vec3 finalLight = clamp(ambient + flashlight, 0.0, 1.0);
        FragColor = vec4(baseColor.rgb * finalLight, baseColor.a);
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
    else
    {
        FragColor = baseColor;

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