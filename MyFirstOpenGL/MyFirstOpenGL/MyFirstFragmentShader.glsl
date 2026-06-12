#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;

out vec4 FragColor;

uniform int objectType;
uniform float time;
uniform sampler2D textureSampler;

void main()
{
    // OBJ con textura
    if (objectType == 3)
    {
        FragColor = texture(textureSampler, uvsFragmentShader);
    }
}