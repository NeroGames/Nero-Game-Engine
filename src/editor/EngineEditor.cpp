////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/EngineEditor.h>
#include <Nero/editor/EditorSetting.h>
#include <Nero/editor/EditorInterface.h>
#include <Nero/core/utility/FileUtil.h>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace  nero
{
	EngineEditor::EngineEditor() : CoreEngine(false)
		,m_EditorStarted(false)
		,m_EditorTextureHolder(nullptr)
		,m_EditorSoundHolder(nullptr)
		,m_Setting(nullptr)
	{
		//initialize log
		initializeLogging();

		//load window setting
		loadEarlySetting();

		//create the rendering window
		createRenderWindow();

		//create the loading screen
		createLoadingScreen();

		//start a new thread and initialize the editor in background
		startBackgroundTask();
    }

	EngineEditor::~EngineEditor()
    {
		destroyEditor();
    }

	void EngineEditor::destroyEditor()
	{
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
			m_RenderWindow.clear(EngineConstant.CANVAS_COLOR);
                m_Interface->render();
			m_RenderWindow.display();
        }
    }

	void EngineEditor::initializeLogging()
	{
		std::string file = getPath({"setting", "logging"}, StringPool.EXTENSION_TEXT);

		if(!fileExist(file))
		{
			createDirectory(getPath({"setting"}));

			//TODO
			//crteate setting file
		}

		el::Configurations setting(file);
		el::Loggers::reconfigureAllLoggers(setting);

		//first log
		nero_log("Starting Nero Game Engine Editor ...");
		nero_log("Lean more at https://nero-game.com/");
	}

	void EngineEditor::loadEarlySetting()
	{
		std::string file = getPath({"setting", "window"}, StringPool.EXTENSION_JSON);

		if(!fileExist(file))
		{
			saveFile(file, EditorSetting.windowSetting.dump(3));
		}

		file = getPath({"setting", "resource"}, StringPool.EXTENSION_JSON);
		if(!fileExist(file))
		{
			//saveFile(file, EditorSetting.resourceSetting.dump(3));
		}

		m_Setting = std::make_shared<Setting>();

		m_Setting->setDirectory("setting");
		m_Setting->loadSetting("window");
		m_Setting->loadSetting("resource");

		#ifdef NERO_GAME_DEBUG
			nero_log(m_Setting->toString());
		#endif
	}

	void EngineEditor::createRenderWindow()
    {
		//create render window
		m_RenderWindow.create(sf::VideoMode(m_Setting->getSetting("window").getUInt("width"), m_Setting->getSetting("window").getUInt("height")), EngineConstant.ENGINE_WINDOW_TITLE, sf::Style::Default);
		m_RenderWindow.setVerticalSyncEnabled(true);
		m_RenderWindow.resetGLStates();
		m_RenderWindow.setPosition(sf::Vector2i(m_Setting->getSetting("window").getUInt("position_x"), m_Setting->getSetting("window").getUInt("position_y")));
		setWindowIcon(m_Setting->getSetting("window").getString("icon_file"));

		//setup imgui
		ImGui::SFML::Init(m_RenderWindow);
	}

	void EngineEditor::createLoadingScreen()
    {
		//load resource
			//font
		FontHolder::Ptr	fontHolder = std::make_shared<FontHolder>(m_Setting->getSetting("resource").getSetting("font"));
		fontHolder->loadDirectory("resource/startup/font");
			//texture
		TextureHolder::Ptr textureHolder = std::make_shared<TextureHolder>(m_Setting->getSetting("resource").getSetting("texture"));
		textureHolder->loadDirectory("resource/startup/texture");
			//animation
		AnimationHolder::Ptr animationHolder = std::make_shared<AnimationHolder>(m_Setting->getSetting("resource").getSetting("animation"));
		animationHolder->loadDirectory("resource/startup/animation");
			//sound
		SoundHolder::Ptr soundHolder = std::make_shared<SoundHolder>(m_Setting->getSetting("resource").getSetting("sound"));
		soundHolder->loadDirectory("resource/startup/sound");
			//music
		MusicHolder::Ptr musicHolder = std::make_shared<MusicHolder>(m_Setting->getSetting("resource").getSetting("music"));
		musicHolder->loadDirectory("resource/startup/music");

		//build startup screen
		m_LoadingScreen = std::make_unique<LoadingScreen>();
		m_LoadingScreen->setRenderWindow(&m_RenderWindow);
		m_LoadingScreen->setFontHolder(fontHolder);
		m_LoadingScreen->setTextureHolder(textureHolder);
		m_LoadingScreen->setAnimationHolder(animationHolder);
		m_LoadingScreen->setSoundHolder(soundHolder);
		m_LoadingScreen->setMusicHolder(musicHolder);

		//let the user initialize the screen
        m_LoadingScreen->init();
    }

	void EngineEditor::startBackgroundTask()
	{
		m_StartEditorFuture = std::async(std::launch::async, &EngineEditor::startEngine, this, std::ref(m_EditorStarted), m_LoadingScreen->getDuration());
	}

	int EngineEditor::startEngine(bool& engineStarted, const int duration)
	{
		//Start a clock
		sf::Clock clock;

		//build directory
		buildDirectory();

		//load setting
		loadSetting();

		//check everionment variable
		checkEnvironmentVariable();

		//load editor resource
		loadEditorResource();

		//load starter resource pack
		loadStarterResourcePack();

		//load workspace resource
		loadWorkspaceResource();

		//create camera
		createCamera();

		//create editor interface
		createEditorInterface();

		//open last project
		openLastProject();

		//wait if necessary
		int time = duration - clock.getElapsedTime().asSeconds();
		std::this_thread::sleep_for(std::chrono::seconds(time < 0 ? 0 : time));

		//commit
		engineStarted = true;

		nero_log("starting completed");

        return 0;
    }

	void EngineEditor::buildDirectory()
	{
		nero_log("building engine editor directory");

		//create directory
		//createDirectory(getPath({"setting"})); //already created
		createDirectory(getPath({"logging"}));
		createDirectory(getPath({"plugin"}));
		createDirectory(getPath({"template"}));
			createDirectory(getPath({"template", "cpp project"}));
			createDirectory(getPath({"template", "lua project"}));
			createDirectory(getPath({"template", "cpp-lua project"}));
		createDirectory(getPath({"resource"}));
			createDirectory(getPath({"resource", "startup"}));
				createDirectory(getPath({"resource", "startup", "texture"}));
				createDirectory(getPath({"resource", "startup", "animation"}));
				createDirectory(getPath({"resource", "startup", "sound"}));
				createDirectory(getPath({"resource", "startup", "music"}));
				createDirectory(getPath({"resource", "startup", "font"}));
			createDirectory(getPath({"resource", "editor"}));
				createDirectory(getPath({"resource", "startup", "texture"}));
				createDirectory(getPath({"resource", "startup", "sound"}));
				createDirectory(getPath({"resource", "startup", "language"}));
			createDirectory(getPath({"resource", "starterpack"}));
				createDirectory(getPath({"resource", "starterpack", "font"}));
				createDirectory(getPath({"resource", "starterpack", "texture"}));
				createDirectory(getPath({"resource", "starterpack", "animation"}));
				createDirectory(getPath({"resource", "starterpack", "lightmap"}));
				createDirectory(getPath({"resource", "starterpack", "sound"}));
				createDirectory(getPath({"resource", "starterpack", "music"}));
				createDirectory(getPath({"resource", "starterpack", "language"}));
				createDirectory(getPath({"resource", "starterpack", "shader"}));
				createDirectory(getPath({"resource", "starterpack", "luascript"}));
				createDirectory(getPath({"resource", "starterpack", "cppscript"}));

		//create file
		//logging_setting, window_setting already exist

			//setting
		/*std::string resource_setting = getPath({"setting", "resource"}, StringPool.EXTENSION_JSON);
		if(!fileExist(resource_setting))
		{
			//saveFile(editor_setting, EditorSetting.resourceSetting.dump(3));
		}*/
	}

	void EngineEditor::loadSetting()
	{
		nero_log("loading setting");

		//logging setting	: already loaded
		//window setting	: already loaded

		//recent project setting
		m_Setting->loadSetting("recent_project");

		//list of workspace
		m_Setting->loadSetting("worksapce");

		//dockspace ids
		m_Setting->loadSetting("dockspace");
	}

	void EngineEditor::checkEnvironmentVariable()
	{
		nero_log("checking environment variable");

		char* neroGameHome	= getenv("NERO_GAME_HOME");
		char* visualStudio	= getenv("NERO_GAME_VISUAL_STUDIO");
		char* qtCreator		= getenv("NERO_GAME_QT_CREATOR");

		Setting environment;

		environment.setString("nero_game_home", (neroGameHome != nullptr) ? std::string(neroGameHome)	: StringPool.BLANK);
		environment.setString("visual_studio", (visualStudio != nullptr) ? std::string(visualStudio) : StringPool.BLANK);
		environment.setString("qt_creator", (qtCreator != nullptr) ? std::string(qtCreator) : StringPool.BLANK);

		m_Setting->setSetting("environment", environment);

		neroGameHome	= nullptr;
		visualStudio	= nullptr;
		qtCreator		= nullptr;
	}

	void EngineEditor::loadEditorResource()
	{
		nero_log("loading editor resource");

		m_EditorTextureHolder = std::make_shared<TextureHolder>(m_Setting->getSetting("resource").getSetting("texture"));
		m_EditorTextureHolder->loadDirectory("resource/editor/texture");

		m_EditorSoundHolder = std::make_shared<SoundHolder>(m_Setting->getSetting("resource").getSetting("sound"));
		m_EditorSoundHolder->loadDirectory("resource/editor/sound");

		m_EditorFontHolder = std::make_shared<FontHolder>(m_Setting->getSetting("resource").getSetting("font"));
		m_EditorFontHolder->loadDirectory("resource/editor/font");
	}

	void EngineEditor::loadStarterResourcePack()
	{
		nero_log("loading starter pack resource");

		m_ResourceManager = ResourceManager::Ptr(new ResourceManager(m_Setting->getSetting("resource")));
		m_ResourceManager->loadDirectory("resource/starterpack");
	}

	void EngineEditor::loadWorkspaceResource()
	{
		nero_log("loading workspace resource");

		if(m_Setting->getSetting("recent_project").getString("last_workspace") != StringPool.BLANK)
		{
			m_ResourceManager->loadDirectory(getPath({m_Setting->getSetting("recent_project").getString("last_workspace"), "Resource"}));
		}
	}

	void EngineEditor::createCamera()
	{
		m_AdvancedCamera = std::make_shared<AdvancedCamera>();
	}

	void EngineEditor::createEditorInterface()
	{
		nero_log("creating editor interface");

		m_Interface = EditorInterface::Ptr(new EditorInterface(m_RenderWindow));

		//let the interface access to the editor setting
		m_Interface->setSetting(m_Setting);

		m_Interface->setEditorTextureHolder(m_EditorTextureHolder);
		m_Interface->setEditorSoundHolder(m_EditorSoundHolder);
		m_Interface->setEditorFontHolder(m_EditorFontHolder);
		m_Interface->setResourceManager(m_ResourceManager);
		m_Interface->setCamera(m_AdvancedCamera);

		//set callback, allow interface to change window title
		m_Interface->setCallbackWindowTitle([this](const std::string& title)
		{
			setWindowTitle("Nero Game Engine - " +  title);
		});

		m_Interface->init();
	}

	void EngineEditor::openLastProject()
	{
		nero_log("opening last project");

		if(m_Setting->getSetting("recent_project").getString("last_project") != StringPool.BLANK)
		{
			m_Interface->openProject(m_Setting->getSetting("recent_project").getString("last_project"));
		}
	}
}
