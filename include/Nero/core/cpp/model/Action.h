////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ACTION_H
#define ACTION_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicsObject.h>
#include <Nero/core/cpp/object/SpriteObject.h>
#include <Nero/core/cpp/object/LayerObject.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    struct ObjectAction
    {
        typedef std::unique_ptr<ObjectAction> Ptr;
        virtual void call(Object::Ptr object, sf::Time time_step = EngineConstant.TIME_PER_FRAME){};
    };

    struct LayerAction
    {
        typedef std::unique_ptr<LayerAction> Ptr;
        virtual void                         call(LayerObject::Ptr object,
                                                  sf::Time         time_step = EngineConstant.TIME_PER_FRAME){};
    };

    struct SpriteAction
    {
        typedef std::unique_ptr<SpriteAction> Ptr;
        virtual void                          call(SpriteObject::Ptr object,
                                                   sf::Time          time_step = EngineConstant.TIME_PER_FRAME){};
    };

    struct PhysicAction
    {
        typedef std::unique_ptr<PhysicAction> Ptr;
        virtual void                          call(PhysicsObject::Ptr object,
                                                   sf::Time          time_step = EngineConstant.TIME_PER_FRAME){};
    };
} // namespace nero
#endif // ACTION_H
