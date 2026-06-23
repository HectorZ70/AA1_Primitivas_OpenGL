#include "ModelGameObject.h"

ModelGameObject::ModelGameObject(Model* model)
    : model(model), visible(true)
{
}

Transform& ModelGameObject::GetTransform() 
{ 
    return transform; 
}

const Transform& ModelGameObject::GetTransform() const 
{ 
    return transform; 
}

const Model* ModelGameObject::GetModel() const 
{ 
    return model; 
}

void ModelGameObject::SetVisible(bool state) 
{ 
    visible = state; 
}

bool ModelGameObject::IsVisible() const 
{ 
    return visible; 
}