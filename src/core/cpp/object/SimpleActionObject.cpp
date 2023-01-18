////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/SimpleActionObject.h>
#include <Nero/core/cpp/utility/Utility.h>
// STD
#include <iostream>
////////////////////////////////////////////////////////////
namespace nero
{
    SimpleActionObject::SimpleActionObject()
        : m_Object(nullptr)
        , m_ActionTable()
        , m_TimeStep(EngineConstant.TIME_PER_FRAME)
    {
    }

    void SimpleActionObject::setObject(Object::Ptr object)
    {
        m_Object = object;
    }

    void SimpleActionObject::dropAction(sf::String name)
    {
        m_ActionTable.erase(name);
    }

    void SimpleActionObject::callAction(sf::String name)
    {
        m_ActionTable[name]->call(m_Object, m_TimeStep);
    }

    Object::Ptr SimpleActionObject::getObject()
    {
        return m_Object;
    }

    void SimpleActionObject::updateObject(sf::Time time_step)
    {
        m_TimeStep = time_step;
    }
} // namespace nero
