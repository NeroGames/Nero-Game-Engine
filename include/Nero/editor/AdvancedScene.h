////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/editor/GameLevelBuilder.h>
#include <Nero/editor/GameScreenBuilder.h>
//STD
#include <memory>
#include <vector>
////////////////////////////////////////////////////////////
/*#include <vector>
//Box2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2TimeStep.h>
//#include <Nero/core/cpp/scene/SceneBuilder.h>
//#include <Nero/core/cpp/scene/UndoManager.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <Nero/editor/EditorUtility.h>
#include <Nero/core/cpp/scene/SceneBuilder.h>*/
////////////////////////////////////////////////////////////
namespace nero
{
	class AdvancedScene
	{
		public:
			typedef std::shared_ptr<AdvancedScene> Ptr;

		public:
															AdvancedScene();

			GameLevelBuilder::Ptr							addGameLevel(const Parameter& parameter);
			GameScreenBuilder::Ptr							addGameScreen(const Parameter& parameter);
			//setting
			void											setEngineSetting(const Setting::Ptr& setting);
			void											setProjectSetting(const Setting::Ptr& setting);
			std::vector<std::string> 						getGameLevelNameTable();
			GameLevelBuilder::Ptr							selectLevelBuilder(const std::string& name);

		private:
			//selection
			GameLevelBuilder::Ptr							m_SelectedGameLevel;
			GameScreenBuilder::Ptr							m_SelectedGameScreen;
			//storage
			std::vector<GameLevelBuilder::Ptr>				m_GameLevelTable;
			std::vector<GameScreenBuilder::Ptr>				m_GameScreenTable;
			std::vector<std::string>						m_GameLevelNameTable;
			//setting
			Setting::Ptr									m_EngineSetting;
			Setting::Ptr									m_ProjectSetting;
			Setting::Ptr									m_SceneSetting;

	};
}
/*namespace nero
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
			typedef Scene::Ptr (CreateCppSceneFn)(Scene::Context);


		public:
			struct	WorldChunk;
			struct	GameLevel;
			struct	GameScreen;
			typedef std::shared_ptr<GameLevel> GameLevelPtr;
			typedef std::shared_ptr<WorldChunk> WorldChunkPtr;
			typedef std::shared_ptr<GameScreen> GameScreenPtr;

			struct WorldChunk
			{
				int					chunkId			= -1;
				std::string			name			= StringPool.BLANK;
				bool				visible			= true;
				bool				selected		= false;
				SceneBuilder::Ptr	sceneBuilder	= std::make_shared<SceneBuilder>();
				bool				loadWithLevel	= true;
			};

			struct GameLevel
			{
				GameLevel()
				{
					Setting viewCenter;
					viewCenter.setFloat("x", 0.f);
					viewCenter.setFloat("y", 0.f);

					Setting gravity;
					gravity.setFloat("x", 0.f);
					gravity.setFloat("y", 9.8f);

					levelSetting = std::make_shared<Setting>();

					levelSetting->setSetting("gravity", gravity);
					levelSetting->setSetting("view_center", viewCenter);
					levelSetting->setFloat("time_step", 40.f);
					levelSetting->setUInt("velocity_iteration", 8);
					levelSetting->setUInt("position_iteration", 3);
					//
					levelSetting->setBool("draw_axis", true);
					levelSetting->setBool("draw_grid", true);
					levelSetting->setBool("draw_shape", true);
					levelSetting->setBool("draw_joint", true);
					levelSetting->setBool("draw_aabb", false);
					levelSetting->setBool("draw_contact_point", false);
					levelSetting->setBool("draw_contact_normal", false);
					levelSetting->setBool("draw_contact_impulse", false);
					levelSetting->setBool("draw_friction_impulse", false);
					levelSetting->setBool("draw_center_of_mass", false);
					levelSetting->setBool("draw_statistic", false);
					levelSetting->setBool("draw_profile", false);
					//
					levelSetting->setBool("enable_warm_starting", true);
					levelSetting->setBool("enable_continous", true);
					levelSetting->setBool("enable_sub_stepping", false);
					levelSetting->setBool("enable_sleep", true);
					//
					levelSetting->setBool("pause_level", false);
					levelSetting->setBool("single_step", false);
					//
					levelSetting->setBool("enable_lighting", true);
					levelSetting->setBool("lighting_on", true);
				}

				WorldChunkPtr getSelectedWorldChunk()
				{
					for(auto chunk : chunkTable)
					{
						if(chunk->selected)
						{
							return chunk;
						}
					}

					return nullptr;
				}

				void setSelectedWorldChunk(const int& chunkId)
				{
					for(auto chunk : chunkTable)
					{
						chunk->selected = false;

						if(chunk->chunkId == chunkId)
						{
							chunk->selected = true;
						}
					}
				}

				int							levelId			= -1;
				std::string					name			= StringPool.BLANK;
				int							chunkCount		= 0;
				bool						selected		= false;
				Setting::Ptr				levelSetting;
				std::vector<WorldChunkPtr>	chunkTable;
			};


			struct GameScreen
            {
				int						screenId		= -1;
				std::string             name			= StringPool.BLANK;
				bool					selected		= false;
				bool					visible			= true;
				SceneBuilder::Ptr       sceneBuilder	= std::make_shared<SceneBuilder>();
            };

        public:
												AdvancedScene();

			void								handleEvent(const sf::Event& event, const EditorMode& editorMode, const BuilderMode& builderMode);
			void								update(const sf::Time& timeStep, const EditorMode& editorMode, const BuilderMode& builderMode);
			void								render(const EditorMode& editorMode, const BuilderMode& builderMode);
			void								renderFrontScreen(const EditorMode& editorMode, const BuilderMode& builderMode);

			void								initialize();
			void								setScene(Scene::Ptr scene);
			void								setRenderContext(const RenderContext::Ptr& renderContext);
			void								setRenderTexture(const RenderTexturePtr& renderTexture);
			void								setResourceMananger(const ResourceManager::Ptr& resourceManager);
			void								setCamera(const Camera::Ptr& camera);
			void								setSetting(const Setting::Ptr& setting);

			void								renderScreenBuilder(sf::RenderTexture& texture);
			void								renderScene(sf::RenderTexture& texture);

			void								addGameLevel(const std::string& name = StringPool.BLANK);
			void								addWorldChunk(const std::string& name = StringPool.BLANK);
			void								addGameScreen(const std::string& name = StringPool.BLANK);

			void								setResourceManager(const ResourceManager::Ptr& resourceManager);
			void								addObject(Object::Type type, const sf::String& label, sf::Vector2f position, const EditorMode& editorMode);

			const std::vector<GameLevelPtr>&	getGameLevelTable();
			const std::vector<GameScreenPtr>&	getGameScreenTable();
			GameLevelPtr						getSelectedGameLevel();
			GameScreenPtr						getSelectedGameScreen();
			WorldChunkPtr						getSelectedWorldChunk();
			void								setSelectedGameLevel(GameLevelPtr gameLevel);
			void								setSelectedGameScreen(GameScreenPtr gameScreen);
			void								setSelectedWorldChunk(WorldChunkPtr worldChunk);
			SceneBuilder::Ptr					getSelectedSceneBuilder(const EditorMode& editorMode);
			void								playScene();

			//Mouse
			void								shiftMouseDown(const b2Vec2& p);
			void								mouseDown(const b2Vec2& p);
			void								mouseUp(const b2Vec2& p);
			void								mouseMove(const b2Vec2& p);
			//Bomb
			void								launchBomb();
			void								launchBomb(const b2Vec2& position, const b2Vec2& velocity);
			void								spawnBomb(const b2Vec2& worldPt);
			void								completeBombSpawn(const b2Vec2& p);
			void								destroyBomb();
			void								jointDestroyed(b2Joint* joint);

			void								setCppSceneCreator(boost::function<CreateCppSceneFn>& createCppScene);
			void								setProjectParameter(const Parameter& parameter);
			Scene::Ptr							createCppScene();

			//
			std::vector<std::string>			getGameLevelNameTable();
			std::vector<std::string>			getGameScreenNameTable();


		private:
			void								renderDebug();
			void								handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
			void								handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
			void								handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
			void								handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);

        private:
            //Friend
			friend class						DestructionListener;
			friend class						BoundaryListener;
			friend class						ContactListener;
			friend class						GameProject;

			std::vector<GameLevelPtr>			m_GameLevelTable;
			GameLevelPtr						m_SelectedGameLevel;
			WorldChunkPtr						m_SelectedWorldChunk;
			std::vector<GameScreenPtr>			m_GameScreenTable;
			SceneBuilder::Ptr					m_SelectedWorldBuilder;
			SceneBuilder::Ptr					m_SelectedScreenBuilder;
			GameScreenPtr						m_SelectedGameScreen;
			//
			Scene::Ptr							m_Scene;
			RenderContext::Ptr					m_RenderContext;
			ResourceManager::Ptr				m_ResourceManager;
			RenderTexturePtr					m_RenderTexture;
			Camera::Ptr							m_Camera;
			Setting::Ptr						m_EngineSetting;

			//
			int									m_GameScreenCount;
			int									m_GameLevelCount;

			//
			DestructionListener					m_DestructionListener;
			sf::String							m_Message;
			sf::String							m_StatMessage;
			sf::String							m_ProfileMessage;
			b2Body*								m_Bomb;
			b2Vec2								m_BombSpawnPoint;
			bool								m_BombSpawning;
			b2Body*								m_GroundBody;
			b2AABB								m_WorldAABB;
			b2MouseJoint*						m_MouseJoint;
			b2Vec2								m_MouseWorld;
			int32								m_StepCount;
			b2Profile							m_MaxProfile;
			b2Profile							m_TotalProfile;

			//
			bool                                m_IsShiftOriginUp;
			bool                                m_IsShiftOriginDown;
			bool                                m_IsShiftOriginLeft;
			bool                                m_IsShiftOriginRight;
			bool                                m_IsLeftShift;
			bool                                m_IsMouseRightButton;
			b2Vec2                              m_LastMousePosition;
			b2Vec2                              m_ViewCenter;
			float                               m_ShitftOriginSpeed;

			//
			boost::function<CreateCppSceneFn>	m_CreateCppScene;
			Parameter								m_ProjectParameter;
    };
}*/

#endif // ADVANCEDSCENE_H
