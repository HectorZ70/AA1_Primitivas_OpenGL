#include "Model.h"

Model::Model(
    const std::vector<float>& vertices,
    const std::vector<float>& uvs,
    const std::vector<float>& normals)
    : textureID(0), numVertices(vertices.size() / 3)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &uvVBO);

    glBindVertexArray(vao);

    // Vértices en location 0
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UVs en location 1
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER,
        uvs.size() * sizeof(float),
        uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void     Model::SetTexture(GLuint id) { textureID = id; }
GLuint   Model::GetTextureID()    const { return textureID; }
GLuint   Model::GetVAO()          const { return vao; }
unsigned Model::GetNumVertices()  const { return numVertices; }

void Model::Draw() const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
}