#pragma once
#include <GL/glew.h>
#include <vector>

class Model
{
private:
    GLuint vao;
    GLuint vbo;
    GLuint uvVBO;
    GLuint textureID;
    unsigned int numVertices;

public:
    Model(
        const std::vector<float>& vertices,
        const std::vector<float>& uvs,
        const std::vector<float>& normals
    );

    void     SetTexture(GLuint textureID);
    GLuint   GetTextureID() const;
    GLuint   GetVAO()       const;
    unsigned GetNumVertices() const;

    void Draw() const;
};
