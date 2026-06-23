#version 440 core

layout(location = 0) in vec3 position;

out vec4 clipPositionGeometryShader;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    clipPositionGeometryShader = gl_Position;
}
