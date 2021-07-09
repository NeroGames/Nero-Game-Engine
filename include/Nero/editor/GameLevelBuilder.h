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
#include <Nero/core/cpp/scene/WorldChunk.h>
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

			void								init();
			ResourceManager::Ptr				getResourceManager();
			void								setEngineSetting(const Setting::Ptr& setting);
			std::string							getLevelName();
			Setting::Ptr						getLevelSetting();
			std::string							getResourceFoler();
			//world chunk
			void								addWorldChunk();
			std::vector<WorldChunk::Ptr>&		getWorldChunkTable();
			WorldChunk::Ptr						getSelectedWorldChunk();
			void								setSelectedWorldChunk(WorldChunk::Ptr worldChunk);


		private:
			Setting::Ptr						m_LevelSetting;
			Setting::Ptr						m_EngineSetting;
			ResourceManager::Ptr				m_ResourceManager;
			//world chunk
			WorldChunk::Ptr						m_SelectedWorldChunk;
			std::vector<WorldChunk::Ptr>		m_WorldChunkTable;
			int									m_CountWorldChunk;
	};
}


#endif // GAMELEVELBUILDER_H
