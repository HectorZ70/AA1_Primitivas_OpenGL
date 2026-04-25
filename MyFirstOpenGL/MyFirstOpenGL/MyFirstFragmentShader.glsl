#version 440 core

in vec3 fragPos;
out vec4 FragColor;

uniform int objectType;
uniform float time;

void main()
{
    // Ciclo de colores
    float t = mod(time, 6.0); 

    vec3 color;
    if (objectType == 0)
    {
    if (t < 2.0)
        color = vec3(1.0, 0.0, 0.0); // rojo
    else if (t < 4.0)
        color = vec3(0.0, 1.0, 0.0); // verde
    else
        color = vec3(0.0, 0.0, 1.0); // azul
    }
    else if (objectType == 1 || objectType == 2)
    {
            if (fragPos.y > 0.0)
            color = vec3(1.0, 1.0, 0.0); // Amarillo
        else
            color = vec3(1.0, 0.5, 0.0); // Naranja
   
    }

    FragColor = vec4(color, 1.0);
}