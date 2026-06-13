#version 440 core

in vec3 fragPos;
in vec2 uvsGeometryShader;

out vec4 FragColor;

uniform int objectType;
uniform float time;
uniform sampler2D textureSampler;
uniform vec4 tint;
uniform float tintStreght;

void main()
{

    if (objectType == 4)
    {
        FragColor = vec4(tintStreght, 1.0, 0.0, 0.0);
    }

    // OBJ con textura
    else if (objectType == 3)
    {
        vec4 texColor = texture(textureSampler, uvsGeometryShader);
        FragColor = texColor * mix(vec4(1.0), tint, tintStreght);
    }

    // Pirámide RGB animada
    else if (objectType == 0)
    {
        float t = mod(time, 6.0);

        vec3 color;

        if (t < 2.0)
            color = vec3(1.0, 0.0, 0.0);
        else if (t < 4.0)
            color = vec3(0.0, 1.0, 0.0);
        else
            color = vec3(0.0, 0.0, 1.0);

        FragColor = vec4(color, 1.0);
    }

    // Cubo y ortho
    else
    {
        vec3 color =
            (fragPos.y > 0.0)
            ? vec3(1.0, 1.0, 0.0)
            : vec3(1.0, 0.5, 0.0);

        FragColor = vec4(color, 1.0);
    }
}