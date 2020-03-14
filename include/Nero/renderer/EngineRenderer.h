////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef ENGINERENDERER_H
#define ENGINERENDERER_H
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/engine/CoreEngine.h>
/////////////////////////////////////////////////////////////
namespace  nero
{
	class EngineRenderer : public CoreEngine
	{
		public:
									EngineRenderer();
			virtual                ~EngineRenderer() override;
		private:
			//game loop
			void                    handleEvent()                    override;
			void                    update(const sf::Time& timeStep) override;
			void                    render()                         override;
	};

}

#endif // ENGINERENDERER_H
