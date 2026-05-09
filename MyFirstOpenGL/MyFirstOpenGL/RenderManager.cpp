#include "RenderManager.h"

RenderManager::RenderManager()
    : shaderProgram(0)
{
}

void RenderManager::Initialize(
    const char* vertexShaderPath,
    const char* fragmentShaderPath
)
{
    ShaderProgram shaders;

    shaders.vertexShader =
        LoadVertexShader(vertexShaderPath);

    shaders.fragmentShader =
        LoadFragmentShader(fragmentShaderPath);

    shaderProgram = CreateProgram(shaders);

    glUseProgram(shaderProgram);

    // Uniforms
    timeLocation =
        glGetUniformLocation(shaderProgram, "time");

    objectTypeLocation =
        glGetUniformLocation(shaderProgram, "objectType");
}

void RenderManager::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::Render(GameObject& object, float time)
{
    if (!object.IsVisible())
        return;

    glUseProgram(shaderProgram);

    glUniform1f(timeLocation, time);

    glUniform1i(
        objectTypeLocation,
        object.GetObjectType()
    );

    object.Draw();
}

GLuint RenderManager::GetProgram() const
{
    return shaderProgram;
}