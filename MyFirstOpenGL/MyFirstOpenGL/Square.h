#pragma once

#include <GL/glew.h>
#include <glm.hpp>

class Square
{
public:
	Square();
	void Draw(GLint offsetLocation, const glm::vec2& offset);

private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

