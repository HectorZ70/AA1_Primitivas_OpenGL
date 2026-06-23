#pragma once
#include "Primitive.h"
#include "Transform.h"

class GameObject
{
private:
    Primitive* primitive;
    Transform  transform;
    bool visible;
    int objectType;

public:
    GameObject(Primitive* primitive, int objectType);

    Transform& GetTransform();
    const Transform& GetTransform() const;
    const Primitive* GetPrimitive() const;

    void SetVisible(bool state);
    bool IsVisible() const;
    int  GetObjectType() const;
};

