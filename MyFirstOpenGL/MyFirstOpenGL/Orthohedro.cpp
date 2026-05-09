#include "Orthohedro.h"

Orthohedro::Orthohedro()
{
	// Vertices del ortoedro (base y techo)
	vertices = {
		// frente
	   -0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,

	   // atrás
	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f
	};

	// Indices para formar las caras
	indices = {
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

	SetupMesh();
}
