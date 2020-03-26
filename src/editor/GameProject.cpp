////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/editor/GameProject.h>
#include <Nero/core/cpp/utility/FileUtil.h>
#include <Nero/core/cpp/utility/DateUtil.h>
#include <boost/dll.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
/////////////////////////////////////////////////////////////

namespace nero
{
	GameProject::GameProject()
    {

    }

	void GameProject::loadResource(const Setting& parameter)
	{
		/*m_ResourceManager = ResourceManager::Ptr(new ResourceManager(m_ProjectSetting->getSetting("resource")));

		if(parameter.getBool("load_startup_package"))
		{
			m_ResourceManager->loadDirectory("resource/starterpack");
		}

		m_ResourceManager->loadDirectory("project/path/resource");*/

	}

	void GameProject::init(const Setting& parameter)
    {
		m_ProjectParameter = parameter;
		m_ProjectParameter.setString("source_directory", getPath({m_ProjectParameter.getString("project_directory"), "Source", parameter.getString("project_name")}));
		m_ProjectParameter.setString("build_directory", getPath({m_ProjectParameter.getString("project_directory"), "Build"}));
		m_ProjectParameter.setString("cmake_file", getPath({m_ProjectParameter.getString("source_directory"), "CMakeLists"}, StringPool.EXTENSION_TEXT));

		std::string cleanProject		= "mingw32-make -C \"" + m_ProjectParameter.getString("build_directory") + "\" -k clean";
		std::string configureProject	= "cmake -G \"MinGW Makefiles\" -S \"" + m_ProjectParameter.getString("source_directory") + "\" -B \"" + m_ProjectParameter.getString("build_directory") + "\"";
		std::string buildProject		= "mingw32-make -C \"" + m_ProjectParameter.getString("build_directory") + "\"";

		m_ProjectParameter.setString("clean_project_command", cleanProject);
		m_ProjectParameter.setString("configure_project_command", configureProject);
		m_ProjectParameter.setString("build_project_command", buildProject);

		std::string library = formatString(m_ProjectParameter.getString("project_name"), StringFormat::ONE_WORD);

		std::string libraryFile			= getPath({m_ProjectParameter.getString("build_directory"), "libnerogame-" + library}, StringPool.EXTENSION_DLL);
		std::string libraryFileCopy		= getPath({m_ProjectParameter.getString("build_directory"), "libnerogame-" + library + "-copy"}, StringPool.EXTENSION_DLL);
		std::string copylibraryCommand	= "copy \"" + libraryFile + "\" \"" + libraryFileCopy + "\"";

		m_ProjectParameter.setString("library_file", libraryFile);
		m_ProjectParameter.setString("library_file_copy", libraryFileCopy);
		m_ProjectParameter.setString("copy_library_command", copylibraryCommand);

		m_ProjectParameter.setString("qt_creator", "\"C:\\Qt\\Tools\\QtCreator\\bin\\qtcreator.exe\"");
		m_ProjectParameter.setString("visual_studio", "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Preview\\Common7\\IDE\\devenv.exe\"");

		//nero_log(m_ProjectParameter.toString());

		m_AdvancedScene = std::make_shared<AdvancedScene>();
		m_AdvancedScene->setSetting(m_EngineSetting);
		m_AdvancedScene->setRenderTexture(m_RenderTexture);
		m_AdvancedScene->setResourceManager(m_ResourceManager);
		m_AdvancedScene->setRenderContext(m_RenderContext);
		m_AdvancedScene->setCamera(m_Camera);
		m_AdvancedScene->setProjectParameter(m_ProjectParameter);
		m_AdvancedScene->initialize();
    }

	ResourceManager::Ptr GameProject::getResourceManager()
	{
		return m_ResourceManager;
	}


    void GameProject::loadProject()
    {
		std::string levelDirectory = getPath({m_ProjectParameter.getString("project_directory"), "Scene", "Level"});
		std::experimental::filesystem::path directoryPath(levelDirectory);

		if(std::experimental::filesystem::is_empty(directoryPath))
		{
			return;
		}

		m_AdvancedScene->m_SelectedGameLevel = nullptr;
		m_AdvancedScene->m_SelectedWorldChunk = nullptr;
		m_AdvancedScene->m_SelectedWorldBuilder = nullptr;
		m_AdvancedScene->m_GameLevelTable.clear();

		//Iterate over files in the folder
		std::experimental::filesystem::directory_iterator it{directoryPath};
		while (it != std::experimental::filesystem::directory_iterator{})
		{
			auto gameLevel = loadGameLevel(loadJson(it->path().string(), true));

			m_AdvancedScene->m_GameLevelTable.push_back(gameLevel);

			if(gameLevel->selected)
			{
				m_AdvancedScene->m_SelectedGameLevel = gameLevel;

				for(auto chunk : gameLevel->chunkTable)
				{
					if(chunk->selected)
					{
						m_AdvancedScene->m_SelectedWorldChunk	= chunk;
						m_AdvancedScene->m_SelectedWorldBuilder = chunk->sceneBuilder;
						break;
					}
				}
			}

			it++;
		}


    }

	AdvancedScene::GameLevelPtr GameProject::loadGameLevel(const nlohmann::json& level)
	{
		auto gameLevel = std::make_shared<AdvancedScene::GameLevel>();
		gameLevel->name			= level["level_name"].get<std::string>();
		gameLevel->levelId		= level["level_id"];
		gameLevel->chunkCount	= level["chunk_count"];
		gameLevel->selected		= level["selected"];

		for(auto& chunk : level["chunk_table"])
		{
			auto worldChunk = std::make_shared<AdvancedScene::WorldChunk>();
			worldChunk->sceneBuilder->setResourceManager(m_ResourceManager);
			worldChunk->sceneBuilder->setRenderTexture(m_RenderTexture);
			worldChunk->sceneBuilder->setRenderContext(m_RenderContext);

			worldChunk->name			= chunk["chunk_name"].get<std::string>();
			worldChunk->chunkId			= chunk["chunk_id"];
			worldChunk->visible			= chunk["chunk_visible"];
			worldChunk->selected		= chunk["selected"];
			worldChunk->sceneBuilder->loadScene(chunk["world_chunk"]);

			gameLevel->chunkTable.push_back(worldChunk);
		}

		return gameLevel;
	}

	void GameProject::saveProject()
	{

	}

	void GameProject::saveGameLevel()
	{
		if(m_AdvancedScene->m_SelectedGameLevel)
		{
			auto level = m_AdvancedScene->m_SelectedGameLevel;

			nlohmann::json level_save;

			level_save["level_id"]		= level->levelId;
			level_save["level_name"]	= level->name;
			level_save["selected"]		= level->selected;
			level_save["chunk_count"]	= level->chunkCount;
			level_save["chunk_table"]	= nlohmann::json::array();

			for(auto& chunk : level->chunkTable)
			{
				nlohmann::json chunk_save;

				chunk_save["world_chunk"]		= chunk->sceneBuilder->saveScene();
				chunk_save["chunk_id"]			= chunk->chunkId;
				chunk_save["chunk_name"]		= chunk->name;
				chunk_save["chunk_visible"]		= chunk->visible;
				chunk_save["selected"]			= chunk->selected;
				chunk_save["load_with_level"]	= chunk->loadWithLevel;

				level_save["world_chunk_table"].push_back(chunk_save);
			}


			std::string levelFile = getPath({m_ProjectParameter.getString("project_directory"), "Scene", "Level", level->name}, StringPool.EXTENSION_JSON);
			saveFile(levelFile, level_save.dump(3), true);
		}
	}

	void GameProject::loadGameLevel()
	{

	}

	void GameProject::saveGameScreen()
	{
		if(m_AdvancedScene->m_SelectedGameScreen)
		{
			auto screen = m_AdvancedScene->m_SelectedGameScreen;

			nlohmann::json screen_save;

			screen_save["game_screen"]	= screen->sceneBuilder->saveScene();
			screen_save["screen_id"]	= screen->screenId;
			screen_save["screen_name"]	= screen->name;
			screen_save["selected"]		= screen->selected;

			std::string screenFile = getPath({m_ProjectParameter.getString("project_directory"), "Scene", "Screen", screen->name}, StringPool.EXTENSION_JSON);
			saveFile(screenFile, screen_save.dump(3), true);
		}
	}

	void GameProject::loadGameScreen()
	{

	}

	void GameProject::loadLibrary()
    {
		nero_log(m_ProjectParameter.getString("library_file"));

		if(!fileExist(m_ProjectParameter.getString("library_file")))
		{
			nero_log("no library to load");
			return;
		}


		if(fileExist(m_ProjectParameter.getString("library_file_copy")))
		{
			m_AdvancedScene->m_Scene = nullptr;
			m_AdvancedScene->m_CreateCppScene.clear();
			m_CreateCppSceneFn.clear();

		   removeFile(m_ProjectParameter.getString("library_file_copy"));
        }


		system(m_ProjectParameter.getString("copy_library_command").c_str());


		boost::dll::fs::path library_path(removeFileExtension(m_ProjectParameter.getString("library_file_copy")));

		try
		{
			boost::dll::shared_library lib(m_ProjectParameter.getString("library_file_copy"));

			nero_log_if("project library loaded", lib.is_loaded());

			m_CreateCppSceneFn.clear();

			m_CreateCppSceneFn = boost::dll::import_alias<CreateCppSceneFn>(library_path, "createScene", boost::dll::load_mode::append_decorations);


			if(!m_CreateCppSceneFn.empty())
			{
				m_AdvancedScene->setCppSceneCreator(m_CreateCppSceneFn);
			}


		}
		catch (std::exception e)
		{
			nero_log("loading failed");
		}
    }

    void GameProject::openEditor()
    {
		std::string codeEditor = m_ProjectParameter.getString("code_editor");

		if(codeEditor == "Qt Creator")
		{
			openQtCreator();
		}
		else if(codeEditor == "Visual Studio")
		{
			openVisualStudio();
		}
    }

	void GameProject::openQtCreator(const std::string& file)
	{
		//get the list of qtcreator process
		std::string listProcessCmd		= "tasklist /fo csv | findstr /i \"qtcreator\"";
		std::string processCSV			= exec(listProcessCmd.c_str());

		//if the Editor has been opened and are still available (has not been closed)
		if(m_EditorProcessId != StringPool.BLANK && processCSV.find(m_EditorProcessId) != std::string::npos)
		{
			//open it
			std::string cmd = "START \"\" " + m_ProjectParameter.getString("qt_creator") + " -pid " + m_EditorProcessId;
			system(cmd.c_str());
		}
		else
		{
			//open a new process
			std::string open_editor = "START \"\" " + m_ProjectParameter.getString("qt_creator") + " \"" + m_ProjectParameter.getString("cmake_file") +"\"";
			system(open_editor.c_str());

			//save the process id
				 processCSV  = exec(listProcessCmd.c_str());
			auto processTab  = splitString(processCSV, '\r\n');

			m_EditorProcessId = splitString(processTab.back(), ',').at(1);
		}
	}

	void GameProject::openVisualStudio(const std::string& file)
	{
		std::string listProcessCmd		= "tasklist /fo csv | findstr /i \"devenv\"";
		std::string processCSV			= exec(listProcessCmd.c_str());

		if(file == StringPool.BLANK)
		{
			//if the Editor has been opened and are still available (has not been closed)
			if(m_EditorProcessId != StringPool.BLANK && processCSV.find(m_EditorProcessId) != std::string::npos)
			{
				//open it
				std::string cmd = m_ProjectParameter.getString("visual_studio") + " /Edit";
				system(cmd.c_str());
			}
			else
			{
				std::string cmd = "START \"\" " + m_ProjectParameter.getString("visual_studio") + " \"" + m_ProjectParameter.getString("source_directory") +"\"" + " /Edit";
				system(cmd.c_str());

				//save the process id
					 processCSV  = exec(listProcessCmd.c_str());
				auto processTab  = splitString(processCSV, '\r\n');

				m_EditorProcessId = splitString(processTab.back(), ',').at(1);
			}
		}
		else
		{
			std::string cmd = "START \"\" " + m_ProjectParameter.getString("visual_studio") + " \"" + file + "\" " + " /Edit";
			system(cmd.c_str());
		}
	}

    std::string GameProject::exec(const char* cmd)
    {
        std::stringstream string_buffer;
        std::streambuf * cout_buffer = std::cout.rdbuf(string_buffer.rdbuf());

        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

        if (!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }

        std::cout.rdbuf(cout_buffer);

        return result;
    }

	void GameProject::compileProject(const BackgroundTask& backgroundTask)
	{

	}

    void GameProject::compileProject()
    {
		BackgroundTask& backgroundTask =  createBackgroundTask("Compile Project", "Game Project");

		backgroundTask.setStatus(1);
		backgroundTask.addMessage("Cleaning Project ...");
		backgroundTask.setErrorCode(
					system(m_ProjectParameter.getString("clean_project_command").c_str()));

		backgroundTask.setStatus(2);
		backgroundTask.addMessage("Configuring Project ...");
		backgroundTask.setErrorCode(
					system(m_ProjectParameter.getString("configure_project_command").c_str()));

		backgroundTask.setStatus(3);
		backgroundTask.addMessage("Building Project ...");
		backgroundTask.setErrorCode(
					system(m_ProjectParameter.getString("build_project_command").c_str()));

		backgroundTask.setStatus(4);
		backgroundTask.addMessage("Compilation Completed !!");

		std::this_thread::sleep_for(std::chrono::seconds(2));

		backgroundTask.setCompleted(true);

    }

	BackgroundTask& GameProject::createBackgroundTask(const std::string& name, const std::string& category)
	{
		m_BackgroundTaskTable.push_back(BackgroundTask(name, category));

		return m_BackgroundTaskTable.back();
	}

	std::vector<BackgroundTask>& GameProject::getBackgroundTaskTable()
	{
		return m_BackgroundTaskTable;
	}


    AdvancedScene::Ptr GameProject::getAdvancedScene()
    {
        return m_AdvancedScene;
    }

	void GameProject::setRenderTexture(const RenderTexturePtr& renderTexture)
	{
	   m_RenderTexture = renderTexture;
	}

	void GameProject::setCamera(const Camera::Ptr& camera)
	{
	   m_Camera = camera;
	}

	std::string GameProject::getProjectName() const
	{
		return m_ProjectParameter.getString("project_name");
	}

	void GameProject::setRenderContext(const RenderContextPtr& renderContext)
	{
		m_RenderContext = renderContext;
	}

	void GameProject::setSetting(const Setting::Ptr& setting)
	{
		m_EngineSetting = setting;
	}

	void GameProject::close()
	{
		//close editor (window)
		if(m_EditorProcessId != StringPool.BLANK)
		{
			std::string kill_command = "taskkill /F /PID " + m_EditorProcessId;
			system(kill_command.c_str());
		}

		m_AdvancedScene->m_Scene = nullptr;
		m_CreateCppSceneFn.clear();
	}

	void GameProject::createScriptObject(const Setting& parameter)
	{
		nero_log("creating script object");
		std::string header_template	= StringPool.BLANK;
		std::string source_template	= StringPool.BLANK;

		nero_log(parameter.getString("script_type"));

		if(parameter.getString("script_type") == "Game Level Script")
		{
			header_template	= loadText("template/cpp-project/CppGameLevel.h");
			source_template	= loadText("template/cpp-project/CppGameLevel.cpp");
		}
		else if(parameter.getString("script_type") == "Game Screen Script")
		{
			header_template	= loadText("template/cpp-project/CppGameScreen.h");
			source_template	= loadText("template/cpp-project/CppGameScreen.cpp");
		}

		std::string class_name		= parameter.getString("class_name") + "ScriptObject";
		std::string class_header	= boost::algorithm::to_upper_copy(class_name) + "_H";

		//file 1 : header
		boost::algorithm::replace_all(header_template, "::Class_Name::",		class_name);
		boost::algorithm::replace_all(header_template, "::Header_Gard::",		class_header);
		boost::algorithm::replace_all(header_template, "::Namespace::",			m_ProjectParameter.getString("project_namespace"));
		boost::algorithm::replace_all(header_template, "::Project_Name::",		m_ProjectParameter.getString("project_name"));
		boost::algorithm::replace_all(header_template, "::Project_Lead::",		m_ProjectParameter.getString("project_lead"));
		boost::algorithm::replace_all(header_template, "::Coypright_Date::",	toString(getCurrentDateTime().date().year()));

		//file 2 : source
		boost::algorithm::replace_all(source_template, "::Class_Name::",		class_name);
		boost::algorithm::replace_all(source_template, "::Namespace::",			m_ProjectParameter.getString("project_namespace"));
		boost::algorithm::replace_all(source_template, "::Project_Name::",		m_ProjectParameter.getString("project_name"));
		boost::algorithm::replace_all(source_template, "::Project_Lead::",		m_ProjectParameter.getString("project_lead"));
		boost::algorithm::replace_all(source_template, "::Coypright_Date::",	toString(getCurrentDateTime().date().year()));

		nero_log(m_ProjectParameter.getString("source_directory"));

		std::string header_file = getPath({m_ProjectParameter.getString("source_directory"),"cpp", "script", class_name}, StringPool.EXTENSION_CPP_HEADER);
		nero_log(header_file);

		saveFile(header_file, header_template);
		saveFile(getPath({m_ProjectParameter.getString("source_directory"), "cpp", "script", class_name}, StringPool.EXTENSION_CPP_SOURCE), source_template);
	}

}

