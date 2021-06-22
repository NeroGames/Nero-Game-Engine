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
	{

	}

	void GameLevelBuilder::init()
	{
		m_ResourceManager = std::make_shared<ResourceManager>(m_EngineSetting->getSetting("resource"));
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
		m_LevelSetting->getString("level_name");
	}

	Setting::Ptr GameLevelBuilder::getLevelSetting()
	{
		return m_LevelSetting;
	}
}

