#include "Model.h"
#include <iostream>

Model::Model(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals)
{
    this->numVertexs = vertexs.size() / 3;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(float), vertexs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::Render(GLuint shaderProgram, float time) const
{
    glUseProgram(shaderProgram);

    // objectType = 3 ? sin rotaciones especiales, centrado en origen
    GLint typeLoc = glGetUniformLocation(shaderProgram, "objectType");
    GLint timeLoc = glGetUniformLocation(shaderProgram, "time");
    GLint posLoc = glGetUniformLocation(shaderProgram, "objectPosition");

    glUniform1i(typeLoc, 3);           // tipo nuevo para el modelo OBJ
    glUniform1f(timeLoc, time);
    glUniform3f(posLoc, 0.0f, 0.0f, 0.0f);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numVertexs);
    glBindVertexArray(0);
}