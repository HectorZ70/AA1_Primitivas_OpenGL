#include "Transform.h"

Transform::Transform()
    : position(0.0f), rotation(0.0f), scale(1.0f)
{
}

void Transform::SetPosition(const glm::vec3& pos) { position = pos; }
void Transform::SetRotation(const glm::vec3& rot) { rotation = rot; }
void Transform::SetScale(const glm::vec3& scl) { scale = scl; }
void Transform::SetWiden(const float scl) { scale.x = scl; }

glm::vec3 Transform::GetPosition() const { return position; }
glm::vec3 Transform::GetRotation() const { return rotation; }
glm::vec3 Transform::GetScale()    const { return scale; }

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);
    return model;
}