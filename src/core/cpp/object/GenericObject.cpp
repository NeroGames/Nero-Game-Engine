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
    GenericObject::GenericObject()
    {
    }

    GenericObject::~GenericObject()
    {
        destroyObject();
    }

    void GenericObject::destroyObject()
    {
        // m_Parent simply refers to the parent, it does not hold the parent, therefore we do not
        // call delete(m_Parent)
        m_Parent = nullptr;
    }
} // namespace nero
