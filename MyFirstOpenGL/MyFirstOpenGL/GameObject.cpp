#include "GameObject.h"

GameObject::GameObject(Primitive* primitive, int objectType)
    : primitive(primitive), objectType(objectType), visible(true)
{
}

Transform& GameObject::GetTransform() { return transform; }
const Transform& GameObject::GetTransform() const { return transform; }

const Primitive* GameObject::GetPrimitive() const { return primitive; }

void GameObject::SetVisible(bool state) { visible = state; }
bool GameObject::IsVisible()    const { return visible; }
int  GameObject::GetObjectType() const { return objectType; }