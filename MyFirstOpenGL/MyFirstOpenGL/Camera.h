#pragma once 

#include "GameObject.h"

class Camera : public GameObject
{
private:
	float fFov;
	float aspectRatio;
	glm::mat4 viewMatrix;
	glm::mat4 proMatrix;
	glm::vec3 localVectorUp;

public:
	Camera();
	void SetFOV(const float& fov);
	float GetFOV() const;
	void SetAspectRatio(const float& aspectRatio);
	float GetAspectRatio() const;
	//void SetLocalVectorUp(const glm::vec3& localVectorUp);
	//glm::vec3 GetLocalVectorUp() const;
	void SetViewMatrix(const glm::mat4& viewMatrix);
	glm::mat4 GetViewMatrix() const; 
	void SetProyectionMatrix(const glm::mat4& proMatrix);
	glm::mat4 GetProyectionMatrix() const;
};