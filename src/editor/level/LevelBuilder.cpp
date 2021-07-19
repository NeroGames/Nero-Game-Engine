////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/level/LevelBuilder.h>
////////////////////////////////////////////////////////////
namespace nero
{
	GameLevelBuilder::GameLevelBuilder():
		 m_LevelSetting(std::make_shared<Setting>())
		,m_EngineSetting(nullptr)
		,m_SelectedWorldChunk(nullptr)
		,m_WorldChunkTable()
		,m_CountWorldChunk(0)
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

	WorldChunkBuilder::Ptr GameLevelBuilder::addWorldChunk()
	{
		WorldChunkBuilder::Ptr worldChunk = std::make_shared<WorldChunkBuilder>();
		worldChunk->setChunkId(++m_CountWorldChunk);
		worldChunk->setName(std::string("world chunk ") + toString(worldChunk->getChunkId()));
		worldChunk->setSelected(false);
		worldChunk->setLoadWithLevel(false);
		worldChunk->setVisible(true);

		m_WorldChunkTable.push_back(worldChunk);


		WorldBuilder::Ptr worldBuilder = std::make_shared<WorldBuilder>();

		worldBuilder->setRenderContext(m_RenderContext);
		worldBuilder->setRenderTexture(m_RenderTexture);
		worldBuilder->setResourceManager(m_ResourceManager);
		//worldBuilder->setLightManager(const LightManagerPtr& lightManager);

		//add first layer
		auto layer = worldBuilder->addLayer();
		worldBuilder->selectLayer(layer->getObjectId());

		worldChunk->setWorldBuilder(worldBuilder);

		setSelectedWorldChunk(m_WorldChunkTable.back());

		return m_WorldChunkTable.back();
	}

	std::vector<WorldChunkBuilder::Ptr>& GameLevelBuilder::getWorldChunkTable()
	{
		return m_WorldChunkTable;
	}

	WorldChunkBuilder::Ptr GameLevelBuilder::getSelectedWorldChunk()
	{
		return m_SelectedWorldChunk;
	}

	void GameLevelBuilder::setSelectedWorldChunk(WorldChunkBuilder::Ptr worldChunk)
	{
		m_SelectedWorldChunk = worldChunk;
	}

	void GameLevelBuilder::setRenderContext(const RenderContext::Ptr& renderContext)
	{
		m_RenderContext = renderContext;
	}

	void GameLevelBuilder::setRenderTexture(const std::shared_ptr<sf::RenderTexture>& renderTexture)
	{
		m_RenderTexture = renderTexture;
	}

	void GameLevelBuilder::saveGameLevel()
	{
		for(auto worldChunk : m_WorldChunkTable)
		{
			//update selections status
			m_SelectedWorldChunk->getChunkId() == worldChunk->getChunkId() ? worldChunk->setSelected(true) : worldChunk->setSelected(false);

			auto worldBuilder = worldChunk->getWorldBuilder();

			nlohmann::json save		= nlohmann::json::object();
			save["chunk_id"]		= worldChunk->getChunkId();
			save["name"]			= worldChunk->getName();
			save["selected"]		= worldChunk->isSelected();
			save["load_with_level"] = worldChunk->isLoadWithLevel();
			save["visible"]			= worldChunk->isVisible();
			save["world"]			= worldBuilder->saveScene();

			file::saveFile(file::getPath({m_LevelSetting->getString("level_directory"), "chunk", worldChunk->getName()}, StringPool.EXT_NERO), save.dump(3), true);
		}
	}

	void GameLevelBuilder::loadGameLevel()
	{

		std::experimental::filesystem::path folderPath(file::getPath({m_LevelSetting->getString("level_directory"), "chunk"}));

		std::experimental::filesystem::directory_iterator it{folderPath};
		while(it != std::experimental::filesystem::directory_iterator{})
		{
			loadWorldChunk(it->path().string());

			it++;
		}
	}

	void GameLevelBuilder::loadWorldChunk(const std::string& fileName)
	{
		nero_log(fileName);

		nlohmann::json save = file::loadJson(fileName, true);

		WorldChunkBuilder::Ptr worldChunk = std::make_shared<WorldChunkBuilder>();
		worldChunk->setChunkId(save["chunk_id"]);
		worldChunk->setName(save["name"]);
		worldChunk->setSelected(save["selected"]);
		worldChunk->setLoadWithLevel(save["load_with_level"]);
		worldChunk->setVisible(save["visible"]);

		WorldBuilder::Ptr worldBuilder = std::make_shared<WorldBuilder>();

		worldBuilder->setRenderContext(m_RenderContext);
		worldBuilder->setRenderTexture(m_RenderTexture);
		worldBuilder->setResourceManager(m_ResourceManager);

		//add first layer
		worldBuilder->loadScene(save["world"]);
		worldChunk->setWorldBuilder(worldBuilder);

		m_WorldChunkTable.push_back(worldChunk);

		if(worldChunk->isSelected())
		{
			setSelectedWorldChunk(worldChunk);
		}

		nero_log("loading chunk end");
	}
}

