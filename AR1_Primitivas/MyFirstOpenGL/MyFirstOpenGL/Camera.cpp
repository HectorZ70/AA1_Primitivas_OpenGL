#include "Camera.h"

Camera::Camera(float fov, float aspectRatio,
    float nearPlane, float farPlane)
    : fov(fov), aspectRatio(aspectRatio),
    nearPlane(nearPlane), farPlane(farPlane)
{
}

Transform& Camera::GetTransform() 
{ 
    return transform; 
}

const Transform& Camera::GetTransform() const 
{ 
    return transform; 
}

glm::mat4 Camera::GetViewMatrix() const
{
    glm::vec3 pos = transform.GetPosition();
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    return glm::lookAt(pos, target, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(
        glm::radians(fov),
        aspectRatio,
        nearPlane,
        farPlane
    );
}

void  Camera::SetFOV(float f) 
{ 
    fov = f; 
}

float Camera::GetFOV() const 
{ 
    return fov; 
}

void  Camera::SetAspectRatio(float r) 
{ 
    aspectRatio = r; 
}

float Camera::GetAspectRatio() const 
{ 
    return aspectRatio; 
}

void Camera::SetTarget(const glm::vec3& t)
{
    target = t;
}

glm::vec3 Camera::GetTarget() const
{
    return target;
}
