#pragma once

#include <glm.hpp>
#include "Primitive.h"

class GameObject
{
private:

    Primitive* primitive;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool visible;

    int objectType;

public:

    GameObject(Primitive* primitive, int objectType);

    void Draw();

    void SetPosition(const glm::vec3& pos);
    glm::vec3 GetPosition() const;

    void SetRotation(const glm::vec3& rot);
    glm::vec3 GetRotation() const;

    void SetScale(const glm::vec3& scl);
    glm::vec3 GetScale() const;

    void SetVisible(bool state);
    bool IsVisible() const;

    int GetObjectType() const;
};

