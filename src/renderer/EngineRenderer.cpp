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

		if(m_GameScene)
		{
			m_GameScene->m_QuitEngine = [this](){ m_RenderWindow.close();};
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
		//
		m_EngineStarted = true;
	}

	void EngineRenderer::loadGame()
	{

	}

	void EngineRenderer::handleEvent()
	{
		sf::Event event;
		while(m_RenderWindow.pollEvent(event))
		{
			if(m_StartupScreen && !m_EngineStarted)
			{
				//TODO Startup Screen
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
			//TODO Startup Screen
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
			//TODO Startup Screen
		}
		else
		{
			m_GameScene->render();
		}
	}
}
