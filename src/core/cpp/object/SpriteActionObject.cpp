////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/SpriteActionObject.h>
////////////////////////////////////////////////////////////
namespace nero
{
    SpriteActionObject::SpriteActionObject()
        : m_SpriteObject(nullptr)
        , m_ActionTable()
        , m_TimeStep(EngineConstant::TIME_PER_FRAME)
    {
    }

    void SpriteActionObject::setObject(Object::Ptr object)
    {
        m_SpriteObject = SpriteObject::Cast(object);
    }

    void SpriteActionObject::dropAction(sf::String name)
    {
        m_ActionTable.erase(name);
    }

    void SpriteActionObject::callAction(sf::String name)
    {
        m_ActionTable[name]->call(m_SpriteObject, m_TimeStep);
    }

    SpriteObject::Ptr SpriteActionObject::getObject()
    {
        return m_SpriteObject;
    }

    void SpriteActionObject::updateObject(sf::Time time_step)
    {
        m_TimeStep = time_step;
    }
} // namespace nero
