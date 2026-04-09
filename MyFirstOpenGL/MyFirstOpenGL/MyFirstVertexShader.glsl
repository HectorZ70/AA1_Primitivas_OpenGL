#version 440 core

layout(location = 0) in vec3 posicion;

uniform vec2 offset;
uniform float time;

void main() {

    // ESCALA
    float scale = 0.5;

    // ROTACIÓN X
    float angleX = time;
    mat3 rotX = mat3(
        1, 0, 0,
        0, cos(angleX), -sin(angleX),
        0, sin(angleX), cos(angleX)
    );

    // ROTACIÓN Y
    float angleY = time;
    mat3 rotY = mat3(
        cos(angleY), 0, sin(angleY),
        0, 1, 0,
        -sin(angleY), 0, cos(angleY)
    );

    vec3 pos = posicion * scale;       // aplicar escala
    pos = rotY * rotX * pos;           // luego rotar

    // ?? MOVIMIENTO ARRIBA/ABAJO
    float verticalMovement = sin(time) * 0.3; // amplitud 0.3 unidades
    pos.y += verticalMovement;

    gl_Position = vec4(pos.x + offset.x, pos.y + offset.y, pos.z, 1.0);
}