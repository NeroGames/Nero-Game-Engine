////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMELEVELBUILDER_H
#define GAMELEVELBUILDER_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/resource/ResourceManager.h>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
	class GameLevelBuilder
	{
		public:
			typedef std::shared_ptr<GameLevelBuilder> Ptr;

		public:
												GameLevelBuilder();

			ResourceManager::Ptr				getResourceManager();
			void								setEngineSetting(const Setting::Ptr& setting);

		private:
			Setting::Ptr						m_LevelSetting;
			Setting::Ptr						m_EngineSetting;
			ResourceManager::Ptr				m_ResourceManager;
	};
}


#endif // GAMELEVELBUILDER_H
