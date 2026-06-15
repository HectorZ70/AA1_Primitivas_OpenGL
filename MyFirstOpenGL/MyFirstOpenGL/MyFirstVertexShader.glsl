#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 uvsVertexShader;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 uvsGeometryShader;

uniform mat4 mvp;
uniform mat4 model;
uniform int  objectType;

void main() {
    fragPos = vec3(model * vec4(posicion, 1.0));
    fragNormal = normalize(mat3(transpose(inverse(model))) * posicion);

    if (objectType == 3)
        uvsGeometryShader = uvsVertexShader;
    else
        uvsGeometryShader = vec2(0.0);

    gl_Position = mvp * vec4(posicion, 1.0);
}