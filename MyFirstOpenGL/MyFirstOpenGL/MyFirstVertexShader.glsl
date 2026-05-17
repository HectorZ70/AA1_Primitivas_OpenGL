#version 440 core
layout(location = 0) in vec3 position;

out vec3 fragPos;

uniform mat4 mvp;

void main()
{
    fragPos     = position;
    gl_Position = mvp * vec4(position, 1.0);
}