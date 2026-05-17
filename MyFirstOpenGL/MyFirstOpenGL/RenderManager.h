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

    // Uniforms compartidos
    GLint mvpLocation;
    GLint objectTypeLocation;
    GLint timeLocation;

    // Uniform exclusivo de Model
    GLint textureSamplerLocation;

public:
    RenderManager();

    void Initialize(
        const char* vertexShaderPath,
        const char* fragmentShaderPath
    );

    void Clear();

    // Para Primitive (pirámide, cubo, ortoedro)
    void Render(
        const Primitive& primitive,
        const glm::mat4& mvp,
        int              objectType,
        float            time
    );

    // Para Model (OBJ con textura)
    void Render(
        const Model& model,
        const glm::mat4& mvp,
        float            time
    );

    GLuint GetProgram() const;
};
