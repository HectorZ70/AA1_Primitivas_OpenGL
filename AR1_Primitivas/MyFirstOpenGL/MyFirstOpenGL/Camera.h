#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Transform.h"

class Camera
{
private:
    Transform transform;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    glm::vec3 target;

public:
    Camera(float fov, float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f);

    Transform& GetTransform();
    const Transform& GetTransform() const;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    void  SetFOV(float fov);
    float GetFOV() const;
    void  SetAspectRatio(float ratio);
    float GetAspectRatio() const;
    void SetTarget(const glm::vec3& t);
    glm::vec3 GetTarget() const;
};