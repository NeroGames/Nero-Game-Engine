////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/editor/EngineEditor.h>
#include <Nero/editor/EditorSetting.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/editor/EditorInterface.h>
#include <Nero/core/cpp/utility/File.h>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace  nero
{
	EngineEditor::EngineEditor() : CoreEngine(false)
		,m_EditorStarted(false)
	{
		//initialize logging
		initLogging();

		//load window and resource settings
		initSetting();

		//create the rendering window
		createRenderWindow();

		//create the loading screen
		createLoadingScreen();

		//start a new thread and initialize the editor in background
		backgroundStartup();
    }

	EngineEditor::~EngineEditor()
    {
		destroy();
    }

	void EngineEditor::destroy()
	{
		m_EditorTextureHolder->clear();
		m_EditorFontHolder->clear();
		m_EditorSoundHolder->clear();

		m_EditorStarted			= false;
		m_LoadingScreen			= nullptr;
		m_EditorTextureHolder	= nullptr;
		m_EditorFontHolder		= nullptr;
		m_EditorSoundHolder		= nullptr;
		m_EditorSetting			= nullptr;
		m_EditorCamera		= nullptr;
		m_Interface				= nullptr;

		ImGui::SFML::Shutdown();
	}

	void EngineEditor::handleEvent()
	{
        sf::Event event;
		while(m_RenderWindow.pollEvent(event))
        {
			if(!m_EditorStarted)
            {
				//loading screen event
                m_LoadingScreen->handleEvent(event);
            }
            else
            {
				//editor interface event
                m_Interface->handleEvent(event);
            }
        }
    }

	void EngineEditor::update(const sf::Time& timeStep)
	{
		if(!m_EditorStarted)
        {
			//update loading screen
             m_LoadingScreen->update(timeStep);
        }
        else
        {
			//update interface
            m_Interface->update(timeStep);
            m_Interface->updateFrameRate(getFrameRate(), getFrameTime());
        }
    }

	void EngineEditor::render()
	{
		if(!m_EditorStarted)
        {
			//render loading screen
			m_RenderWindow.clear(m_LoadingScreen->getCanvasColor());
                m_LoadingScreen->render();
			m_RenderWindow.display();
        }
        else
        {
			//render interface
			m_RenderWindow.clear(EngineConstant.COLOR_CANVAS);
                m_Interface->render();
			m_RenderWindow.display();
        }
    }

	void EngineEditor::initLogging()
	{
		//initialize the engine logger
		logging::Logger::init();

		//TODO load with setting
		//Setting loggingSetting;
		//loggingSetting.file::loadJson(file::loadJson(file::getPath({"setting", "logging"})));
		//Logger::init(loggingSetting);

		nero_log("Nero Game Engine");
		nero_log("Copyright (C) Nero Games Corporation. All rights reserved");
		nero_log("Lean more at https://nero-game.com/");
		nero_log("---------------------------------------------------------------------");
		nero_log("Engine Editor is starting ..");
	}

	void EngineEditor::initSetting()
	{
		nero_log("loading startup setting ..");

		m_EditorSetting = std::make_shared<Setting>();

		nero_log("loading render window settings")
		std::string window_setting = file::getPath({"setting", "window"}, string::StringPool.EXT_JSON);
		if(!file::fileExist(window_setting))
		{
			file::saveFile(window_setting, EditorSetting.windowSetting.dump(3));
		}
		m_EditorSetting->loadSetting(window_setting, true);


		nero_log("loading resource settings");
		std::string resource_setting = file::getPath({"setting", "resource"}, string::StringPool.EXT_JSON);
		if(!file::fileExist(resource_setting))
		{
			//TODO
			//file::saveFile(file, EditorSetting.resourceSetting.dump(3));
		}
		m_EditorSetting->loadSetting(resource_setting, true);

		nero_log("load startup setting completed")
	}

	void EngineEditor::createRenderWindow()
    {
		nero_log("creating render window ..");

		m_RenderWindow.create(sf::VideoMode(m_EditorSetting->getSetting("window").getUInt("width"),
												m_EditorSetting->getSetting("window").getUInt("height")),
												EngineConstant.ENGINE_WINDOW_TITLE, sf::Style::Default);
		m_RenderWindow.setPosition(sf::Vector2i(m_EditorSetting->getSetting("window").getInt("position_x"),
												m_EditorSetting->getSetting("window").getInt("position_y")));

		m_RenderWindow.setVerticalSyncEnabled(true);
		m_RenderWindow.resetGLStates();
		setWindowIcon(m_EditorSetting->getSetting("window").getString("icon_file"));

		nero_log("initializing imgui");
		ImGui::SFML::Init(m_RenderWindow, false);

		nero_log("render window creation completed");
	}

	void EngineEditor::createLoadingScreen()
    {
		nero_log("creating loading screen ..");

		nero_log("loading startup resources")
		ResourceManager::Ptr resourceManager = std::make_shared<ResourceManager>(m_EditorSetting->getSetting("resource"));
		resourceManager->loadDirectory("resource/startup");

		nero_log("setting up loading screen")
		m_LoadingScreen = std::make_unique<LoadingScreen>();
		m_LoadingScreen->setRenderWindow(&m_RenderWindow);
		m_LoadingScreen->setResourceManager(resourceManager);

		nero_log("initializing loading screen")
        m_LoadingScreen->init();

		nero_log("loading screen creation completed");
    }

	void EngineEditor::backgroundStartup()
	{
		nero_log("launching background startup ..");
		m_StartupFuture = std::async(std::launch::async, &EngineEditor::startEngine, this, std::ref(m_EditorStarted), m_LoadingScreen->getDuration());
	}

	int EngineEditor::startEngine(bool& engineStarted, const int duration)
	{
		nero_log("running background startup .");

		nero_log("starting a clock");
		sf::Clock clock;

		//build directory
		buildDirectory();

		//load setting
		loadSetting();

		//check everionment variable
		checkEnvironmentVariable();

		//load editor resource
		loadEditorResource();

		//create camera
		createCamera();

		//create editor interface
		createEditorInterface();

		//open last project
		openLastProject();

		//wait if necessary
		int time = static_cast<int>(duration - clock.getElapsedTime().asSeconds());
		std::this_thread::sleep_for(std::chrono::seconds(time < 0 ? 0 : time));

		//commit
		engineStarted = true;

		nero_log("background startup completed");
		nero_log("---------------------------------------------------------------------");

        return 0;
    }

	void EngineEditor::buildDirectory()
	{
		nero_log("building editor directory");

		//create directory
		file::createDirectory(file::getPath({"setting"}));
		file::createDirectory(file::getPath({"logging"}));
		file::createDirectory(file::getPath({"plugin"}));
		file::createDirectory(file::getPath({"template"}));
			file::createDirectory(file::getPath({"template", "cpp-project"}));
			file::createDirectory(file::getPath({"template", "lua-project"}));
			file::createDirectory(file::getPath({"template", "cpp-lua-project"}));
		file::createDirectory(file::getPath({"resource"}));
			file::createDirectory(file::getPath({"resource", "startup"}));
				file::createDirectory(file::getPath({"resource", "startup", "texture"}));
				file::createDirectory(file::getPath({"resource", "startup", "animation"}));
				file::createDirectory(file::getPath({"resource", "startup", "sound"}));
				file::createDirectory(file::getPath({"resource", "startup", "music"}));
				file::createDirectory(file::getPath({"resource", "startup", "font"}));
			file::createDirectory(file::getPath({"resource", "editor"}));
				file::createDirectory(file::getPath({"resource", "startup", "texture"}));
				file::createDirectory(file::getPath({"resource", "startup", "sound"}));
				file::createDirectory(file::getPath({"resource", "startup", "language"}));
			file::createDirectory(file::getPath({"resource", "starterpack"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "font"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "texture"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "animation"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "lightmap"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "sound"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "music"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "language"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "shader"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "luascript"}));
				file::createDirectory(file::getPath({"resource", "starterpack", "cppscript"}));
	}

	void EngineEditor::loadSetting()
	{
		nero_log("loading editor settings");

		nero_log("loading recent project settings");
		checkRecentProject();
		m_EditorSetting->loadSetting(file::getPath({"setting", "recent_project"}));

		nero_log("loading workspace settings");
		checkWorkspace();
		m_EditorSetting->loadSetting(file::getPath({"setting", "worksapce"}));

		nero_log("loading dockspace settings");
		m_EditorSetting->loadSetting(file::getPath({"setting", "dockspace"}));
		m_EditorSetting->getSetting("dockspace").setBool("imgui_setting_exist",
			file::fileExist(file::getPath({"setting", EditorConstant.FILE_IMGUI_SETTING}, string::StringPool.EXT_INI)));


		nero_log("loading startup settings");
		checkWorkspace();
		m_EditorSetting->loadSetting(file::getPath({"setting", "startup"}));
	}

	void EngineEditor::checkRecentProject()
	{
		if(!file::fileExist(file::getPath({"setting", "recent_project"}, string::StringPool.EXT_JSON)))
		{
			file::saveFile(file::getPath({"setting", "recent_project"}, string::StringPool.EXT_JSON), nlohmann::json::array().dump(3));

			return;
		}

		auto recent_project_old = file::loadJson(file::getPath({"setting", "recent_project"}));

		nlohmann::json recent_project = nlohmann::json::array();

		for(auto project : recent_project_old)
		{
			if(file::fileExist(file::getPath({project["project_directory"], ".project"})))
			{
				recent_project.push_back(project);
			}
		}

		file::saveFile(file::getPath({"setting", "recent_project"}, string::StringPool.EXT_JSON), recent_project.dump(3), true);
	}

	void EngineEditor::checkWorkspace()
	{
		if(!file::fileExist(file::getPath({"setting", "workspace"}, string::StringPool.EXT_JSON)))
		{
			file::saveFile(file::getPath({"setting", "workspace"}, string::StringPool.EXT_JSON), nlohmann::json::array().dump(3));

			return;
		}

		auto workspace_setting_old= file::loadJson(file::getPath({"setting", "workspace"}));

		nlohmann::json workspace_setting;

		for(auto workspace : workspace_setting_old)
		{
			if(file::fileExist(file::getPath({workspace["workspace_directory"], ".workspace"})))
			{
				workspace_setting.push_back(workspace);
			}
		}

		file::saveFile(file::getPath({"setting", "workspace"}, string::StringPool.EXT_JSON), workspace_setting.dump(3), true);
	}

	void EngineEditor::checkEnvironmentVariable()
	{
		nero_log("checking environment variable");

		char* nero_game_home_char	= getenv("NERO_GAME_HOME");
		char* visual_studio_char	= getenv("NERO_GAME_VISUAL_STUDIO");
		char* qt_creator_char		= getenv("NERO_GAME_QT_CREATOR");

		std::string nero_game_home	= nero_game_home_char != nullptr ? std::string(nero_game_home_char)	: string::StringPool.BLANK;
		std::string visual_studio	= visual_studio_char != nullptr ? std::string(visual_studio_char)	: string::StringPool.BLANK;
		std::string qt_creator		= qt_creator_char != nullptr ? std::string(qt_creator_char)	: string::StringPool.BLANK;

		Setting environment;

		environment.setString("nero_game_home", nero_game_home);
		environment.setString("visual_studio", visual_studio);
		environment.setString("qt_creator",	qt_creator);

		m_EditorSetting->setSetting("environment", environment);

		nero_game_home_char	= nullptr;
		visual_studio_char	= nullptr;
		qt_creator_char		= nullptr;

		if(nero_game_home != string::StringPool.BLANK)
		{
			nero_log("NERO_GAME_HOME = " + nero_game_home);
		}
		else
		{
			nero_log("NERO_GAME_HOME not found");
		}

		if(visual_studio != string::StringPool.BLANK)
		{
			nero_log("NERO_GAME_VISUAL_STUDIO = " + visual_studio);
		}
		else
		{
			nero_log("NERO_GAME_VISUAL_STUDIO not found");
		}


		if(qt_creator != string::StringPool.BLANK)
		{
			nero_log("NERO_GAME_QT_CREATOR = " + qt_creator);
		}
		else
		{
			nero_log("NERO_GAME_QT_CREATOR not found");
		}
	}

	void EngineEditor::loadEditorResource()
	{
		nero_log("loading editor resource");

		m_EditorTextureHolder = std::make_shared<TextureHolder>(m_EditorSetting->getSetting("resource").getSetting("texture"));
		m_EditorTextureHolder->loadDirectory("resource/editor/texture");

		m_EditorSoundHolder = std::make_shared<SoundHolder>(m_EditorSetting->getSetting("resource").getSetting("sound"));
		m_EditorSoundHolder->loadDirectory("resource/editor/sound");

		m_EditorFontHolder = std::make_shared<FontHolder>(m_EditorSetting->getSetting("resource").getSetting("font"));
		m_EditorFontHolder->loadDirectory("resource/editor/font");
	}

	void EngineEditor::createCamera()
	{
		m_EditorCamera = std::make_shared<AdvancedCamera>();
	}

	void EngineEditor::createEditorInterface()
	{
		nero_log("creating editor interface");

		m_Interface = EditorInterface::Ptr(new EditorInterface(m_RenderWindow));

		//let the interface access to the editor setting
		m_Interface->setEditorSetting(m_EditorSetting);

		m_Interface->setEditorTextureHolder(m_EditorTextureHolder);
		m_Interface->setEditorSoundHolder(m_EditorSoundHolder);
		m_Interface->setEditorFontHolder(m_EditorFontHolder);
		m_Interface->setEditorCamera(m_EditorCamera);

		//set callback, allow interface to change window title
		m_Interface->setCallbackWindowTitle([this](const std::string& title)
		{
			setWindowTitle("Nero Game Engine - " +  title);
		});

		m_Interface->init();
	}

	void EngineEditor::openLastProject()
	{
		if(m_EditorSetting->getSetting("startup").getBool("open_last_project"))
		{
			nero_log("opening last project");

			auto recent_project = file::loadJson(file::getPath({"setting", "recent_project"}));

			if(!recent_project.empty())
			{
				std::string project_directory = recent_project.back()["project_directory"];

				//open project
				m_Interface->m_ProjectManager->openProject(project_directory);
			}
			else
			{
				nero_log("recent projects are emtpy");
			}
		}
	}
}
