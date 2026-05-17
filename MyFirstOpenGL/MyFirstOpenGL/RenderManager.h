#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "Primitive.h"
#include "Utils.h"

class RenderManager
{
private:
    GLuint shaderProgram;
    GLint  mvpLocation;
    GLint  objectTypeLocation;
    GLint  timeLocation;

public:
    RenderManager();

    void Initialize(
        const char* vertexShaderPath,
        const char* fragmentShaderPath
    );
    void Clear();

    // Recibe solo lo que necesita: geometría, MVP, tipo y tiempo
    void Render(
        const Primitive& primitive,
        const glm::mat4& mvp,
        int              objectType,
        float            time
    );

    GLuint GetProgram() const;
};
