////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/PhysicActionObject.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicActionObject::PhysicActionObject():
        m_PhysicObject(nullptr)
        ,m_ActionTable()
        ,m_TimeStep(TIME_PER_FRAME)
    {

    }

    void PhysicActionObject::setObject(Object::Ptr object)
    {
        m_PhysicObject = PhysicObject::Cast(object);
    }

    void PhysicActionObject::dropAction(sf::String name)
    {
        m_ActionTable.erase(name);
    }

    void PhysicActionObject::callAction(sf::String name)
    {
        m_ActionTable[name]->call(m_PhysicObject, m_TimeStep);
    }

    PhysicObject::Ptr  PhysicActionObject::getObject()
    {
        return m_PhysicObject;
    }

    void PhysicActionObject::updateObject(sf::Time time_step)
    {
        m_TimeStep = time_step;
    }
}


