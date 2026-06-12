#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvsVertexShader;
layout(location = 2) in vec3 normalsVertexShader;

out vec2 uvsGeometryShader;
out vec3 normalsGeometryShader;

uniform mat4 mvp;

void main() {
    uvsGeometryShader = uvsVertexShader;
    normalsGeometryShader = normalsVertexShader;

    gl_Position = mvp * vec4(position, 1.0);
}