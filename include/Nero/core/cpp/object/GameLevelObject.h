////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMELEVELOBJECT_H
#define GAMELEVELOBJECT_H
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Box2D/Dynamics/b2World.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	class GameLevelObject : public Object
	{
		public:
								GameLevelObject();
			void				initialize(const Setting& parameter);
			b2World*			getPhysicWorld();


		private:
			b2World*			m_PhysicWorld;
			Setting				m_Parameter;
	};

}

#endif // GAMELEVELOBJECT_H
