#include "Model.h"
#include <iostream>

Model::Model(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals)
{
    this->textureID = 0;   // sin textura por defecto
    this->numVertexs = vertexs.size() / 3;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->uvVBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(float), vertexs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, this->uvVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::SetTexture(GLuint textureID)
{
    this->textureID = textureID;
}

void Model::Render(GLuint shaderProgram, float time, glm::vec3 position) const
{
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

    glUniform1i(glGetUniformLocation(shaderProgram, "objectType"), 3);
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectPosition"), position.x, position.y, position.z);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numVertexs);
    glBindVertexArray(0);
}