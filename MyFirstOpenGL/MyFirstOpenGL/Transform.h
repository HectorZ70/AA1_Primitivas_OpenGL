#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Transform
{
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    Transform();

    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::vec3& rot);
    void SetScale(const glm::vec3& scl);

    void SetWiden(const float scl);

    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale()    const;

    glm::mat4 GetModelMatrix() const;
};