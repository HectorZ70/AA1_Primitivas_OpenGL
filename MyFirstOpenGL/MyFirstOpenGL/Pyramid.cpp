#include "Pyramid.h"


Pyramid::Pyramid()
{
	GLfloat vertices[] = {
		// Base de la piramide
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,
		-0.5f, 0.0f, 0.5f,

		// Punta de la piramide
		0.0f, 0.8f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0,

		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Pyramid::Draw(GLint offsetLocation, const glm::vec2& offset)
{
	glUniform2f(offsetLocation, offset.x, offset.y);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
