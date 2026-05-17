#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 uvsVertexShader;

out vec3 fragPos;
out vec2 uvsGeometryShader;

uniform mat4 mvp;
uniform int  objectType;

void main()
{
    fragPos = posicion;

    // Solo los modelos OBJ usan UVs
    if (objectType == 3)
        uvsGeometryShader = uvsVertexShader;
    else
        uvsGeometryShader = vec2(0.0);

    gl_Position = mvp * vec4(posicion, 1.0);
}