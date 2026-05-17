#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "Primitive.h"
#include "Model.h"
#include "Utils.h"

class RenderManager
{
private:
    GLuint shaderProgram;

    GLint mvpLocation;
    GLint objectTypeLocation;
    GLint timeLocation;

    GLint textureSamplerLocation;

public:
    RenderManager();

    void Initialize(
        const char* vertexShaderPath,
        const char* fragmentShaderPath
    );

    void Clear();

    // Render Primitive
    void Render(
        const Primitive& primitive,
        const glm::mat4& mvp,
        int              objectType,
        float            time
    );

    // Render Model
    void Render(
        const Model& model,
        const glm::mat4& mvp,
        float            time
    );

    GLuint GetProgram() const;
};
