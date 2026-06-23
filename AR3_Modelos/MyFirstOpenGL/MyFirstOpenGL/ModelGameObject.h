#pragma once
#include "Model.h"
#include "Transform.h"

class ModelGameObject
{
private:
    Model* model;
    Transform transform;
    bool visible;

public:
    ModelGameObject(Model* model);

    Transform& GetTransform();
    const Transform& GetTransform() const;
    const Model* GetModel() const;

    void SetVisible(bool state);
    bool IsVisible() const;
};
