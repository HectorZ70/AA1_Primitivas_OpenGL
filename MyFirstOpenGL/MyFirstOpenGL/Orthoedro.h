#pragma once

#include <GL/glew.h>
#include <glm.hpp>

class Orthoedro {
public:
	Orthoedro();
	void Draw(GLint offsetLocation, const glm::vec2& offset);

private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};