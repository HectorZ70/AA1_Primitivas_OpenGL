#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvsVertexShader;
layout(location = 2) in vec3 normalsVertexShader;

out vec2  uvsGeometryShader;
out vec3  normalsGeometryShader;
out vec3  worldPositionGeometryShader;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    vec3 worldPos = vec3(model * vec4(position, 1.0));

    uvsGeometryShader = uvsVertexShader;
    normalsGeometryShader = normalMatrix * normalsVertexShader;
    worldPositionGeometryShader = worldPos;

    gl_Position = mvp * vec4(position, 1.0);
}
