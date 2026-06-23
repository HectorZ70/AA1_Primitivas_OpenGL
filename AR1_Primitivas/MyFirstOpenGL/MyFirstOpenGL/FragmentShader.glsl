#version 440 core

in vec4 clipPositionFragmentShader;

out vec4 FragColor;

uniform int objectType; // 0 = cubo, 1 = ortoedro, 2 = piramide
uniform float time;

const int OBJECT_TYPE_CUBE = 0;
const int OBJECT_TYPE_ORTHO = 1;
const int OBJECT_TYPE_PYRAMID = 2;

const vec4 COLOR_YELLOW = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 COLOR_ORANGE = vec4(1.0, 0.5, 0.0, 1.0);
const vec4 COLOR_RED = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 COLOR_GREEN = vec4(0.0, 1.0, 0.0, 1.0);
const vec4 COLOR_BLUE = vec4(0.0, 0.0, 1.0, 1.0);

const float COLOR_CYCLE_DURATION = 2.0; 

void main()
{
    // Cubo y ortoedro
    if (objectType == OBJECT_TYPE_CUBE || objectType == OBJECT_TYPE_ORTHO)
    {
        float ndcY = clipPositionFragmentShader.y / clipPositionFragmentShader.w;
        FragColor = (ndcY > 0.0) ? COLOR_YELLOW : COLOR_ORANGE;
        return;
    }

    // Piramide
    if (objectType == OBJECT_TYPE_PYRAMID)
    {
        float phase = mod(time, COLOR_CYCLE_DURATION * 3.0);

        if (phase < COLOR_CYCLE_DURATION)
        {
            FragColor = COLOR_RED;
        } 
        else if (phase < COLOR_CYCLE_DURATION * 2.0)
        {
            FragColor = COLOR_GREEN;
        }
        else
        {
            FragColor = COLOR_BLUE;
        }
         
        return;
    }

    FragColor = vec4(1.0); 
}
