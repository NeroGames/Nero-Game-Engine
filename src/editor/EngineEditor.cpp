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
	{
		//initialize log, create setting directory if not exist
		initializeLogging();

		//load setting
		loadSetting();

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
			m_RenderWindow.clear(m_LoadingScreen->getBackgroundColor());
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
		std::string file = getPath({"setting", "logging_setting"}, StringPool.EXTENSION_TEXT);

		if(!fileExist(file))
		{
			createDirectory(getPath({"setting"}));

			//TODO
			//crteate setting file
		}

		el::Configurations setting(file);
		el::Loggers::reconfigureAllLoggers(setting);

		nero_log("Starting Nero Game Engine Editor ...");
	}

	void EngineEditor::loadSetting()
	{
		std::string file = getPath({"setting", "window_setting"}, StringPool.EXTENSION_JSON);

		if(!fileExist(file))
		{
			saveFile(file, EditorSetting.windowSetting.dump(3));
		}

		m_Setting.setDirectory("setting");
		m_Setting.load("window_setting");

		#ifdef NERO_GAME_DEBUG
			nero_log(m_Setting.toString());
		#endif
	}

	void EngineEditor::createRenderWindow()
    {
		//create render window
		m_RenderWindow.create(sf::VideoMode(m_Setting.getUInt("window_width"), m_Setting.getUInt("window_height")), EngineConstant.ENGINE_WINDOW_TITLE, sf::Style::Default);
		m_RenderWindow.setVerticalSyncEnabled(true);
		m_RenderWindow.resetGLStates();
		m_RenderWindow.setPosition(sf::Vector2i(m_Setting.getUInt("window_position_x"), m_Setting.getUInt("window_position_y")));
		setWindowIcon(m_Setting.getString("window_icon"));

		//setup imgui
		ImGui::SFML::Init(m_RenderWindow);
    }

	void EngineEditor::createLoadingScreen()
    {
        m_LoadingScreen = std::make_unique<LoadingScreen>();
		m_LoadingScreen->setRenderWindow(&m_RenderWindow);
        m_LoadingScreen->init();
    }

	void EngineEditor::startBackgroundTask()
	{
		m_StartEditorFuture = std::async(std::launch::async, &EngineEditor::startEngine, this, std::ref(m_EditorStarted), m_LoadingScreen->getMinTime());
	}

	int EngineEditor::startEngine(bool& engineStarted, const int duration)
    {
		//build directory

		//check environment variable

		//create the interface

		//load last workspace resource

		//open last project

		//commit


        //create the interface
		m_Interface = EditorInterface::Ptr(new EditorInterface(m_RenderWindow));

		//let the interface access to the editor setting
		//TODO
		//m_Interface->setEditorSetting(m_Setting);

		//set callback, allow interface to change window title
		m_Interface->setCallbackWindowTitle([this](const std::string& title)
        {
            setWindowTitle("Nero Game Engine - " +  title);
		});

		//m_Interface->loadAllProject();

		//check everionment variable
		char* neroGameHome	= getenv("NERO_GAME_HOME");
		char* visualStudio	= getenv("NERO_GAME_VISUAL_STUDIO");
		char* qtCreator		= getenv("NERO_GAME_QT_CREATOR");

		m_Setting.setString("nero_game_home",	(neroGameHome != nullptr) ? std::string(neroGameHome)	: StringPool.BLANK);
		m_Setting.setString("visual_studio",	(neroGameHome != nullptr) ? std::string(visualStudio)	: StringPool.BLANK);
		m_Setting.setString("qt_creator",		(neroGameHome != nullptr) ? std::string(qtCreator)		: StringPool.BLANK);

        std::this_thread::sleep_for(std::chrono::seconds(duration));

        //commit
        engineStarted = true;

        return 0;
    }

	void EngineEditor::destroyEditor()
	{
		ImGui::SFML::Shutdown();
	}

}
