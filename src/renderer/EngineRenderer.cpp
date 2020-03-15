////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/renderer/EngineRenderer.h>
#include <Nero/renderer/NoGameFound.h>
#include <Nero/core/cpp/utility/LogUtil.h>
/////////////////////////////////////////////////////////////

namespace  nero
{
	EngineRenderer::EngineRenderer() : CoreEngine(true)
		,m_GameScene(nullptr)
		,m_StartupScreen(nullptr)
		,m_EngineStarted(false)
	{
		bool everythingGood = checkDirectory();

		if(everythingGood)
		{
			loadGame();
		}
		else
		{
			noGameFound();
		}
	}

	EngineRenderer::~EngineRenderer()
	{

	}

	bool EngineRenderer::checkDirectory()
	{
		return false;
	}

	void EngineRenderer::noGameFound()
	{
		//create window
		m_RenderWindow.create(sf::VideoMode(
							EngineConstant.ENGINE_WINDOW_WIDTH,
							EngineConstant.ENGINE_WINDOW_HEIGHT),
							EngineConstant.NO_GAME_FOUND,
							sf::Style::Close);

		//build NoGameScene
		m_GameScene = NoGameFound::Ptr(new NoGameFound(Scene::Context(
							EngineConstant.NO_GAME_FOUND,
							m_RenderTexture,
							m_ResourceManager,
							m_Camera,
							m_Setting,
							Scene::EngineType::RENDER_ENGINE,
							Scene::PlatformType::WINDOWS)));

		//set quit engine callback
		m_GameScene->m_QuitEngine = [this](){ m_RenderWindow.close();};

		//skip startup screen display
		m_EngineStarted = true;
	}

	void EngineRenderer::loadGame()
	{
		//load parameter

		//load statup screen
		loadStartupScreen();

		//start bacground initialization
		startBackgroundTask();
	}

	void EngineRenderer::startBackgroundTask()
	{
		m_StartEngineFuture = std::async(std::launch::async, &EngineRenderer::startEngine, this, std::ref(m_EngineStarted), m_StartupScreen->getDuration());
	}

	void EngineRenderer::loadStartupScreen()
	{
		/*boost::dll::shared_library lib(m_ProjectParameter.getString("library_file"));
		m_CreateStartupScreen = boost::dll::import_alias<CreateCppSceneFn>(library_path, "createStartupScreen", boost::dll::load_mode::append_decorations);

		if(!m_CreateStartupScreen.empty())
		{
			m_StartupScreen = m_CreateStartupScreen();
		}*/
	}

	int EngineRenderer::startEngine(bool& engineStarted, const int duration)
	{
		return 0;
	}

	void EngineRenderer::handleEvent()
	{
		sf::Event event;
		while(m_RenderWindow.pollEvent(event))
		{
			if(m_StartupScreen && !m_EngineStarted)
			{
				m_GameScene->handleEvent(event);
			}
			else
			{
				m_GameScene->handleEvent(event);
			}
		}
	}

	void EngineRenderer::update(const sf::Time& timeStep)
	{
		if(m_StartupScreen && !m_EngineStarted)
		{
			m_StartupScreen->update(timeStep);
		}
		else
		{
			m_GameScene->update(timeStep);
		}
	}

	void EngineRenderer::render()
	{
		if(m_StartupScreen && !m_EngineStarted)
		{
			m_StartupScreen->render();
		}
		else
		{
			m_GameScene->render();
		}
	}
}
