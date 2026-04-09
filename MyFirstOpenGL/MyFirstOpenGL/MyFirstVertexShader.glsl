#version 440 core

layout(location = 0) in vec3 posicion;

uniform vec2 offset;
uniform float time;

void main() {

    float scale = 0.5;

    // Matrices de rotacion
    float angleX = time;
    mat3 rotX = mat3(
        1, 0, 0,
        0, cos(angleX), -sin(angleX),
        0, sin(angleX), cos(angleX)
    );

    float angleY = time;
    mat3 rotY = mat3(
        cos(angleY), 0, sin(angleY),
        0, 1, 0,
        -sin(angleY), 0, cos(angleY)
    );

    vec3 pos = posicion * scale; // Aplicar escala
    pos = rotY * rotX * pos; // Luego rotar

    // Movimiento vertical
    float verticalMovement = sin(time) * 0.3;
    pos.y += verticalMovement;

    gl_Position = vec4(pos.x + offset.x, pos.y + offset.y, pos.z, 1.0);
}