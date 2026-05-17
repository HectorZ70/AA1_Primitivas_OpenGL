#pragma once
#include "Primitive.h"
#include "Transform.h"

class GameObject
{
private:
    Primitive* primitive;
    Transform  transform;
    bool       visible;
    int        objectType;

public:
    GameObject(Primitive* primitive, int objectType);

    // main.cpp anima via Transform
    Transform& GetTransform();
    const Transform& GetTransform() const;

    // RenderManager accede a Primitive directamente
    const Primitive* GetPrimitive() const;

    void SetVisible(bool state);
    bool IsVisible()     const;
    int  GetObjectType() const;
};

