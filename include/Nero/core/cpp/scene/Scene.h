////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCENE_H
#define SCENE_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/camera/Camera.h>
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/model/Collision.h>
#include <Nero/core/cpp/scene/SceneUtility.h>
#include <Nero/core/cpp/scene/ShapeRenderer.h>
#include <Nero/core/cpp/scene/ObjectManager.h>
#include <Nero/core/cpp/scene/SoundManager.h>
#include <Nero/core/cpp/scene/SceneBuilder.h>
#include <Nero/core/cpp/object/GameLevelScriptObject.h>
#include <Nero/core/cpp/object/GameScreenScriptObject.h>
//SFML
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
//Box2D
#include <Box2D/Dynamics/b2World.h>
//STD
#include <memory>
#include <vector>
//
#include <ltbl/LightSystem.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
	class  Scene : public b2ContactListener
    {
		public: //type definiton
			typedef std::shared_ptr	<Scene>				Ptr;
			typedef std::shared_ptr<sf::RenderTexture>	RenderTexturePtr;
			typedef std::shared_ptr<ltbl::LightSystem>	LightManagerPtr;
			typedef GameScreenScriptObject::Ptr	(CreateCppGameScreenScript)(ScriptObject::Context);
			typedef GameLevelScriptObject::Ptr	(CreateCppGameLevelScript)(ScriptObject::Context);


		public: //utility
			enum class EngineType
			{
				RENDER_ENGINE,
				EDITOR_ENGINE,
				SCENE_RENDERER
			};

			enum class PlatformType
			{
				WINDOWS,
				LINUX,
				MACOS,
				ANDROID,
				IOS
			};

			class Context
			{
				public:
					Context(std::string sceneName, RenderTexturePtr renderTexture, ResourceManager::Ptr resourceManager, Camera::Ptr camera,
							Setting::Ptr setting, EngineType engineType, PlatformType platformType);

					Context();

				private:
					friend class			Scene;
					friend class			GameProject;
					friend class			AdvancedScene;
					RenderTexturePtr		renderTexture;
					ResourceManager::Ptr    resourceManager;
					Camera::Ptr             camera;
					Setting::Ptr			setting;
					EngineType				engineType;
					PlatformType			platformType;
					std::string				sceneName;
			};

		public:
			//constructor
											Scene(Context context);
			virtual                        ~Scene();
			//main method
			virtual void					handleEvent(const sf::Event& event);
			virtual void					update(const sf::Time& timeStep);
			virtual void					render();
			virtual void					renderShape();
			virtual void					renderFrontScreen();

		protected:
			virtual void					init();
			virtual void                    destroy();
			//collision
			virtual void					handleCollisionContactBegin(Collision collision);
			virtual void					handleCollisionContactEnd(Collision collision);
			virtual void					handleCollisionPreSolveContact(Collision collision);
			virtual void					handleCollisionPostSolveContact(Collision collision);
			//input
			virtual void					handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
			virtual void					handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
			virtual void					handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
			virtual void					handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);
			//gameplay
			void							pauseScene();
			void							resumeScene();
			void							resetScene();
			void							quitScene();
			void							hideGameWorld();
			void							showGameWorld();
			void							loadGameLevel(const std::string& levelName);
			void							enableGameLevel(const std::string& levelName);
			void							loadWorldChunk(const std::string& name);
			void							unLoadGameLevel(const std::string& name);
			void							unLoadWorldChunk(const std::string& name);
			void							hideGameScreen(const std::string& name);
			void							showGameScreen(const std::string& name);
			void							enableObjectLayer(const std::string& name);
			void							disableObjectLayer(const std::string& name);
			void							enableGameChunk(const std::string& name);
			void							disableGameChunk(const std::string& name);

			Context&						getSceneContext();
			RenderTexturePtr				getRenderTexture();
			ResourceManager::Ptr			getResourceHolder();

		private:
			void							BeginContact(b2Contact* contact);
			void							EndContact(b2Contact* contact);
			void							PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
			void							PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
			//
			void							setupLighting();
			void							renderLighting();
			//
			void							disableCurrentGameLevel();
			void							destroyLevel(Object::Ptr levelObject);


        protected:
			//friend
			friend class					DestructionListener;
			friend class					BoundaryListener;
			friend class					ContactListener;
			friend class					AdvancedScene;
			friend class					GameProject;
			friend class					EngineRenderer;
			friend class					SceneRenderer;
			//scene context
			Context                         m_SceneContext;
			//game world
			Object::Ptr                     m_GameWorld;
			//physic world
			b2World*                        m_PhysicWorld;
			int32							m_ContactPointCount;
			ContactPoint					m_ContactVectorTablele[MAX_CONTACT_POINT];
			//manager
			ShapeRenderer					m_ShapeRenderer;
			ObjectManager::Ptr				m_ObjectManager;
			SoundManager::Ptr				m_SoundManager;
			//frame rate
			float							m_FrameRate;
			float							m_FrameTime;
			//gameplay
			bool							m_HideWorld;
			//text display
			sf::Text						m_InformationText;
			sf::String						m_InformationContent;
			//
			Setting::Ptr					m_LevelSetting;
			//
			LightManagerPtr					m_LightManager;
			//
			std::function<void()>           m_QuitEngine;
			//
			std::map<std::string, boost::function<CreateCppGameLevelScript>>	m_CreateGameLevelMap;
			std::map<std::string, boost::function<CreateCppGameScreenScript>>	m_CreateGameScreenMap;
			//std::map<std::string, boost::function<CreateCppSimpleScript>>	m_CreateGameScreenMap;
			//std::map<std::string, boost::function<CreateCppPhysicScript>> m_CreateGameScreenMap;
			//
			Setting::Ptr					m_GameSetting;
			SceneBuilder::Ptr				m_SceneBuilder;
			//
			std::map<std::string, ResourceManager::Ptr> m_GameLevelResourceMap;
			//
	};
}

#endif // SCENE_H
