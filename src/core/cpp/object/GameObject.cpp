////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/object/GameObject.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    GameObject::GameObject()
    {
    }

    GameObject::~GameObject()
    {
        destroyObject();
    }

    void GameObject::destroyObject()
    {
        GenericObject::destroyObject();
    }
} // namespace nero
