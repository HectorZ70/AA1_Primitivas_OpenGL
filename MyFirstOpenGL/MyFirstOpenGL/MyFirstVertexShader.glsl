#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvsVertexShader;
layout(location = 2) in vec3 normalsVertexShader;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 uvsGeometryShader;
out vec3 normalsGeometryShader;
out vec3 worldPositionGeometryShader;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 model;
uniform int  objectType;

void main() {
    uvsGeometryShader = uvsVertexShader;
    normalsGeometryShader = normalMatrix * normalsVertexShader; // <-- THIS WAS MISSING
    worldPositionGeometryShader = vec3(model * vec4(position, 1.0));
    gl_Position = mvp * vec4(position, 1.0);
void main() {
    fragPos = vec3(model * vec4(posicion, 1.0));
    fragNormal = normalize(mat3(transpose(inverse(model))) * posicion);

    if (objectType == 3)
        uvsGeometryShader = uvsVertexShader;
    else
        uvsGeometryShader = vec2(0.0);

    gl_Position = mvp * vec4(posicion, 1.0);
}