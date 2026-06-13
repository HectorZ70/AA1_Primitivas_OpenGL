#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;

out vec4 FragColor;

uniform int objectType;
uniform float time;
uniform sampler2D textureSampler;

// Orange and yellow color code extracted from AI (Claude)

const vec4 COLOR_YELLOW = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 COLOR_ORANGE = vec4(1.0, 0.5, 0.0, 1.0);
const vec4 COLOR_RED = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 COLOR_GREEN = vec4(0.0, 1.0, 0.0, 1.0);
const vec4 COLOR_BLUE = vec4(0.0, 0.0, 1.0, 1.0);

// Pyramid color duration
const float COLOR_CYCLE_DURATION = 2.0;

void main()
{
	// objectType == 0 --> Cube
	// objectType == 1 --> Ortho
	// objectType == 2 --> Pyramid
	// objectType == 3 --> Models with texture (Troll, Rock and Dog)
	
	if (objectType == 0)
	{
		float ndcY = primitivePosition.y / primitivePosition.w;
		FragColor = (ndcY > 0.0) ? COLOR_YELLOW : COLOR_ORANGE;
	}
	else if (objectType == 1)
	{
		float ndcY = primitivePosition.y / primitivePosition.w;
		FragColor = (ndcY > 0.0) ? COLOR_YELLOW : COLOR_ORANGE;
	}
	else if (objectType == 2)
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
	}
	else if (objectType == 3)
	{	
		FragColor = texture(textureSampler, uvsFragmentShader);
	}
}