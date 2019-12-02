#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H

#include <vector>
#include <memory>
//Box2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2TimeStep.h>
//#include <Nero/editor/SceneBuilder.h>
//#include <Nero/editor/UndoManager.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <ltbl/LightSystem.hpp>
#include <Nero/editor/EditorUtility.h>
#include <Nero/editor/SceneBuilder.h>

namespace nero
{
    //Forward declaration
    class AdvancedScene;

    //This is called when a joint in the world is implicitly destroyed because an attached body is destroyed.
    //This gives us a chance to nullify the mouse joint.
    class DestructionListener : public b2DestructionListener
    {
        public:
            void    SayGoodbye(b2Fixture* fixture);
            void    SayGoodbye(b2Joint* joint);

           std::shared_ptr<AdvancedScene>  scene;
    };

    class AdvancedScene
    {
		public:
			typedef std::shared_ptr<AdvancedScene> Ptr;
			typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;
			typedef std::shared_ptr<RenderContext> RenderContextPtr;

		public:
			struct	WorldChunk;
			struct	GameLevel;
			struct	GameScreen;
			typedef std::shared_ptr<GameLevel> GameLevelPtr;
			typedef std::shared_ptr<WorldChunk> WorldChunkPtr;
			typedef std::shared_ptr<GameScreen> GameScreenPtr;

			struct WorldChunk
			{
				WorldChunk(const std::string& name): name(name)
				{
					sceneBuilder = std::make_shared<SceneBuilder>();
				}

				int					chunkId			= -1;
				std::string			name			= StringPool.BLANK;
				bool				isVisible		= true;
				SceneBuilder::Ptr	sceneBuilder	= nullptr;
			};

			struct GameLevel
			{
				GameLevel(const std::string& name): name(name), chunkTable()
				{
					//Empty
				}

				int							levelId		= -1;
				std::string					name		= StringPool.BLANK;
				int							chunkCount	= 0;
				std::vector<WorldChunkPtr>	chunkTable;
			};


			struct GameScreen
            {
				GameScreen(const std::string& name): name(name)
				{
					sceneBuilder = std::make_shared<SceneBuilder>();
				}

				int						screenId		= -1;
				std::string             name			= StringPool.BLANK;
				SceneBuilder::Ptr       sceneBuilder	= nullptr;
            };

        public:
			AdvancedScene();

			void								handleEvent(const sf::Event& event, const EditorMode& editorMode, const BuilderMode& builderMode);
			void								update(const sf::Time& timeStep, const EditorMode& editorMode, const BuilderMode& builderMode);
			void								render(const EditorMode& editorMode, const BuilderMode& builderMode);

			void								initialize();
			 void								setScene(Scene::Ptr scene);
			 void								setRenderContext(const RenderContextPtr& renderContext);
			 void								setRenderTexture(const RenderTexturePtr& renderTexture);
			 void								setResourceMananger(const ResourceManager::Ptr& resourceManager);
			 void								setCamera(const Camera::Ptr& camera);
			 void								setSetting(const Setting::Ptr& setting);

			 void								renderScreenBuilder(sf::RenderTexture& texture);
			 void								renderScene(sf::RenderTexture& texture);

			 void								addGameLevel(const std::string& name);
			 void								addWorldChunk(const std::string& name);
			 void								addGameScreen(const std::string& name);

			 void								setResourceManager(const ResourceManager::Ptr& resourceManager);
			 void								addObject(Object::Type type, const sf::String& label, sf::Vector2f position, const EditorMode& editorMode);

			 GameLevelPtr						getSelectedGameLevel();
			 WorldChunkPtr						getSelectedWorldChunk();
			 void								setSelectedWorldChunk(WorldChunkPtr worldChunk);
			 SceneBuilder::Ptr					getSelectedSceneBuilder(const EditorMode& editorMode);


        private:
            //Friend
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;
            friend class                SceneManager;
			friend class				GameProject;
            //Destruction Listener
            DestructionListener         m_DestructionListener;

			std::vector<GameLevelPtr>	m_GameLevelTable;
			GameLevelPtr				m_SelectedGameLevel;
			WorldChunkPtr				m_SelectedWorldChunk;
			std::vector<GameScreenPtr>  m_GameScreenTable;
			SceneBuilder::Ptr			m_SelectedWorldBuilder;
			SceneBuilder::Ptr			m_SelectedScreenBuilder;
			GameScreenPtr				m_SelectedGameScreen;
            //other
            b2Body*                     m_Bomb;
            b2Vec2                      m_BombSpawnPoint;
            bool                        m_BombSpawning;
            b2Body*                     m_GroundBody;
            b2AABB                      m_WorldAABB;
            b2MouseJoint*               m_MouseJoint;
            b2Vec2                      m_MouseWorld;
            int32                       m_StepCount;
            b2Profile                   m_MaxProfile;
            b2Profile                   m_TotalProfile;

			Scene::Ptr                  m_Scene;
			RenderContextPtr            m_RenderContext;
			ltbl::LightSystem			m_LightEngine;
			ResourceManager::Ptr		m_ResourceManager;
			RenderTexturePtr			m_RenderTexture;
			Camera::Ptr					m_Camera;
			Setting::Ptr				m_EngineSetting;

			//
			int							m_GameScreenCount;

    };
}

#endif // ADVANCEDSCENE_H
