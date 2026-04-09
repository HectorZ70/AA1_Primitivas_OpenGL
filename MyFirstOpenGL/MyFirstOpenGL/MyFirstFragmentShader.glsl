#version 440 core

out vec4 FragColor;

uniform float time;

void main()
{
    // Ciclo de colores
    float t = mod(time, 6.0); 

    vec3 color;

    if (t < 2.0)
        color = vec3(1.0, 0.0, 0.0); // rojo
    else if (t < 4.0)
        color = vec3(0.0, 1.0, 0.0); // verde
    else
        color = vec3(0.0, 0.0, 1.0); // azul

    FragColor = vec4(color, 1.0);
}