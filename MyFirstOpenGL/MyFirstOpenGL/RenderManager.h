#pragma once

#include <GL/glew.h>

#include "GameObject.h"
#include "Utils.h"

class RenderManager
{
private:

    GLuint shaderProgram;

    GLint timeLocation;
    GLint objectTypeLocation;

public:

    RenderManager();

    void Initialize(
        const char* vertexShaderPath,
        const char* fragmentShaderPath
    );

    void Clear();

    void Render(GameObject& object, float time);

    GLuint GetProgram() const;
};

