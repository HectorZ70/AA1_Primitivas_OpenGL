#pragma once
#include <GL/glew.h>
#include <glm.hpp>

class Light
{
private:
	glm::vec3 color;
	float intensity;

	GLint colorLocation;
	GLint intensityLocation;

public:
	Light(const glm::vec3& color = glm::vec3(0.15f, 0.15f, 0.2f), float intensity = 1.0f);

	void Initialize(GLuint shaderProgram);

	void Apply() const;

	void SetColor(const glm::vec3& newColor);
	glm::vec3 GetColor() const;

	void SetIntensity(float newIntensity);
	float GetIntensity() const;
};

