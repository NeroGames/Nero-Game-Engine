////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMELEVELOBJECT_H
#define GAMELEVELOBJECT_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/engine/Parameter.h>
#include <Box2D/Dynamics/b2World.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	class GameLevelObject : public Object
	{
		public:
			typedef std::shared_ptr<GameLevelObject>    Ptr;

		public:
								GameLevelObject();
			void				init(const Parameter& parameter);
			b2World*			getPhysicWorld();


		private:
			b2World*			m_PhysicWorld;
			Parameter			m_Parameter;
	};

}

#endif // GAMELEVELOBJECT_H
