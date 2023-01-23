////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/LayerActionObject.h>
////////////////////////////////////////////////////////////
namespace nero
{
    LayerActionObject::LayerActionObject()
        : m_LayerObject(nullptr)
        , m_ActionTable()
        , m_TimeStep(EngineConstant.TIME_PER_FRAME)
    {
    }

    void LayerActionObject::setObject(Object::Ptr object)
    {
        m_LayerObject = LayerObject::Cast(object);
    }

    void LayerActionObject::dropAction(sf::String name)
    {
        m_ActionTable.erase(name);
    }

    void LayerActionObject::callAction(sf::String name)
    {
        m_ActionTable[name]->call(m_LayerObject, m_TimeStep);
    }

    LayerObject::Ptr LayerActionObject::getObject()
    {
        return m_LayerObject;
    }

    void LayerActionObject::updateObject(sf::Time time_step)
    {
        m_TimeStep = time_step;
    }
} // namespace nero
