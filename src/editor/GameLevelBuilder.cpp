////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/GameLevelBuilder.h>
////////////////////////////////////////////////////////////
namespace nero
{
	GameLevelBuilder::GameLevelBuilder():
		 m_LevelSetting(std::make_shared<Setting>())
		,m_EngineSetting(nullptr)
		,m_SelectedWorldChunk(nullptr)
		,m_WorldChunkTable()
		,m_CountWorldChunk(-1)
	{

	}

	void GameLevelBuilder::init()
	{
		m_ResourceManager = std::make_shared<ResourceManager>(m_EngineSetting->getSetting("resource"));
		m_ResourceManager->loadDirectory(m_LevelSetting->getString("resource_directory"));
	}

	ResourceManager::Ptr GameLevelBuilder::getResourceManager()
	{
		return m_ResourceManager;
	}

	void GameLevelBuilder::setEngineSetting(const Setting::Ptr& setting)
	{
		m_EngineSetting = setting;
	}

	std::string GameLevelBuilder::getLevelName()
	{
		return m_LevelSetting->getString("level_name");
	}

	Setting::Ptr GameLevelBuilder::getLevelSetting()
	{
		return m_LevelSetting;
	}

	std::string GameLevelBuilder::getResourceFoler()
	{
		return m_LevelSetting->getString("resource_directory");
	}

	void GameLevelBuilder::addWorldChunk()
	{
		auto worldChunk = std::make_shared<WorldChunk>();
		worldChunk->m_ChunkId		= ++m_CountWorldChunk;
		worldChunk->m_ChunkName		= std::string("world chunk ") + toString(worldChunk->m_ChunkId);
		worldChunk->m_Selected		= false;
		worldChunk->m_LoadWithLevel	= false;

		m_WorldChunkTable.push_back(worldChunk);
	}

	std::vector<WorldChunk::Ptr>& GameLevelBuilder::getWorldChunkTable()
	{
		return m_WorldChunkTable;
	}

	WorldChunk::Ptr GameLevelBuilder::getSelectedWorldChunk()
	{
		return m_SelectedWorldChunk;
	}

	void GameLevelBuilder::setSelectedWorldChunk(WorldChunk::Ptr worldChunk)
	{
		m_SelectedWorldChunk = worldChunk;
	}
}

