#include "GameObject.h"

GameObject::GameObject(Primitive* primitive, int objectType)
    :
    primitive(primitive),
    objectType(objectType),
    position(0.0f),
    rotation(0.0f),
    scale(1.0f),
    visible(true)
{
}

void GameObject::Draw()
{
    if (!visible)
        return;

    primitive->Draw();
}

void GameObject::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

glm::vec3 GameObject::GetPosition() const
{
    return position;
}

void GameObject::SetRotation(const glm::vec3& rot)
{
    rotation = rot;
}

glm::vec3 GameObject::GetRotation() const
{
    return rotation;
}

void GameObject::SetScale(const glm::vec3& scl)
{
    scale = scl;
}

glm::vec3 GameObject::GetScale() const
{
    return scale;
}

void GameObject::SetVisible(bool state)
{
    visible = state;
}

bool GameObject::IsVisible() const
{
    return visible;
}

int GameObject::GetObjectType() const
{
    return objectType;
}