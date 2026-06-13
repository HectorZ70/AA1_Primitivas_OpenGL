#include "RenderManager.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

RenderManager::RenderManager()
    : shaderProgram(0),
    mvpLocation(-1), objectTypeLocation(-1),
    timeLocation(-1), textureSamplerLocation(-1),
    modelLocation(-1), normalMatrixLocation(-1),
    ambientColorLocation(-1), ambientIntensityLocation(-1),
    flashlightPosLocation(-1), flashlightDirLocation(-1),
    flashlightColorLocation(-1), flashlightCutoffLocation(-1),
    flashlightOuterLocation(-1), flashlightIntensityLocation(-1),
    flashlightOnLocation(-1)
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

    // Lighting
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");
    ambientColorLocation = glGetUniformLocation(shaderProgram, "ambientColor");
    ambientIntensityLocation = glGetUniformLocation(shaderProgram, "ambientIntensity");
    flashlightPosLocation = glGetUniformLocation(shaderProgram, "flashlightPos");
    flashlightDirLocation = glGetUniformLocation(shaderProgram, "flashlightDir");
    flashlightColorLocation = glGetUniformLocation(shaderProgram, "flashlightColor");
    flashlightCutoffLocation = glGetUniformLocation(shaderProgram, "flashlightCutoff");
    flashlightOuterLocation = glGetUniformLocation(shaderProgram, "flashlightOuter");
    flashlightIntensityLocation = glGetUniformLocation(shaderProgram, "flashlightIntensity");
    flashlightOnLocation = glGetUniformLocation(shaderProgram, "flashlightOn");
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

    // Ambient: dim bluish night sky feel
    glUniform3f(ambientColorLocation, 0.15f, 0.15f, 0.2f);
    glUniform1f(ambientIntensityLocation, 1.0f);

    // Flashlight
    glUniform3f(flashlightPosLocation, camPos.x, camPos.y, camPos.z);
    glUniform3f(flashlightDirLocation, camForward.x, camForward.y, camForward.z);
    glUniform3f(flashlightColorLocation, 1.0f, 0.95f, 0.85f);
    glUniform1f(flashlightCutoffLocation, glm::cos(glm::radians(15.0f)));
    glUniform1f(flashlightOuterLocation, glm::cos(glm::radians(22.0f)));
    glUniform1f(flashlightIntensityLocation, 3.0f);
    glUniform1i(flashlightOnLocation, flashOn ? 1 : 0);
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
    float time)
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(objectTypeLocation, 3);
    glUniform1f(timeLocation, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.GetTextureID());
    glUniform1i(textureSamplerLocation, 0);

    model.Draw();
}

GLuint RenderManager::GetProgram() const { return shaderProgram; }