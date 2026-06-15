#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "Primitive.h"
#include "Model.h"
#include "Utils.h"
#include "Light.h"
#include "Flashlight.h"

class RenderManager {
private:
    GLuint shaderProgram;
    GLint  mvpLocation;
    GLint  objectTypeLocation;
    GLint  timeLocation;
    GLint  textureSamplerLocation;
    GLuint tint;
    GLuint tintStrenght;

    // Lighting uniforms
    GLint  modelLocation;
    GLint  normalMatrixLocation;
    Light      ambientLight;
    Flashlight flashlight;

    GLint mvpLocation;
    GLint modelLocation;
    GLint objectTypeLocation;
    GLint timeLocation;
    GLint textureSamplerLocation;

    // Dia/noche
    GLint sunDirectionLoc;
    GLint moonDirectionLoc;
    GLint sunIntensityLoc;
    GLint moonIntensityLoc;
    GLint ambientColorLoc;
    GLint ambientStrengthLoc;

    // Linterna
    GLint flashlightOnLoc;
    GLint flashlightPosLoc;
    GLint flashlightDirLoc;
    GLint flashlightInnerLoc;
    GLint flashlightOuterLoc;
    GLint flashlightRangeLoc;

public:
    RenderManager();
    void Initialize(const char* vertexShaderPath,
        const char* fragmentShaderPath);
    void Clear();

    void Initialize(
        const char* vertexShaderPath,
        const char* geometryShaderPath,
        const char* fragmentShaderPath
    );
    void SetDayNightUniforms(
        const glm::vec3& sunDir, float sunIntensity,
        const glm::vec3& moonDir, float moonIntensity,
        const glm::vec3& ambColor, float ambStrength);

    void SetFlashlightUniforms(
        bool on,
        const glm::vec3& pos,
        const glm::vec3& dir,
        float innerCutoff,
        float outerCutoff,
        float range);

    void Render(const Primitive& primitive,
        const glm::mat4& mvp,
        const glm::mat4& model,
        int objectType, float time);

    // Render Model
    void Render(
        const Model& model,
        const glm::mat4& mvp,
        float            time,
        int              objectType,
        glm::vec4        tintColor = glm::vec4(1.0f),
        float            tintStrenghtValue = 0.0f
    );
    void Render(const Model& model,
        const glm::mat4& mvpMat,
        const glm::mat4& modelMat,
        float time);

    // Lighting
    void SetLight(
        const glm::mat4& modelMatrix,
        const glm::vec3& camPos,
        const glm::vec3& camForward,
        bool             flashOn
    );

    Light& GetLight();
    Flashlight& GetFlashlight();

    GLuint GetProgram() const;
};