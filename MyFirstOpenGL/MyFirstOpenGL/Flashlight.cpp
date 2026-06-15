#include "Flashlight.h"

Flashlight::Flashlight(const glm::vec3& color, float innerAngleDegrees, float outerAngleDegrees, float intensity, bool startOn)
	: position(0.f), direction(0.f, 0.f, -1.f), 
	color(color), intensity(intensity), on(startOn), 
	positionLocation(-1), directionLocation(-1), 
	colorLocation(-1), cutOffLocation(-1), 
	outerCutOffLocation(-1), intensityLocation(-1), 
	onLocation(-1)
{
	SetConeAngles(innerAngleDegrees, outerAngleDegrees);
}

void Flashlight::Initialize(GLuint shaderProgram) {
	positionLocation = glGetUniformLocation(shaderProgram, "flashlightPos");
	directionLocation = glGetUniformLocation(shaderProgram, "flashlightDir");
	colorLocation = glGetUniformLocation(shaderProgram, "flashlightColor");
	cutOffLocation = glGetUniformLocation(shaderProgram, "flashlightCutOff");
	outerCutOffLocation = glGetUniformLocation(shaderProgram, "flashlightOuter");
	intensityLocation = glGetUniformLocation(shaderProgram, "flashlightIntensity");
	onLocation = glGetUniformLocation(shaderProgram, "flashlightOn");
}

void Flashlight::Apply() const {
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(outerCutOffLocation, outerCutOff);
	glUniform1f(intensityLocation, intensity);
	glUniform1f(onLocation, on ? 1 : 0);
}

void Flashlight::Update(const glm::vec3& newPosition, const glm::vec3& newDirection) {
	position = newPosition;
	direction = newDirection;
}

void Flashlight::Toggle() {
	on = !on;
}

void Flashlight::SetOn(bool isOn) {
	on = isOn;
}

bool Flashlight::IsOn() const {
	return on;
}

void Flashlight::SetColor(const glm::vec3& newColor) {
	color = newColor;
}

glm::vec3 Flashlight::GetColor() const {
	return color;
}

void Flashlight::SetIntensity(float newIntensity) {
	intensity = newIntensity;
}

float Flashlight::GetIntensity() const {
	return intensity;
}

void Flashlight::SetConeAngles(float innerAngleDegrees, float outerAngleDegrees) {
	cutOff = glm::cos(glm::radians(innerAngleDegrees));
	outerCutOff = glm::cos(glm::radians(outerAngleDegrees));
}