////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/object/GenericObject.h>
// STD
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GameObject : public GenericObject
    {
      public: // Types Definiton
        typedef std::shared_ptr<GameObject> Ptr;

      public: // Constructor
        GameObject();
        virtual ~GameObject();

        virtual void destroyObject();
    };

} // namespace nero

#endif // GAMEOBJECT_H
