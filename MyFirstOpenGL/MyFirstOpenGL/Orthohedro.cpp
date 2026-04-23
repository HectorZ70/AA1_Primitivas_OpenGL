#include "Orthohedro.h"

Orthohedro::Orthohedro()
{
	// Vertices del ortoedro (base y techo)
	GLfloat vertices[] = {
	// frente
	-1.0f, -0.5f,  0.3f,
	 1.0f, -0.5f,  0.3f,
	 1.0f,  0.5f,  0.3f,
	-1.0f,  0.5f,  0.3f,

	// atrás
	-1.0f, -0.5f, -0.3f,
	 1.0f, -0.5f, -0.3f,
	 1.0f,  0.5f, -0.3f,
	-1.0f,  0.5f, -0.3f
	};

	// Indices para formar las caras
	GLuint indices[] = {
		// Cara frontal
		0, 1, 2,
		2, 3, 0,

		// Cara trasera
		5, 4, 7,
		7, 6, 5,

		// Cara izquierda
		4, 0, 3,
		3, 7, 4,

		// Cara derecha
		1, 5, 6,
		6, 2, 1,

		// Cara superior
		3, 2, 6,
		6, 7, 3,

		// Cara inferior
		4, 5, 1,
		1, 0, 4
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

void Orthohedro::Draw(GLint offsetLocation, const glm::vec2& offset)
{
	// Actualizar el offset
	glUniform2f(offsetLocation, offset.x, offset.y);

	// Dibujar el ortoedro usando VAO
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
