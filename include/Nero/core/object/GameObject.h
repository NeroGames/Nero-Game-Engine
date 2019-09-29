////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/object/GenericObject.h>
//STD
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GameObject : public GenericObject
    {
        public: //Types Definiton
            typedef std::shared_ptr<GameObject> Ptr;

        public: //Constructor
                                                GameObject();
            virtual                            ~GameObject();

            virtual void                        destroyObject();
    };

}

#endif // GAMEOBJECT_H
