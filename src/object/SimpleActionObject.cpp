////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#include <NERO/object/SimpleActionObject.h>

#include <iostream>
#include <NERO/Utility.h>

namespace nero
{
    SimpleActionObject::SimpleActionObject():
        m_Object(nullptr),
        m_ActionTable(),
        m_TimeStep(TIME_PER_FRAME)
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

    Object::Ptr  SimpleActionObject::getObject()
    {
        return m_Object;
    }

    void SimpleActionObject::updateObject(sf::Time time_step)
    {
        m_TimeStep = time_step;
    }
}

