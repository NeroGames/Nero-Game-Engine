////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef ENGINERENDERER_H
#define ENGINERENDERER_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/engine/CoreEngine.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/cpp/engine/StartupScreen.h>
//STD
#include <future>
/////////////////////////////////////////////////////////////
namespace  nero
{
	class EngineRenderer : public CoreEngine
	{
		public:
			typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;

		public:
									EngineRenderer();
			virtual                ~EngineRenderer() override;
		private:
			bool					checkDirectory();
			void					noGameFound();
			void					loadGame();
			void					loadScene();
			//game loop
			void                    handleEvent()                    override;
			void                    update(const sf::Time& timeStep) override;
			void                    render()                         override;

		private:
			//Game Scene
			Scene::Ptr				m_GameScene;
			//Startup Screen
			StartupScreen::Ptr		m_StartupScreen;
			std::future<int>        m_StartEngineFuture;
			bool                    m_EngineStarted;
			//Resource Manager
			ResourceManager::Ptr    m_ResourceManager;
			//Camera
			Camera::Ptr				m_Camera;
			//Setting
			Setting::Ptr			m_Setting;
			//Render Texture
			RenderTexturePtr		m_RenderTexture;
	};

}

#endif // ENGINERENDERER_H
