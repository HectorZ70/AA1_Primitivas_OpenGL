#pragma once
#include <GL/glew.h>
#include <vector>

class Primitive
{
protected:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    void SetupMesh();

public:
    Primitive();
    virtual ~Primitive();

    void SetVerticesAndVariables(std::vector<GLfloat> vertices, std::vector<GLuint>  indices);
    virtual void Draw() const;
};

