#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 uvsVertexShader;

out vec3 fragPos;
out vec2 uvsGeometryShader;
uniform vec2 offset;
uniform float time;
uniform int objectType;
uniform vec3 objectPosition;

void main() {

    float scale = 0.5;

    vec3 pos = posicion * scale;

    mat3 rotX;
    mat3 rotY;
    mat3 rotZ;

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

        rotZ = mat3(1.0);

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

        rotZ = mat3(1.0);

        verticalMovement = sin(time) * 0.75;
    }
   else if(objectType == 2)
{
    // Forma ortoedro (la original)
    vec3 orthoScale = vec3(2.0, 1.0, 0.6);

    // Cubo perfecto
    vec3 cubeScale = vec3(1.0);

    float t = (sin(time) + 1.0) * 0.5;

    vec3 scale = mix(orthoScale, cubeScale, t);

    pos *= scale;

    float angleZ = time * 2.0;

    rotX = mat3(1.0);
    rotY = mat3(1.0);

    rotZ = mat3(
         cos(angleZ), -sin(angleZ), 0,
        sin(angleZ),  cos(angleZ), 0,
        0, 0, 1
    );

    verticalMovement = 0.0;
}

    else if (objectType == 3)
{
uvsGeometryShader = uvsVertexShader;
    rotX = mat3(1.0);
    rotY = mat3(1.0);
    rotZ = mat3(1.0);
    verticalMovement = 0.0;
}

    pos = rotY * rotX * rotZ * pos;
    pos.y += verticalMovement;

    fragPos = pos;

    gl_Position = vec4(
    pos + objectPosition,
    1.0
);
}