#version 440 core

in vec3 fragPos;
in vec2 uvsGeometryShader;

out vec4 FragColor;

uniform int objectType;
uniform float time;
uniform sampler2D textureSampler;

void main()
{
    if (objectType == 3)
    {
        // Modelo OBJ con textura
        FragColor = texture(textureSampler, uvsGeometryShader);
    }
    else if (objectType == 0)
    {
        float t = mod(time, 6.0);
        vec3 color;
        if      (t < 2.0) color = vec3(1.0, 0.0, 0.0); // rojo
        else if (t < 4.0) color = vec3(0.0, 1.0, 0.0); // verde
        else              color = vec3(0.0, 0.0, 1.0);  // azul
        FragColor = vec4(color, 1.0);
    }
    else // objectType 1 y 2
    {
        vec3 color = (fragPos.y > 0.0)
            ? vec3(1.0, 1.0, 0.0)  // amarillo
            : vec3(1.0, 0.5, 0.0); // naranja
        FragColor = vec4(color, 1.0);
    }
}