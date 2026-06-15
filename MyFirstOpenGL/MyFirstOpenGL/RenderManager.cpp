#include "RenderManager.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

RenderManager::RenderManager()
    : shaderProgram(0),
    mvpLocation(-1), objectTypeLocation(-1),
    timeLocation(-1), textureSamplerLocation(-1),
    modelLocation(-1), normalMatrixLocation(-1)
{
}

void RenderManager::Initialize(
    const char* vertexShaderPath,
    const char* geometryShaderPath,
    const char* fragmentShaderPath)
{
    ShaderProgram shaders;
    shaders.vertexShader = LoadVertexShader(vertexShaderPath);
    shaders.geometryShader = LoadGeometryShader(geometryShaderPath);
    shaders.fragmentShader = LoadFragmentShader(fragmentShaderPath);
    shaderProgram = CreateProgram(shaders);

    glUseProgram(shaderProgram);
    mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    objectTypeLocation = glGetUniformLocation(shaderProgram, "objectType");
    timeLocation = glGetUniformLocation(shaderProgram, "time");
    textureSamplerLocation = glGetUniformLocation(shaderProgram, "textureSampler");
    tint = glGetUniformLocation(shaderProgram, "tint");
    tintStrenght = glGetUniformLocation(shaderProgram, "tintStreght");

    // Lighting
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");
    ambientLight.Initialize(shaderProgram);
    flashlight.Initialize(shaderProgram);
}

void RenderManager::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::SetLight(
    const glm::mat4& modelMatrix,
    const glm::vec3& camPos,
    const glm::vec3& camForward,
    bool flashOn)
{
    glUseProgram(shaderProgram);

    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMat));

    flashlight.SetOn(flashOn);
    flashlight.Update(camPos, camForward);

    ambientLight.Apply();
    flashlight.Apply();
}

Light& RenderManager::GetLight()
{
    return ambientLight;
}

Flashlight& RenderManager::GetFlashlight()
{
    return flashlight;
}

void RenderManager::Render(
    const Primitive& primitive,
    const glm::mat4& mvp,
    int objectType, float time)
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(objectTypeLocation, objectType);
    glUniform1f(timeLocation, time);
    primitive.Draw();
}

void RenderManager::Render(
    const Model& model,
    const glm::mat4& mvp,
    float            time,
    int objectType,
    glm::vec4        tintColor,
    float            tintStrenghtValue)
    float time)
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(objectTypeLocation, objectType);
    glUniform1i(objectTypeLocation, 3);
    glUniform1i(objectTypeLocation, 3);
    glUniform1f(timeLocation, time);
    glUniform4fv(tint, 1, glm::value_ptr(tintColor));      // location=tint, valor=tintColor
    glUniform1f(tintStrenght, tintStrenghtValue);           // location=tintStrenght, valor=tintStrenghtValue

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.GetTextureID());
    glUniform1i(textureSamplerLocation, 0);
    model.Draw();
}

GLuint RenderManager::GetProgram() const { return shaderProgram; }