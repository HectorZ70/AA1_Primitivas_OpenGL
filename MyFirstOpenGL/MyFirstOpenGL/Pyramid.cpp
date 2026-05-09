#include "Pyramid.h"

Pyramid::Pyramid()
{
    vertices =
    {
        -0.5f, 0.0f, -0.5f,
         0.5f, 0.0f, -0.5f,
         0.5f, 0.0f,  0.5f,
        -0.5f, 0.0f,  0.5f,

         0.0f, 0.8f,  0.0f
    };

    indices =
    {
        0, 1, 2,
        2, 3, 0,

        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    SetupMesh();
}