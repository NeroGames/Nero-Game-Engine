////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/object/GameLevelObject.h>
/////////////////////////////////////////////////////////////

namespace nero
{
	GameLevelObject::GameLevelObject():
		m_PhysicWorld(nullptr)
	{

	}

	void GameLevelObject::initialize(const Setting& parameter)
	{
		m_PhysicWorld = new b2World(b2Vec2(0.f, 9.8f));
	}

	b2World* GameLevelObject::getPhysicWorld()
	{
		return m_PhysicWorld;
	}
}

