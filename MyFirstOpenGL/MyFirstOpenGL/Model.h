#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>

class Model
{
public:
    Model(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render(GLuint shaderProgram, float time, glm::vec3 position) const;
private:
    GLuint VAO, VBO, uvVBO;
    unsigned int numVertexs;
};
#endif
