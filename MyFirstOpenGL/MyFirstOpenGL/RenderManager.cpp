#include "RenderManager.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

RenderManager::RenderManager()
    : shaderProgram(0),
    mvpLocation(-1),
    modelLocation(-1),
    objectTypeLocation(-1),
    timeLocation(-1),
    textureSamplerLocation(-1),
    normalMatrixLocation(-1),
    sunDirectionLoc(-1),
    moonDirectionLoc(-1),
    sunIntensityLoc(-1),
    moonIntensityLoc(-1),
    ambientColorLoc(-1),
    ambientStrengthLoc(-1),
    flashlightOnLoc(-1),
    flashlightPosLoc(-1),
    flashlightDirLoc(-1),
    flashlightInnerLoc(-1),
    flashlightOuterLoc(-1),
    flashlightRangeLoc(-1)
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

    // Base
    mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    objectTypeLocation = glGetUniformLocation(shaderProgram, "objectType");
    timeLocation = glGetUniformLocation(shaderProgram, "time");
    textureSamplerLocation = glGetUniformLocation(shaderProgram, "textureSampler");

    // HEAD: tint + normal matrix + sistema de luz clásico
    tint = glGetUniformLocation(shaderProgram, "tint");
    tintStrenght = glGetUniformLocation(shaderProgram, "tintStreght");
    normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");
    ambientLight.Initialize(shaderProgram);
    flashlight.Initialize(shaderProgram);

    // develop: día/noche
    sunDirectionLoc = glGetUniformLocation(shaderProgram, "sunDirection");
    moonDirectionLoc = glGetUniformLocation(shaderProgram, "moonDirection");
    sunIntensityLoc = glGetUniformLocation(shaderProgram, "sunIntensity");
    moonIntensityLoc = glGetUniformLocation(shaderProgram, "moonIntensity");
    ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
    ambientStrengthLoc = glGetUniformLocation(shaderProgram, "ambientStrength");

    // develop: linterna extendida
    flashlightOnLoc = glGetUniformLocation(shaderProgram, "flashlightOn");
    flashlightPosLoc = glGetUniformLocation(shaderProgram, "flashlightPos");
    flashlightDirLoc = glGetUniformLocation(shaderProgram, "flashlightDir");
    flashlightInnerLoc = glGetUniformLocation(shaderProgram, "flashlightInnerCutoff");
    flashlightOuterLoc = glGetUniformLocation(shaderProgram, "flashlightOuterCutoff");
    flashlightRangeLoc = glGetUniformLocation(shaderProgram, "flashlightRange");
}

void RenderManager::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ── HEAD: SetLight ──────────────────────────────────────────────────────────
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

Light& RenderManager::GetLight() { return ambientLight; }
Flashlight& RenderManager::GetFlashlight() { return flashlight; }

// ── develop: SetDayNightUniforms ────────────────────────────────────────────
void RenderManager::SetDayNightUniforms(
    const glm::vec3& sunDir, float sunIntensity,
    const glm::vec3& moonDir, float moonIntensity,
    const glm::vec3& ambColor, float ambStrength)
{
    glUseProgram(shaderProgram);
    glUniform3fv(sunDirectionLoc, 1, glm::value_ptr(sunDir));
    glUniform3fv(moonDirectionLoc, 1, glm::value_ptr(moonDir));
    glUniform1f(sunIntensityLoc, sunIntensity);
    glUniform1f(moonIntensityLoc, moonIntensity);
    glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambColor));
    glUniform1f(ambientStrengthLoc, ambStrength);
}

// ── develop: SetFlashlightUniforms ─────────────────────────────────────────
void RenderManager::SetFlashlightUniforms(
    bool on,
    const glm::vec3& pos,
    const glm::vec3& dir,
    float innerCutoff,
    float outerCutoff,
    float range)
{
    glUseProgram(shaderProgram);
    glUniform1i(flashlightOnLoc, on ? 1 : 0);
    glUniform3fv(flashlightPosLoc, 1, glm::value_ptr(pos));
    glUniform3fv(flashlightDirLoc, 1, glm::value_ptr(dir));
    glUniform1f(flashlightInnerLoc, innerCutoff);
    glUniform1f(flashlightOuterLoc, outerCutoff);
    glUniform1f(flashlightRangeLoc, range);
}

// ── Render Primitive ────────────────────────────────────────────────────────
void RenderManager::Render(
    const Primitive& primitive,
    const glm::mat4& mvp,
    const glm::mat4& model,
    int objectType, float time)
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(objectTypeLocation, objectType);
    glUniform1f(timeLocation, time);
    primitive.Draw();
}

// ── Render Model OBJ ───────────────────────────────────────────────────────
void RenderManager::Render(
    const Model& modelObj,
    const glm::mat4& mvpMat,
    const glm::mat4& modelMat,
    float            time,
    int              objectType,
    glm::vec4        tintColor,
    float            tintStrenghtValue)
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvpMat));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniform1i(objectTypeLocation, objectType);
    glUniform1f(timeLocation, time);
    glUniform4fv(tint, 1, glm::value_ptr(tintColor));
    glUniform1f(tintStrenght, tintStrenghtValue);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, modelObj.GetTextureID());
    glUniform1i(textureSamplerLocation, 0);
    modelObj.Draw();
}

GLuint RenderManager::GetProgram() const { return shaderProgram; }