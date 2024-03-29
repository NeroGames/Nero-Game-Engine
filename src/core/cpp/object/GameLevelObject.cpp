////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/object/GameLevelObject.h>
/////////////////////////////////////////////////////////////

namespace nero
{
    GameLevelObject::GameLevelObject()
        : Object()
        , m_PhysicWorld(nullptr)
    {
    }

    void GameLevelObject::init(const Parameter& parameter)
    {
        m_Parameter = parameter;
        // m_PhysicWorld = new b2World(b2Vec2(0.f, 9.8f));
    }

    std::shared_ptr<b2World> GameLevelObject::getPhysicWorld()
    {
        return m_PhysicWorld;
    }
} // namespace nero
