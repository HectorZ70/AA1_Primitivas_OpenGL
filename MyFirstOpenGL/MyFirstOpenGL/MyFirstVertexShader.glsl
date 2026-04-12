#version 440 core

layout(location = 0) in vec3 posicion;

out vec3 fragPos;
uniform vec2 offset;
uniform float time;
uniform int objectType;

void main() {

    float scale = 0.5;

    vec3 pos = posicion * scale;

    mat3 rotX;
    mat3 rotY;

    float verticalMovement;

    if (objectType == 0)
    {
        float angleX = time;
        float angleY = time;

        rotX = mat3(
            1, 0, 0,
            0, cos(angleX), -sin(angleX),
            0, sin(angleX), cos(angleX)
        );

        rotY = mat3(
            cos(angleY), 0, sin(angleY),
            0, 1, 0,
            -sin(angleY), 0, cos(angleY)
        );

        verticalMovement = sin(time) * 0.75;
    }
    else if (objectType == 1)
    {
        rotX = mat3(1.0);

        float angleY = time * 2.0;

        rotY = mat3(
            cos(angleY), 0, sin(angleY),
            0, 1, 0,
            -sin(angleY), 0, cos(angleY)
        );

        verticalMovement = sin(time) * 0.75;;
    }

    pos = rotY * rotX * pos;
    pos.y += verticalMovement;

    fragPos = pos;

    gl_Position = vec4(pos.x + offset.x,
                       pos.y + offset.y,
                       pos.z,
                       1.0);
}