#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;
in vec3 worldPosition;

out vec4 FragColor;

uniform int objectType;
uniform float time;
uniform sampler2D textureSampler;
uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform vec3  flashlightPos;       
uniform vec3  flashlightDir;       
uniform vec3  flashlightColor;     
uniform float flashlightCutoff;   
uniform float flashlightOuter;     
uniform float flashlightIntensity; 
uniform bool  flashlightOn;       

// Orange and yellow color code extracted from AI (Claude)

const vec4 COLOR_YELLOW = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 COLOR_ORANGE = vec4(1.0, 0.5, 0.0, 1.0);
const vec4 COLOR_RED = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 COLOR_GREEN = vec4(0.0, 1.0, 0.0, 1.0);
const vec4 COLOR_BLUE = vec4(0.0, 0.0, 1.0, 1.0);

// Pyramid color duration
const float COLOR_CYCLE_DURATION = 2.0;

void main()
{
	// ?? Step 1: pick base colour for this object type ??
    vec4 baseColor;

    if (objectType == 0)        // Cube
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
        if      (phase < COLOR_CYCLE_DURATION)        baseColor = COLOR_RED;
        else if (phase < COLOR_CYCLE_DURATION * 2.0)  baseColor = COLOR_GREEN;
        else                                           baseColor = COLOR_BLUE;
    }
    else if (objectType == 3)   // Textured OBJ
    {
        baseColor = texture(textureSampler, uvsFragmentShader);
    }
    else
    {
        baseColor = vec4(0.8, 0.8, 0.8, 1.0);
    }

    // ?? Step 2: lighting (applies to ALL object types) ??
    vec3 normal = normalize(normalsFragmentShader);

    // Ambient
    vec3 ambient = ambientColor * ambientIntensity;

    // Flashlight
    vec3 flashlight = vec3(0.0);
    if (flashlightOn)
    {
        vec3  toFrag    = normalize(worldPosition - flashlightPos);
        float theta     = dot(toFrag, normalize(flashlightDir));
        float epsilon   = flashlightCutoff - flashlightOuter;
        float coneBlend = clamp((theta - flashlightOuter) / epsilon, 0.0, 1.0);

        float diff  = max(dot(normal, -toFrag), 0.0);
        float dist  = length(worldPosition - flashlightPos);
        float atten = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

        flashlight = flashlightColor * flashlightIntensity * diff * coneBlend * atten;
    }

    vec3 finalLight = clamp(ambient + flashlight, 0.0, 1.0);
    FragColor = vec4(baseColor.rgb * finalLight, baseColor.a);
}