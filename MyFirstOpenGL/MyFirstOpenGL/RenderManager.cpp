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

    positionLocation =
        glGetUniformLocation(shaderProgram, "objectPosition");
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

    // Tiempo
    glUniform1f(timeLocation, time);

    // Tipo de objeto
    glUniform1i(
        objectTypeLocation,
        object.GetObjectType()
    );

    // Posición del objeto
    glUniform3f(
        positionLocation,
        object.GetPosition().x,
        object.GetPosition().y,
        object.GetPosition().z
    );

    object.Draw();
}

GLuint RenderManager::GetProgram() const
{
    return shaderProgram;
}