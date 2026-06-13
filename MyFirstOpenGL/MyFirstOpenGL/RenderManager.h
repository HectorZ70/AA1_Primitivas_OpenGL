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

    // Light uniform locations
    GLint modelLocation;
    GLint normalMatrixLocation;
    GLint ambientColorLocation;
    GLint ambientIntensityLocation;
    GLint flashlightPosLocation;
    GLint flashlightDirLocation;
    GLint flashlightColorLocation;
    GLint flashlightCutoffLocation;
    GLint flashlightOuterLocation;
    GLint flashlightIntensityLocation;
    GLint flashlightOnLocation;

public:
    RenderManager();

    void Initialize(
        const char* vertexShaderPath,
        const char* geometryShaderPath,
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

    void SetLight(const glm::mat4& modelMatrix,
        const glm::vec3& camPos,
        const glm::vec3& camForward,
        bool  flashOn);

    GLuint GetProgram() const;
};
