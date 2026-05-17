#include "Camera.h"

Camera::Camera() 
    : 
	GameObject(),
    fFov(70), 
    aspectRatio(), 
    viewMatrix(),
    proMatrix(),
    localVectorUp()
{
}

void Camera::SetFOV(const float& fov)
{
    fFov = fov;
}

float Camera::GetFOV() const
{
    return fFov;
}

void Camera::SetAspectRatio(const float& aRatio)
{
    aspectRatio = aRatio;
}

float Camera::GetAspectRatio() const
{
    return aspectRatio;
}
/*
void Camera::SetLocalVectorUp(const glm::vec3& localVectorUp)
{
    lVectorUp = localVectorUp;
}

glm::vec3 Camera::GetLocalVectorUp() const
{
    return lVectorUp;
}
*/

void Camera::SetViewMatrix(const glm::mat4& vMatrix) {
    viewMatrix = vMatrix;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return viewMatrix;
}

void Camera::SetProyectionMatrix(const glm::mat4& pMatrix) {
    proMatrix = pMatrix;
}

glm::mat4 Camera::GetProyectionMatrix() const
{
    return proMatrix;
}
