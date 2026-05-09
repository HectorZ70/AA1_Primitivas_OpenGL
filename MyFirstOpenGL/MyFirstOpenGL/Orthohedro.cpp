#include "Orthohedro.h"

Orthohedro::Orthohedro()
{
	vertices = {
		// Front
	   -0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,

	   // Back
	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f
	};

	indices = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Back
		5, 4, 7,
		7, 6, 5,

		// Left
		4, 0, 3,
		3, 7, 4,

		// Right
		1, 5, 6,
		6, 2, 1,

		// Superior
		3, 2, 6,
		6, 7, 3,

		// Inferior
		4, 5, 1,
		1, 0, 4
	};

	SetupMesh();
}
