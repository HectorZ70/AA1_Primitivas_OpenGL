#include "Light.h"

Light::Light(const glm::vec3& color, float intensity)
	: color(color), intensity(intensity), colorLocation(-1), intensityLocation(-1)
{
}

void Light::Initialize(GLuint shaderProgram)
{
	colorLocation = glGetUniformLocation(shaderProgram, "ambientColor");
	intensityLocation = glGetUniformLocation(shaderProgram, "ambientIntensity");
}

void Light::Apply() const
{
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(intensityLocation, intensity);
}

void Light::SetColor(const glm::vec3& newColor)
{
	color = newColor;
}

glm::vec3 Light::GetColor() const
{
	return color;
}

void Light::SetIntensity(float newIntensity)
{
	intensity = newIntensity;
}

float Light::GetIntensity() const
{
	return intensity;
}
