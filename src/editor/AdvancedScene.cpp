////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/editor/AdvancedScene.h>
////////////////////////////////////////////////////////////
/*#include <Nero/core/cpp/utility/Math.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/object/GameLevelObject.h>
#include <SFML/Graphics/Sprite.hpp>*/
////////////////////////////////////////////////////////////
namespace nero
{
	AdvancedScene::AdvancedScene():
		 m_SelectedGameLevel(nullptr)
		,m_SelectedGameScreen(nullptr)
		,m_GameLevelTable()
		,m_GameScreenTable()
	{

	}

	GameLevelBuilder::Ptr AdvancedScene::addGameLevel(const Parameter& parameter)
	{
		auto gameLevelBuilder = std::make_shared<GameLevelBuilder>();

		return gameLevelBuilder;
	}

	GameScreenBuilder::Ptr AdvancedScene::addGameScreen(const Parameter& parameter)
	{
		auto gameScreenBuilder = std::make_shared<GameScreenBuilder>();

		return gameScreenBuilder;
	}

}

/*namespace nero
{
    ////////////////////////////////////////////////////////////
   //DestructionListener
   void DestructionListener::SayGoodbye(b2Fixture* fixture)
   {
       B2_NOT_USED(fixture);
   }

   void DestructionListener::SayGoodbye(b2Joint* joint)
   {
       if (scene->m_MouseJoint == joint)
       {
           scene->m_MouseJoint = nullptr;
       }
       else
       {
		   scene->jointDestroyed(joint);
       }
   }

	AdvancedScene::AdvancedScene():
		 m_RenderTexture(nullptr)
		,m_GameScreenCount(0)
		,m_GameLevelCount(0)
		,m_IsLeftShift(false)
		,m_IsMouseRightButton(false)
		,m_IsShiftOriginUp(false)
		,m_IsShiftOriginDown(false)
		,m_IsShiftOriginLeft(false)
		,m_IsShiftOriginRight(false)
		,m_ShitftOriginSpeed(0.5f)
		,m_ViewCenter(0.0f, 20.0f)
		,m_MouseJoint(nullptr)
		,m_GroundBody(nullptr)
		,m_BombSpawning(false)
		,m_Bomb(nullptr)
		,m_BombSpawnPoint(0.f, 0.f)
		,m_WorldAABB()
		,m_MouseWorld(0.f, 0.f)
    {
		m_DestructionListener.scene = AdvancedScene::Ptr(this);

		memset(&m_MaxProfile, 0, sizeof(b2Profile));
		memset(&m_TotalProfile, 0, sizeof(b2Profile));
    }

	void AdvancedScene::addGameLevel(const std::string& name)
	{
		m_GameLevelCount++;

		std::string levelName = name;
		if(levelName == StringPool.BLANK)
		{
			levelName = "Game Level " + toString(m_GameLevelCount);
		}

		m_GameLevelTable.push_back(std::make_shared<GameLevel>());
		m_SelectedGameLevel = m_GameLevelTable.back();
		m_SelectedGameLevel->name = levelName;
		m_SelectedGameLevel->levelId = m_GameLevelCount;

		addWorldChunk("Game Start");
	}

	void AdvancedScene::addObject(Object::Type type, const sf::String& label, sf::Vector2f position, const EditorMode& editorMode)
	{
		if(editorMode == EditorMode::WORLD_BUILDER)
		{
			m_SelectedWorldBuilder->addObject(type, label, position);
		}
		else if(editorMode == EditorMode::WORLD_BUILDER)
		{
			m_SelectedScreenBuilder->addObject(type, label, position);
		}
	}

	void AdvancedScene::addWorldChunk(const std::string& name)
	{
		if(m_SelectedGameLevel)
		{
			m_SelectedGameLevel->chunkCount++;

			std::string chunkName = name;

			if(chunkName == StringPool.BLANK)
			{
				chunkName = "World Chunk " + toString(m_SelectedGameLevel->chunkCount);
			}

			m_SelectedGameLevel->chunkTable.push_back(std::make_shared<WorldChunk>());
			m_SelectedWorldChunk = m_SelectedGameLevel->chunkTable.back();
			m_SelectedWorldChunk->chunkId = m_SelectedGameLevel->chunkCount;
			m_SelectedWorldChunk->name = chunkName;
			m_SelectedWorldBuilder = m_SelectedWorldChunk->sceneBuilder;
			m_SelectedWorldBuilder->setResourceManager(m_ResourceManager);
			m_SelectedWorldBuilder->setRenderTexture(m_RenderTexture);
			m_SelectedWorldBuilder->setRenderContext(m_RenderContext);
			m_SelectedWorldBuilder->addLayer();
			m_SelectedGameLevel->setSelectedWorldChunk(m_SelectedWorldChunk->chunkId);
		}
	}

	void AdvancedScene::addGameScreen(const std::string& name)
	{
		m_GameScreenCount++;

		std::string screenName = name;
		if(screenName == StringPool.BLANK)
		{
			screenName = "Game Screen " + toString(m_GameScreenCount);
		}


		m_GameScreenTable.push_back(std::make_shared<GameScreen>());
		m_SelectedGameScreen = m_GameScreenTable.back();
		m_SelectedGameScreen->screenId = m_GameScreenCount;
		m_SelectedGameScreen->name = screenName;
		m_SelectedScreenBuilder = m_SelectedGameScreen->sceneBuilder;
		m_SelectedScreenBuilder->setResourceManager(m_ResourceManager);
		m_SelectedScreenBuilder->setRenderTexture(m_RenderTexture);
		m_SelectedScreenBuilder->setRenderContext(m_RenderContext);
		m_SelectedScreenBuilder->addLayer();
	}

	void AdvancedScene::handleEvent(const sf::Event& event, const EditorMode& editorMode, const BuilderMode& builderMode)
    {
		if(editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::OBJECT)
		{
			m_SelectedWorldBuilder->handleEvent(event);
		}
		else if(editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::MESH)
		{
			m_SelectedWorldBuilder->getMeshEditor()->handleEvent(event);
		}
		else if(editorMode == EditorMode::SCREEN_BUILDER && builderMode == BuilderMode::OBJECT)
		{
			m_SelectedScreenBuilder->handleEvent(event);
		}
		else if(editorMode == EditorMode::PLAY_GAME && m_Scene)
		{
			m_Scene->handleEvent(event);

			switch(event.type)
			{
				//Keyboard events
				case sf::Event::KeyPressed:
					handleKeyboardInput(event.key.code, true);
					break;
				case sf::Event::KeyReleased:
					handleKeyboardInput(event.key.code, false);
					break;

				//Mouse buttons events
				case sf::Event::MouseButtonPressed:
					handleMouseButtonInput(event.mouseButton, true);
					break;
				case sf::Event::MouseButtonReleased:
					handleMouseButtonInput(event.mouseButton, false);
					break;

				//Mouse mouvements event
				case sf::Event::MouseMoved:
					handleMouseMoveInput(event.mouseMove);
					break;
			}
		}
    }

	void AdvancedScene::update(const sf::Time& timeStep, const EditorMode& editorMode, const BuilderMode& builderMode)
    {
		if(editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::OBJECT)
		{
			m_SelectedWorldBuilder->update(timeStep);
		}
		else if(editorMode == EditorMode::SCREEN_BUILDER && builderMode == BuilderMode::OBJECT)
		{
			m_SelectedScreenBuilder->update(timeStep);
		}
		else if(editorMode == EditorMode::PLAY_GAME && m_Scene)
		{
			m_Scene->update(timeStep);

			m_Message = m_StatMessage + m_ProfileMessage + m_Scene->m_InformationContent;
			m_Scene->m_InformationText.setString(m_Message);
		}

	}

	void AdvancedScene::render(const EditorMode& editorMode, const BuilderMode& builderMode)
	{
		if(editorMode == EditorMode::WORLD_BUILDER)
		{
			for(const auto& worldChunk : m_SelectedGameLevel->chunkTable)
			{
				if(worldChunk->visible)
				{
					worldChunk->sceneBuilder->render();
				}
			}

		}
		else if(editorMode == EditorMode::SCREEN_BUILDER)
		{
			m_SelectedScreenBuilder->render();
		}
		else if(editorMode == EditorMode::PLAY_GAME && m_Scene)
		{
			m_Scene->render();
			m_Scene->renderShape();

			if(sf::Shader::isAvailable() && m_Scene->m_LevelSetting->getBool("enable_lighting") && m_Scene->m_LevelSetting->getBool("lighting_on"))
			{
				m_Scene->renderLighting();
			}

			renderDebug();
		}

	}

	void AdvancedScene::renderFrontScreen(const EditorMode& editorMode, const BuilderMode& builderMode)
	{
		if(editorMode == EditorMode::PLAY_GAME && m_Scene)
		{
			m_Scene->renderFrontScreen();
		}
	}


	void AdvancedScene::setScene(Scene::Ptr scene)
	{
		m_Scene = scene;
    }

	void AdvancedScene::setRenderContext(const RenderContext::Ptr& renderContext)
	{
		m_RenderContext = renderContext;
	}

	void AdvancedScene::setRenderTexture(const RenderTexturePtr& renderTexture)
	{
		m_RenderTexture = renderTexture;
	}

	void AdvancedScene::setResourceManager(const ResourceManager::Ptr& resourceManager)
	{
		m_ResourceManager = resourceManager;
	}

	void AdvancedScene::setCamera(const Camera::Ptr& camera)
	{
	   m_Camera = camera;
	}

	void AdvancedScene::initialize()
	{
		addGameLevel("Game Start");
		addGameScreen("Start Screen");
	}

	const std::vector<AdvancedScene::GameLevelPtr>& AdvancedScene::getGameLevelTable()
	{
		return m_GameLevelTable;
	}

	const std::vector<AdvancedScene::GameScreenPtr>& AdvancedScene::getGameScreenTable()
	{
		return m_GameScreenTable;
	}


	AdvancedScene::GameLevelPtr AdvancedScene::getSelectedGameLevel()
	{
		return m_SelectedGameLevel;
	}

	AdvancedScene::GameScreenPtr AdvancedScene::getSelectedGameScreen()
	{
		return m_SelectedGameScreen;
	}

	void AdvancedScene::setSetting(const Setting::Ptr& setting)
	{
		m_EngineSetting = setting;
	}

	AdvancedScene::WorldChunkPtr AdvancedScene::getSelectedWorldChunk()
	{
		return m_SelectedWorldChunk;
	}

	void AdvancedScene::setSelectedGameLevel(AdvancedScene::GameLevelPtr gameLevel)
	{
		m_SelectedGameLevel		= gameLevel;
		m_SelectedWorldChunk	= gameLevel->getSelectedWorldChunk();
		m_SelectedWorldBuilder	= m_SelectedWorldChunk->sceneBuilder;
	}

	void AdvancedScene::setSelectedGameScreen(AdvancedScene::GameScreenPtr gameScreen)
	{
		m_SelectedGameScreen	= gameScreen;
		m_SelectedWorldBuilder	= m_SelectedGameScreen->sceneBuilder;
	}

	void AdvancedScene::setSelectedWorldChunk(AdvancedScene::WorldChunkPtr worldChunk)
	{
		m_SelectedWorldChunk	= worldChunk;
		m_SelectedWorldBuilder	= worldChunk->sceneBuilder;
		m_SelectedGameLevel->setSelectedWorldChunk(worldChunk->chunkId);
	}

	SceneBuilder::Ptr AdvancedScene::getSelectedSceneBuilder(const EditorMode& editorMode)
	{
		if(editorMode == EditorMode::SCREEN_BUILDER)
		{
			return m_SelectedScreenBuilder;
		}
		else
		{
			return m_SelectedWorldBuilder;
		}
	}

	void AdvancedScene::playScene()
	{
		if(m_Scene)
		{
			nero_log("cleaning game scene");
			//destroy current scene

			m_Scene = nullptr;
		}


		m_Scene = createCppScene();

		//set level setting
		m_Scene->m_LevelSetting = m_SelectedGameLevel->levelSetting;

		//enable lighting
		if(sf::Shader::isAvailable() && m_Scene->m_LevelSetting->getBool("enable_lighting"))
		{
			m_Scene->setupLighting();
		}

		//ceate level ojbect
		auto gameLevel = std::make_shared<GameLevelObject>();
		gameLevel->setName(m_SelectedGameLevel->name);
		Parameter parameter;
		gameLevel->init(parameter);

		//set up physic world
		m_Scene->m_PhysicWorld = gameLevel->getPhysicWorld();
		m_Scene->m_PhysicWorld->SetContactListener(m_Scene.get());
		m_Scene->m_PhysicWorld->SetDebugDraw(&(m_Scene->m_ShapeRenderer));

		for(auto worldChunk : m_SelectedGameLevel->chunkTable)
		{
			auto chunkObject = std::make_shared<Object>();
			chunkObject->setName(worldChunk->name);

			if(m_Scene->m_LightManager)
			{
				worldChunk->sceneBuilder->setLightManager(m_Scene->m_LightManager);
			}

			worldChunk->sceneBuilder->setPhysicWorld(m_Scene->m_PhysicWorld);
			worldChunk->sceneBuilder->buildScene(chunkObject);

			gameLevel->addChild(chunkObject);
		}

		m_Scene->m_PhysicWorld->SetDestructionListener(&m_DestructionListener);
		b2BodyDef bodyDef;
		m_GroundBody = m_Scene->m_PhysicWorld->CreateBody(&bodyDef);

		m_Scene->m_GameWorld->addChild(gameLevel);
	}

	void AdvancedScene::shiftMouseDown(const b2Vec2& p)
	{
		m_MouseWorld = p;

		if (m_MouseJoint)
		{
			return;
		}

		spawnBomb(p);
	}


	void AdvancedScene::mouseDown(const b2Vec2& p)
	{
		m_MouseWorld = p;

		if (m_MouseJoint)
		{
			return;
		}

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;

		// Query the world for overlapping shapes.
		QueryCallback callback(p);
		m_Scene->m_PhysicWorld->QueryAABB(&callback, aabb);

		if (callback.m_Fixture)
		{
			b2Body* body = callback.m_Fixture->GetBody();
			b2MouseJointDef md;
			md.bodyA = m_GroundBody;
			md.bodyB = body;
			md.target = p;
			md.maxForce = 1000.0f * body->GetMass();
			m_MouseJoint = (b2MouseJoint*)m_Scene->m_PhysicWorld->CreateJoint(&md);
			body->SetAwake(true);
		}
	}

	void AdvancedScene::mouseUp(const b2Vec2& p)
	{
		if (m_MouseJoint)
		{
			m_Scene->m_PhysicWorld->DestroyJoint(m_MouseJoint);
			m_MouseJoint = nullptr;
		}

		if (m_BombSpawning)
		{
			completeBombSpawn(p);
		}
	}

	void AdvancedScene::mouseMove(const b2Vec2& p)
	{
		m_MouseWorld = p;

		if (m_MouseJoint != nullptr)
		{
			m_MouseJoint->SetTarget(p);
		}
	}


	void AdvancedScene::spawnBomb(const b2Vec2& worldPt)
	{
		m_BombSpawnPoint = worldPt;
		m_BombSpawning = true;
	}

	void AdvancedScene::completeBombSpawn(const b2Vec2& p)
	{
		if (m_BombSpawning == false)
		{
			return;
		}

		const float multiplier = 30.0f;
		b2Vec2 vel = m_BombSpawnPoint - p;
		vel *= multiplier;
		launchBomb(m_BombSpawnPoint,vel);
		m_BombSpawning = false;
	}

	void AdvancedScene::launchBomb()
	{
		b2Vec2 p(math::randomFloat(-15.0f, 15.0f), -30.0f);
		b2Vec2 v = -5.0f * p;
		launchBomb(p, v);
	}

	void AdvancedScene::destroyBomb()
	{
		if (m_Bomb)
		{
			m_Scene->m_PhysicWorld->DestroyBody(m_Bomb);
			m_Bomb = nullptr;
		}
	}


	void AdvancedScene::launchBomb(const b2Vec2& position, const b2Vec2& velocity)
	{
		if (m_Bomb != nullptr)
		{
			m_Scene->m_PhysicWorld->DestroyBody(m_Bomb);
			m_Bomb = nullptr;
		}

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = position;
		bd.bullet = true;
		m_Bomb = m_Scene->m_PhysicWorld->CreateBody(&bd);
		m_Bomb->SetLinearVelocity(velocity);

		b2CircleShape circle;
		circle.m_radius = 0.2f;

		b2FixtureDef fd;
		fd.shape = &circle;
		fd.density = 20.0f;
		fd.restitution = 0.0f;

		b2Vec2 minV = position - b2Vec2(0.3f,0.3f);
		b2Vec2 maxV = position + b2Vec2(0.3f,0.3f);

		b2AABB aabb;
		aabb.lowerBound = minV;
		aabb.upperBound = maxV;

		m_Bomb->CreateFixture(&fd);
	}

	void AdvancedScene::jointDestroyed(b2Joint* joint)
	{
		B2_NOT_USED(joint);
	}

	void AdvancedScene::setCppSceneCreator(boost::function<CreateCppSceneFn>& createCppScene)
	{
		m_CreateCppScene = createCppScene;
	}

	Scene::Ptr AdvancedScene::createCppScene()
	{
		return  m_CreateCppScene(Scene::Context(
					 m_ProjectParameter.getString("project_name"),
					 m_RenderTexture,
					 m_ResourceManager,
					 m_Camera,
					 m_EngineSetting,
					 Scene::EngineType::EDITOR_ENGINE,
					 Scene::PlatformType::WINDOWS));
	}

	void AdvancedScene::setProjectParameter(const Parameter& parameter)
	{
		m_ProjectParameter = parameter;
	}

	void AdvancedScene::renderDebug()
	{
		if(m_Scene->m_LevelSetting->getBool("draw_statistic"))
		{
			//bodies/contacts/joints
			int32 bodyCount     = m_Scene->m_PhysicWorld->GetBodyCount();
			int32 contactCount  = m_Scene->m_PhysicWorld->GetContactCount();
			int32 jointCount    = m_Scene->m_PhysicWorld->GetJointCount();

			//proxies/height/balance/quality
			int32 proxyCount    = m_Scene->m_PhysicWorld->GetProxyCount();
			int32 height        = m_Scene->m_PhysicWorld->GetTreeHeight();
			int32 balance       = m_Scene->m_PhysicWorld->GetTreeBalance();
			float32 quality     = m_Scene->m_PhysicWorld->GetTreeQuality();

			m_StatMessage = "body/contact/joint = " + toString(bodyCount) + " / " +  toString(contactCount) + " / " + toString(jointCount) + "\n" \
							"proxy/height/balance/quality = " + toString(proxyCount) + " / " + toString(height) + " / " +  toString(balance) + " / " + toString(quality) + "\n";
		}
		else
		{
			m_StatMessage = StringPool.BLANK;
		}

		// Track maximum profile times
		{
			const b2Profile& p = m_Scene->m_PhysicWorld->GetProfile();

			m_MaxProfile.step               = b2Max(m_MaxProfile.step, p.step);
			m_MaxProfile.collide            = b2Max(m_MaxProfile.collide, p.collide);
			m_MaxProfile.solve              = b2Max(m_MaxProfile.solve, p.solve);
			m_MaxProfile.solveInit          = b2Max(m_MaxProfile.solveInit, p.solveInit);
			m_MaxProfile.solveVelocity      = b2Max(m_MaxProfile.solveVelocity, p.solveVelocity);
			m_MaxProfile.solvePosition      = b2Max(m_MaxProfile.solvePosition, p.solvePosition);
			m_MaxProfile.solveTOI           = b2Max(m_MaxProfile.solveTOI, p.solveTOI);
			m_MaxProfile.broadphase         = b2Max(m_MaxProfile.broadphase, p.broadphase);

			m_TotalProfile.step             += p.step;
			m_TotalProfile.collide          += p.collide;
			m_TotalProfile.solve            += p.solve;
			m_TotalProfile.solveInit        += p.solveInit;
			m_TotalProfile.solveVelocity    += p.solveVelocity;
			m_TotalProfile.solvePosition    += p.solvePosition;
			m_TotalProfile.solveTOI         += p.solveTOI;
			m_TotalProfile.broadphase       += p.broadphase;
		}

		if(m_Scene->m_LevelSetting->getBool("draw_profile"))
		{
			const b2Profile& p = m_Scene->m_PhysicWorld->GetProfile();

			b2Profile aveProfile;
			memset(&aveProfile, 0, sizeof(b2Profile));

			if (m_StepCount > 0)
			{
				float32 scale = 1.0f / m_StepCount;

				aveProfile.step             = scale * m_TotalProfile.step;
				aveProfile.collide          = scale * m_TotalProfile.collide;
				aveProfile.solve            = scale * m_TotalProfile.solve;
				aveProfile.solveInit        = scale * m_TotalProfile.solveInit;
				aveProfile.solveVelocity    = scale * m_TotalProfile.solveVelocity;
				aveProfile.solvePosition    = scale * m_TotalProfile.solvePosition;
				aveProfile.solveTOI         = scale * m_TotalProfile.solveTOI;
				aveProfile.broadphase       = scale * m_TotalProfile.broadphase;
			}

			m_ProfileMessage =  "step [ave] (max) = "           + toString(p.step)          + " [" + toString(aveProfile.step)          + "]" +  "(" + toString(m_MaxProfile.step)          + ") " + "\n" \
								"collide [ave] (max) = "        + toString(p.collide)       + " [" + toString(aveProfile.collide)       + "]" +  "(" + toString(m_MaxProfile.collide)       + ")" + "\n" \
								"solve [ave] (max) = "          + toString(p.solve)         + " [" + toString(aveProfile.solve)         + "]" +  "(" + toString(m_MaxProfile.solve)         + ")" + "\n" \
								"solve init [ave] (max) = "     + toString(p.solveInit)     + " [" + toString(aveProfile.solveInit)     + "]" +  "(" + toString(m_MaxProfile.solveInit)     + ")" + "\n" \
								"solve velocity [ave] (max) = " + toString(p.solveVelocity) + " [" + toString(aveProfile.solveVelocity) + "]" +  "(" + toString(m_MaxProfile.solveVelocity) + ")" + "\n" \
								"solve position [ave] (max) = " + toString(p.solvePosition) + " [" + toString(aveProfile.solvePosition) + "]" +  "(" + toString(m_MaxProfile.solvePosition) + ")" + "\n" \
								"solveTOI [ave] (max) = "       + toString(p.solveTOI)      + " [" + toString(aveProfile.solveTOI)      + "]" +  "(" + toString(m_MaxProfile.solveTOI)      + ")" + "\n" \
								"broad-phase [ave] (max) = "    + toString(p.broadphase)    + " [" + toString(aveProfile.broadphase)    + "]" +  "(" + toString(m_MaxProfile.broadphase)    + ")" + "\n";
		}
		else
		{
			m_ProfileMessage = StringPool.BLANK;
		}


		if (m_MouseJoint)
		{
			b2Vec2 p1 = m_MouseJoint->GetAnchorB();
			b2Vec2 p2 = m_MouseJoint->GetTarget();

			b2Color c;
			c.Set(0.0f, 1.0f, 0.0f);
			m_Scene->m_ShapeRenderer.DrawPoint(p1, 4.0f, c);
			m_Scene->m_ShapeRenderer.DrawPoint(p2, 4.0f, c);

			c.Set(0.8f, 0.8f, 0.8f);
			m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, c);
		}

		if (m_BombSpawning)
		{
			b2Color c;
			c.Set(0.0f, 0.0f, 1.0f);
			m_Scene->m_ShapeRenderer.DrawPoint(m_BombSpawnPoint, 4.0f, c);

			c.Set(0.8f, 0.8f, 0.8f);
			m_Scene->m_ShapeRenderer.DrawSegment(m_MouseWorld, m_BombSpawnPoint, c);
		}

		if(m_Scene->m_LevelSetting->getBool("draw_contact_point"))
		{
			const float32 k_impulseScale = 0.1f;
			const float32 k_axisScale = 0.3f;

			for (int32 i = 0; i < m_Scene->m_ContactPointCount; ++i)
			{
				ContactPoint* point = m_Scene->m_ContactVectorTablele + i;

				if (point->state == b2_addState)
				{
					// Add
					m_Scene->m_ShapeRenderer.DrawPoint(point->position, 10.0f, b2Color(0.3f, 0.95f, 0.3f));
				}
				else if (point->state == b2_persistState)
				{
					// Persist
					m_Scene->m_ShapeRenderer.DrawPoint(point->position, 5.0f, b2Color(0.3f, 0.3f, 0.95f));
				}

				if(m_Scene->m_LevelSetting->getBool("draw_contact_normal"))
				{
					b2Vec2 p1 = point->position;
					b2Vec2 p2 = p1 + k_axisScale * point->normal;
					m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
				}

				if(m_Scene->m_LevelSetting->getBool("draw_contact_impulse"))
				{
					b2Vec2 p1 = point->position;
					b2Vec2 p2 = p1 + k_impulseScale * point->normalImpulse * point->normal;
					m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
				}

				if(m_Scene->m_LevelSetting->getBool("draw_friction_impulse"))
				{
					b2Vec2 tangent = b2Cross(point->normal, 1.0f);
					b2Vec2 p1 = point->position;
					b2Vec2 p2 = p1 + k_impulseScale * point->tangentImpulse * tangent;
					m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
				}
			}
		}
	}

	void AdvancedScene::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
	{
		//Handle only key pressed inputs
		if(isPressed)
		{
			if(key == sf::Keyboard::B)
				launchBomb();
		}

		if(key == sf::Keyboard::LShift)
			m_IsLeftShift = isPressed;
	}

	void AdvancedScene::handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
	{
		sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(m_RenderContext->mouse_position.x, m_RenderContext->mouse_position.y), m_RenderTexture->getView());
		b2Vec2 p = graphics::sf_to_b2(world_pos, EngineConstant.SCALE);

		if(mouse.button == sf::Mouse::Left && isPressed == true)
		{
			if(m_IsLeftShift)
				shiftMouseDown(p);
			else
				mouseDown(p);
		}
		else if (mouse.button == sf::Mouse::Left && isPressed == false)
		{
			mouseUp(p);
		}
		else if (mouse.button == sf::Mouse::Right)
		{
			if(isPressed)
			{
				m_LastMousePosition = p;

			}

			m_IsMouseRightButton = isPressed;
		}

	}

	void AdvancedScene::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
	{
		sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(m_RenderContext->mouse_position.x, m_RenderContext->mouse_position.y), m_RenderTexture->getView());
		b2Vec2 p = graphics::sf_to_b2(world_pos, EngineConstant.SCALE);

		mouseMove(p);
	}

	void AdvancedScene::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
	{

	}

	std::vector<std::string> AdvancedScene::getGameLevelNameTable()
	{
		std::vector<std::string> result;

		for(auto level : m_GameLevelTable)
		{
			result.push_back(level->name);
		}

		return result;
	}

	std::vector<std::string> AdvancedScene::getGameScreenNameTable()
	{
		std::vector<std::string> result;

		for(auto screen : m_GameScreenTable)
		{
			result.push_back(screen->name);
		}

		return result;
	}

}*/

