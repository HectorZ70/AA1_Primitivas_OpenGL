#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <GL/glew.h>

class Model
{
public:
    Model(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render(GLuint shaderProgram, float time) const; 
private:
    GLuint VAO, VBO;
    unsigned int numVertexs;
};
#endif
