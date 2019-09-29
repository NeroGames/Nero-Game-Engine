////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/object/GameObject.h>
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
         //m_Parent simply refers to the parent, it does not hold the parent, therefore we do not call delete(m_Parent)
         m_Parent = nullptr;
     }
}
