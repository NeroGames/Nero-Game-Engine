////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// NERO
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
