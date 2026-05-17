#version 440 core
in  vec3 fragPos;
out vec4 FragColor;

uniform int   objectType;
uniform float time;
uniform sampler2D textureSampler;

void main()
{
    float t = mod(time, 6.0);
    vec3 color;

    if (objectType == 0)
    {
        if      (t < 2.0) color = vec3(1.0, 0.0, 0.0);
        else if (t < 4.0) color = vec3(0.0, 1.0, 0.0);
        else               color = vec3(0.0, 0.0, 1.0);
    }
    else
    {
        color = (fragPos.y > 0.0)
            ? vec3(1.0, 1.0, 0.0)
            : vec3(1.0, 0.5, 0.0);
    }
    else // objectType 1 y 2
    {
        vec3 color = (fragPos.y > 0.0)
            ? vec3(1.0, 1.0, 0.0)  // amarillo
            : vec3(1.0, 0.5, 0.0); // naranja
        FragColor = vec4(color, 1.0);
    }
}