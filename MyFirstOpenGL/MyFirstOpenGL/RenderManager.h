#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "Primitive.h"
#include "Model.h"
#include "Utils.h"
#include "Light.h"
#include "Flashlight.h"

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

    // Model/normal matrix uniform locations (used for lighting)
    GLint modelLocation;
    GLint normalMatrixLocation;

    Light ambientLight;
    Flashlight flashlight;

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
        float            time,
        int objectType,
        glm::vec4        tintColor = glm::vec4(1.0f),
        float            tintStrenghtValue = 0.0f
    );

    void SetLight(const glm::mat4& modelMatrix,
        const glm::vec3& camPos,
        const glm::vec3& camForward,
        bool  flashOn);

    Light& GetLight();
    Flashlight& GetFlashlight();

    GLuint GetProgram() const;
};
