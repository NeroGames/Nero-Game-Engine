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
			WorldChunk::Ptr						addWorldChunk();
			std::vector<WorldChunk::Ptr>&		getWorldChunkTable();
			WorldChunk::Ptr						getSelectedWorldChunk();
			void								setSelectedWorldChunk(WorldChunk::Ptr worldChunk);
			void								setRenderContext(const RenderContext::Ptr& renderContext);
			void								setRenderTexture(const std::shared_ptr<sf::RenderTexture>& renderTexture);

		private:
			Setting::Ptr						m_LevelSetting;
			Setting::Ptr						m_EngineSetting;
			ResourceManager::Ptr				m_ResourceManager;
			//world chunk
			WorldChunk::Ptr						m_SelectedWorldChunk;
			std::vector<WorldChunk::Ptr>		m_WorldChunkTable;
			int									m_CountWorldChunk;

			RenderContext::Ptr								m_RenderContext;
			std::shared_ptr<sf::RenderTexture>				m_RenderTexture;
	};
}


#endif // GAMELEVELBUILDER_H
