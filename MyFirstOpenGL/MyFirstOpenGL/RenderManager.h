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

    int objectType = 3;

    GLint textureSamplerLocation;
    GLuint tint;
    GLuint tintStrenght;

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
        float            time,
        int objectType,
        glm::vec4        tintColor = glm::vec4(1.0f),
        float            tintStrenghtValue = 0.0f
    );
    GLuint GetProgram() const;
};
