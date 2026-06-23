#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Flashlight
{
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float cutOff;
	float outerCutOff;
	float intensity;
	bool on;

	GLint positionLocation;
	GLint directionLocation;
	GLint colorLocation;
	GLint cutOffLocation;
	GLint outerCutOffLocation;
	GLint intensityLocation;
	GLint onLocation;

public:
	Flashlight(const glm::vec3& color = glm::vec3(1.f, 0.95f, 0.85f), float innerAngleDegrees = 15.f, float outerAngleDegrees = 22.f, float intensity = 3.f, bool startOn = true);

	void Initialize(GLuint shaderProgram);

	void Apply() const;

	void Update(const glm::vec3& newPosition, const glm::vec3& newDirection);

	void Toggle();
	void SetOn(bool isOn);
	bool IsOn() const;

	void SetColor(const glm::vec3& newColor);
	glm::vec3 GetColor() const;

	void SetIntensity(float newIntensity);
	float GetIntensity() const;

	void SetConeAngles(float innerAngleDegrees, float outerAngleDegrees);
};

