////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/editor/project/AdvancedScene.h>
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
	AdvancedScene::AdvancedScene():
		 m_SelectedLevel(nullptr)
		,m_RegisteredLevelTable()
		,m_SelectedScreen(nullptr)
		,m_EditorSetting(nullptr)
		,m_ProjectSetting(nullptr)
		,m_RenderContext(nullptr)
		,m_RenderTexture(nullptr)
	{

	}

	void AdvancedScene::init()
	{
		Setting sceneSetting;
		sceneSetting.loadSetting(file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"}, StringPool.EXT_NERO), true, true);
		m_RegisteredLevelTable = sceneSetting.getStringTable("level_table");
	}

	void AdvancedScene::createLevel(const Parameter& parameter)
	{
		//generate paratemers
		std::string level_name		= string::trim(parameter.getString("level_name"));
		std::string level_id		= string::formatString(level_name, string::Format::COMPACT_LOWER);
		std::string class_name		= string::formatString(level_name, string::Format::CAMEL_CASE_UPPER) + "GameLevel";
		std::string header_gard		= string::formatString(level_name, string::Format::COMPACT_UPPER) + "_H";

		//generate source file
			//paremeter
		std::string header			= file::loadText("template/cpp_project/CppGameLevel.h");
		std::string source			= file::loadText("template/cpp_project/CppGameLevel.cpp");
			//file 1 : header
		boost::algorithm::replace_all(header, "::GameLevelClass::",		class_name);
		boost::algorithm::replace_all(header, "::HeaderGard::",			header_gard);
		boost::algorithm::replace_all(header, "::Namespace::",			m_ProjectSetting->getString("project_namespace"));
		boost::algorithm::replace_all(header, "::ProjectName::",		m_ProjectSetting->getString("project_name"));
		boost::algorithm::replace_all(header, "::ProjectLead::",		m_ProjectSetting->getString("project_lead"));
		boost::algorithm::replace_all(header, "::CoyprightDate::",		toString(datetime::getCurrentDateTime().date().year()));
			//file 2 : source
		boost::algorithm::replace_all(source, "::GameLevelClass::",		class_name);
		boost::algorithm::replace_all(source, "::Namespace::",			m_ProjectSetting->getString("project_namespace"));
		boost::algorithm::replace_all(source, "::ProjectName::",		m_ProjectSetting->getString("project_name"));
		boost::algorithm::replace_all(source, "::ProjectLead::",		m_ProjectSetting->getString("project_lead"));
		boost::algorithm::replace_all(source, "::CoyprightDate::",		toString(datetime::getCurrentDateTime().date().year()));
			//save file
		file::saveFile(file::getPath({m_ProjectSetting->getString("source_directory"),"cpp", "level", class_name}, StringPool.EXT_H), header);
		file::saveFile(file::getPath({m_ProjectSetting->getString("source_directory"), "cpp", "level", class_name}, StringPool.EXT_CPP), source);

		//generate level directory
			//directory
		std::string level_directory = file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "level", boost::algorithm::to_lower_copy(level_name)});
		file::createDirectory(level_directory);
		file::createDirectory(file::getPath({level_directory, "chunk"}));
		file::createDirectory(file::getPath({level_directory, "resource"}));
		ResourceManager::buildDirectory(file::getPath({level_directory, "resource"}));
			//document
		Parameter document;
		document.setString("creation_date", datetime::formatDateTime(datetime::getCurrentDateTime()));
		document.setString("level_name", level_name);
		document.setString("level_id", level_id);
		document.setString("template", parameter.getString("template"));
		file::saveFile(file::getPath({level_directory, "level"}, StringPool.EXT_NERO), document.toString());
			//setting
		Setting setting;
		setting.setString("level_name", level_name);
		setting.setString("level_id", level_id);
		setting.setBool("enable_physics", parameter.getBool("enable_physics"));
		setting.setBool("enable_light",  parameter.getBool("enable_light"));
		setting.setInt("chunk_count", 0);
		setting.setBool("opened", false);
		setting.setString("level_directory",  level_directory);
		setting.setString("resource_directory",  file::getPath({level_directory, "resource"}));
		setting.setString("chunk_directory",  file::getPath({level_directory, "chunk"}));
		file::saveFile(file::getPath({level_directory, "setting"}, StringPool.EXT_NERO), setting.toString());

		registerLevel(level_name);
	}

	LevelBuilder::Ptr AdvancedScene::openLevel(const std::string& levelName)
	{
		if(levelName == StringPool.BLANK) return nullptr;

		if(m_SelectedLevel)
		{
			closeSelectedLevel();
		}

		std::string level_directory = file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "level", boost::algorithm::to_lower_copy(levelName)});

		//create level builder
		m_SelectedLevel = std::make_shared<LevelBuilder>();
		m_SelectedLevel->setEditorSetting(m_EditorSetting);
		m_SelectedLevel->getLevelSetting()->loadSetting(file::getPath({level_directory, "setting"}, StringPool.EXT_NERO), true, true);
		m_SelectedLevel->setRenderContext(m_RenderContext);
		m_SelectedLevel->setRenderTexture(m_RenderTexture);
		m_SelectedLevel->loadResource();

		if(file::directoryEmpty(file::getPath({level_directory, "chunk"})))
		{
			auto worldChunk = m_SelectedLevel->addChunk();
			m_SelectedLevel->setSelectedChunk(worldChunk);

			m_SelectedLevel->saveGameLevel();
		}
		else
		{
			m_SelectedLevel->loadGameLevel();
		}

		return m_SelectedLevel;
	}

	void AdvancedScene::closeSelectedLevel()
	{
		//TODO close selected level
	}

	void AdvancedScene::removeLevel(const std::string& levelName)
	{
		//TODO remove level
	}

	void AdvancedScene::registerLevel(const std::string& levelName)
	{
		//register level
		m_RegisteredLevelTable.push_back(levelName);

		Setting scene_document;
		scene_document.loadSetting(file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"}, StringPool.EXT_NERO), true, true);
		scene_document.setStringTable("level_table", m_RegisteredLevelTable);
		file::saveFile(file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"}, StringPool.EXT_NERO), scene_document.toString(), true);
	}

	void AdvancedScene::unregisterLevel(const std::string& levelName)
	{
		auto it = std::find(m_RegisteredLevelTable.begin(), m_RegisteredLevelTable.end(), levelName);

		if(it != m_RegisteredLevelTable.end())
			m_RegisteredLevelTable.erase(it);

		Setting scene_document;
		scene_document.loadSetting(file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"}, StringPool.EXT_NERO), true, true);
		scene_document.setStringTable("level_table", m_RegisteredLevelTable);
		file::saveFile(file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"}, StringPool.EXT_NERO), scene_document.toString(), true);
	}

	void AdvancedScene::createScreen(const Parameter& parameter)
	{
		/*auto gameScreenBuilder = std::make_shared<GameScreenBuilder>();

		return gameScreenBuilder;*/
	}

	void AdvancedScene::setEditorSetting(const Setting::Ptr &setting)
	{
		m_EditorSetting = setting;
	}

	void AdvancedScene::setProjectSetting(const Setting::Ptr& setting)
	{
		m_ProjectSetting = setting;
	}

	std::vector<std::string> AdvancedScene::getRegisteredLevelTable()
	{
		return m_RegisteredLevelTable;
	}

	void AdvancedScene::setRenderContext(const RenderContext::Ptr& renderContext)
	{
		m_RenderContext = renderContext;
	}

	void AdvancedScene::setRenderTexture(const std::shared_ptr<sf::RenderTexture>& renderTexture)
	{
		m_RenderTexture = renderTexture;
	}
}
