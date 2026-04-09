#include "Pyramid.h"


Pyramid::Pyramid()
{
	// Vertices de la piramide (base y punta)
	GLfloat vertices[] = {
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,
		-0.5f, 0.0f, 0.5f,

		0.0f, 0.8f, 0.0f
	};

	// Indices para formar los triangulos de la base y las caras
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0,

		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	// Configuracion de VAO, VBO, EBO
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
	// Actualizar el offset
	glUniform2f(offsetLocation, offset.x, offset.y);

	// Dibujar la piramide usando VAO
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
