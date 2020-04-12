////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
//Boost
#include <boost/dll.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
	Scene::Context::Context(std::string sceneName, RenderTexturePtr renderTexture, ResourceManager::Ptr resourceManager, Camera::Ptr camera, Setting::Ptr setting, EngineType engineType, PlatformType platformType):
		 sceneName(sceneName)
		,renderTexture(renderTexture)
		,resourceManager(resourceManager)
		,camera(camera)
		,setting(setting)
		,engineType(engineType)
		,platformType(platformType)

	{

    }

	Scene::Context::Context()
	{

	}

	Scene::Scene(Context context):
		 m_SceneContext(context)
		,m_GameWorld(std::make_shared<Object>())
		,m_PhysicWorld(nullptr)
		,m_ShapeRenderer(context.renderTexture)
		,m_HideWorld(false)
		,m_LevelSetting(nullptr)
		,m_InformationText()
		,m_LightManager(nullptr)
		,m_GameSetting(context.setting)
	{
		if(context.resourceManager)
		{
			m_InformationText.setFont(context.resourceManager->getFontHolder()->getDefaultFont());
			m_InformationText.setCharacterSize(15.f);
			m_InformationText.setFillColor(sf::Color::White);
			m_InformationText.setPosition(sf::Vector2f(50.f, 10.f));
		}

		m_QuitEngine = [](){};

		//TODO remve
		//demo
		Setting viewCenter;
		viewCenter.setFloat("x", 0.f);
		viewCenter.setFloat("y", 0.f);
		Setting gravity;
		gravity.setFloat("x", 0.f);
		gravity.setFloat("y", 9.8f);
		m_LevelSetting = std::make_shared<Setting>();
		m_LevelSetting->setSetting("gravity", gravity);
		m_LevelSetting->setSetting("view_center", viewCenter);
		m_LevelSetting->setFloat("time_step", 40.f);
		m_LevelSetting->setUInt("velocity_iteration", 8);
		m_LevelSetting->setUInt("position_iteration", 3);
		//
		m_LevelSetting->setBool("draw_axis", true);
		m_LevelSetting->setBool("draw_grid", true);
		m_LevelSetting->setBool("draw_shape", true);
		m_LevelSetting->setBool("draw_joint", true);
		m_LevelSetting->setBool("draw_aabb", false);
		m_LevelSetting->setBool("draw_contact_point", false);
		m_LevelSetting->setBool("draw_contact_normal", false);
		m_LevelSetting->setBool("draw_contact_impulse", false);
		m_LevelSetting->setBool("draw_friction_impulse", false);
		m_LevelSetting->setBool("draw_center_of_mass", false);
		m_LevelSetting->setBool("draw_statistic", false);
		m_LevelSetting->setBool("draw_profile", false);
		//
		m_LevelSetting->setBool("enable_warm_starting", true);
		m_LevelSetting->setBool("enable_continous", true);
		m_LevelSetting->setBool("enable_sub_stepping", false);
		m_LevelSetting->setBool("enable_sleep", true);
		//
		m_LevelSetting->setBool("pause_level", false);
		m_LevelSetting->setBool("single_step", false);
		//
		m_LevelSetting->setBool("enable_lighting", true);
		m_LevelSetting->setBool("lighting_on", true);

		m_PhysicWorld = new b2World(b2Vec2(m_LevelSetting->getSetting("gravity").getFloat("x"),
										   m_LevelSetting->getSetting("gravity").getFloat("y")));
		m_PhysicWorld->SetContactListener(this);
		m_PhysicWorld->SetDebugDraw(&m_ShapeRenderer);
	}

	Scene::~Scene()
    {
		destroy();
    }

	void Scene::destroy()
	{
		//destroy physic world
		m_PhysicWorld = nullptr;
		//delete m_PhysicWorld;
	}

	void Scene::handleEvent(const sf::Event& event)
	{
		switch(event.type)
		{
			//keyboard events
			case sf::Event::KeyPressed:
				handleKeyboardInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				handleKeyboardInput(event.key.code, false);
				break;

			//mouse buttons events
			case sf::Event::MouseButtonPressed:
				handleMouseButtonInput(event.mouseButton, true);
				break;
			case sf::Event::MouseButtonReleased:
				handleMouseButtonInput(event.mouseButton, false);
				break;

			//mouse move event
			case sf::Event::MouseMoved:
				handleMouseMoveInput(event.mouseMove);
				break;

			 //mouse wheel
			case sf::Event::MouseWheelScrolled:
				handleMouseWheelInput(event.mouseWheelScroll);
				break;
		}
	}

	void Scene::update(const sf::Time& timeStep)
	{
		float32 b2TimeStep = m_LevelSetting->getFloat("time_step") > 0.0f ? 1.0f / m_LevelSetting->getFloat("time_step") : float32(0.0f);

		if(b2TimeStep > 0.f)
		{
			b2TimeStep = (b2TimeStep * timeStep.asSeconds())/EngineConstant.TIME_PER_FRAME.asSeconds();
		}

		if(!m_HideWorld)
		{
			if(!m_PhysicWorld->IsLocked())
			{
				//m_ObjectManager->removeDeadPhysicObject();
			}

			if(m_LevelSetting->getBool("pause_level") && !m_LevelSetting->getBool("single_step"))
			{
				b2TimeStep = 0.0f;
				m_SceneContext.engineType == EngineType::RENDER_ENGINE ? m_InformationContent = string::StringPool.BLANK : m_InformationContent = "#-- PAUSED --#";
			}
			else
			{
				m_InformationContent = string::StringPool.BLANK;
			}

			uint32 flags = 0;
			flags += m_LevelSetting->getUInt("draw_shape") * b2Draw::e_shapeBit;
			flags += m_LevelSetting->getUInt("draw_joint") * b2Draw::e_jointBit;
			flags += m_LevelSetting->getUInt("draw_aabb") * b2Draw::e_aabbBit;
			flags += m_LevelSetting->getUInt("draw_center_of_mass") * b2Draw::e_centerOfMassBit;

			m_ShapeRenderer.SetFlags(flags);

			m_PhysicWorld->SetAllowSleeping(m_LevelSetting->getBool("enable_sleep"));
			m_PhysicWorld->SetWarmStarting(m_LevelSetting->getBool("enable_warm_starting"));
			m_PhysicWorld->SetContinuousPhysics(m_LevelSetting->getBool("enable_sub_stepping"));
			m_PhysicWorld->SetSubStepping(m_LevelSetting->getBool("enable_sub_stepping"));

			m_ContactPointCount = 0;

			m_PhysicWorld->Step(b2TimeStep, m_LevelSetting->getInt("velocity_iteration"), m_LevelSetting->getInt("position_iteration"));


			if(!m_LevelSetting->getBool("pause_level") || m_LevelSetting->getBool("single_step"))
			{
				m_GameWorld->update(sf::seconds(b2TimeStep));
			}

			m_InformationText.setString(m_InformationContent);
		}
	}

	void Scene::render()
	{
		return;

		auto gameLevel = m_GameWorld->getAllChild()->front();

		auto chunkTable = gameLevel->getAllChild();

		for(auto it = chunkTable->begin(); it != chunkTable->end(); it++)
		{
			auto childTable = (*it)->getAllChild();

			for(auto it = childTable->begin(); it != childTable->end(); it++)
			{
				LayerObject::Ptr layer_object = LayerObject::Cast(*it);

				if(layer_object->isVisible())
					m_SceneContext.renderTexture->draw(*layer_object);
			}
		}
	}

	void Scene::renderShape()
	{
		m_PhysicWorld->DrawDebugData();
	}

	void Scene::renderFrontScreen()
	{
		 m_SceneContext.renderTexture->draw(m_InformationText);
	}

	void Scene::init()
	{
	   //empty
	}

	void Scene::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
	{
	   //empty
	}

	void Scene::handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
	{
	   //empty
	}

	void Scene::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
	{
	   //empty
	}

	void Scene::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
	{
	   //empty
	}

	void Scene::handleCollisionContactBegin(Collision collision)
	{
	   //empty
	}


	void Scene::handleCollisionContactEnd(Collision collision)
	{
	   //empty
	}

	void Scene::handleCollisionPreSolveContact(Collision collision)
	{
	   //empty
	}


	void Scene::handleCollisionPostSolveContact(Collision collision)
	{
	   //empty
	}


	void Scene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		return;

		const b2Manifold* manifold = contact->GetManifold();

		if (manifold->pointCount == 0)
		{
		  return;
		}

		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
		b2GetPointStates(state1, state2, oldManifold, manifold);

		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		for (int32 i = 0; i < manifold->pointCount && m_ContactPointCount < MAX_CONTACT_POINT; ++i)
		{
			ContactPoint* cp = m_ContactVectorTablele + m_ContactPointCount;
			cp->fixtureA = fixtureA;
			cp->fixtureB = fixtureB;
			cp->position = worldManifold.points[i];
			cp->normal = worldManifold.normal;
			cp->state = state2[i];
			cp->normalImpulse = manifold->points[i].normalImpulse;
			cp->tangentImpulse = manifold->points[i].tangentImpulse;
			cp->separation = worldManifold.separations[i];
			++m_ContactPointCount;
		}

		/*Collision collision(contact, oldManifold, nullptr);

		int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
		int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

		auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
		auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

		if(!objectA || !objectB)
		  return;

		if(objectA->isDead() || objectB->isDead())
		  return;

		collision.setObjectA(objectA);
		collision.setObjectB(objectB);

		handleCollisionPreSolveContact(collision);*/
	}


	void Scene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		return;
		/*Collision collision(contact, nullptr, impulse);

		int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
		int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

		auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
		auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

		if(!objectA || !objectB)
		  return;

		if(objectA->isDead() || objectB->isDead())
		  return;

		collision.setObjectA(objectA);
		collision.setObjectB(objectB);

		handleCollisionPostSolveContact(collision);*/
	}


	void Scene::BeginContact(b2Contact* contact)
	{
		return;
		/*Collision collision(contact, nullptr, nullptr);

		int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
		int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

		auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
		auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

		if(!objectA || !objectB)
		  return;

		if(objectA->isDead() || objectB->isDead())
		  return;

		collision.setObjectA(objectA);
		collision.setObjectB(objectB);

		handleCollisionContactBegin(collision);*/
	}

	void Scene::EndContact(b2Contact* contact)
	{
		return;
		/*Collision collision(contact, nullptr, nullptr);

		int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
		int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

		auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
		auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

		if(!objectA || !objectB)
		  return;

		if(objectA->isDead() || objectB->isDead())
		  return;

		collision.setObjectA(objectA);
		collision.setObjectB(objectB);

		handleCollisionContactEnd(collision);*/
	}

	void Scene::pauseScene()
	{

	}

	void Scene::resumeScene()
	{

	}

	void Scene::resetScene()
	{

	}

	void Scene::quitScene()
	{
		m_QuitEngine();
	}

	void Scene::hideGameWorld()
	{

	}

	void Scene::showGameWorld()
	{

	}

	void Scene::loadGameLevel(const std::string& levelName)
	{
		//load game level json
		auto gameLevelJson = file::loadJson(file::getPath({m_GameSetting->getString("game_level_directory"), levelName}));

		//if game level has script object
		GameLevelScriptObject::Ptr scriptObject = nullptr;
		std::string script_class = gameLevelJson["script_class"].get<std::string>();
		if(script_class != string::StringPool.BLANK)
		{
			boost::dll::fs::path game_library_path(file::removeFileExtension(m_GameSetting->getString("game_library_file")));

			m_CreateGameLevelMap[script_class] = boost::dll::import_alias<CreateCppGameLevelScript>(
													 game_library_path,
													 std::string("create" + script_class),
													 boost::dll::load_mode::append_decorations);

			scriptObject = m_CreateGameLevelMap[script_class](ScriptObject::Context(
														  ));
		}

		//load level resource
		ResourceManager::Ptr resourceManager = std::make_shared<ResourceManager>();
		resourceManager->loadRequired(gameLevelJson["level_resource"]);
		m_GameLevelResourceMap[levelName] = resourceManager;

		//build level object
		GameLevelObject::Ptr levelOject = std::make_shared<GameLevelObject>();
		Parameter levelParameter;
		levelParameter.loadJson(gameLevelJson["level_parameter"]);
		levelOject->init(levelParameter);

		//setup scene builder
		m_SceneBuilder->setResourceManager(m_GameLevelResourceMap[levelName]);
		m_SceneBuilder->setPhysicWorld(levelOject->getPhysicWorld());

		//build world chunk
		for(auto worldChunkJson : gameLevelJson["world_chunk_table"])
		{
			if(worldChunkJson["load_with_level"])
			{
				Object::Ptr chunkOject = std::make_shared<Object>();

				m_SceneBuilder->loadScene(worldChunkJson["world_chunk"]);
				m_SceneBuilder->buildScene(chunkOject);

				levelOject->addChild(chunkOject);
			}
		}

		if(scriptObject)
		{
			//scriptObject->setTarget(levelOject);
			//scriptObject->setEnable(false);

			m_GameWorld->addChild(scriptObject);
		}
		else
		{
			//levelOject->setEnable(false);
			m_GameWorld->addChild(scriptObject);
		}
	}

	void Scene::disableCurrentGameLevel()
	{
		auto& levelTable = *m_GameWorld->getAllChild();

		for(auto level : levelTable)
		{
			//if(level->isEnable())
			{
				//detach the level from the game world
				auto gameLevel = m_GameWorld->removeChild(level);

				//destroy the level
				destroyLevel(gameLevel);

				//break
				break;
			}
		}
	}

	void Scene::destroyLevel(Object::Ptr levelObject)
	{
		//do that in background
	}

	void Scene::enableGameLevel(const std::string& levelName)
	{
		//first disable current level
		disableCurrentGameLevel();

		//proceed to  enable the desired level
		Object::Ptr result = nullptr;
		Object::Ptr gameLevelObject = nullptr;
		Object::Ptr gameLevelScript = nullptr;

		//TODO replace with findGameLevel(levelName)
		for(auto level : *m_GameWorld->getAllChild())
		{
			if(level->getName() == levelName)
			{
				result = level;
				break;
			}
		}

		if (result->getFirstType() == Object::Game_Level_Script_Object)
		{
			gameLevelScript = GameLevelScriptObject::Cast(result);
			//gameLevelObject = gameLevelScript->getTarget();
			//gameLevelScript->setEnable(true);
		}
		else
		{
			gameLevelObject = result;
			//gameLevelObject->setEnable(true);
		}

		//GameLevelObject::Ptr gameLevel = GameLevelObject::Cast(gameLevelObject);

		//m_PhysicWorld = gameLevel->getPhysicWorld();
		m_PhysicWorld->SetContactListener(this);
		m_PhysicWorld->SetDebugDraw(&m_ShapeRenderer);
	}

	void Scene::loadWorldChunk(const std::string& name)
	{

	}

	void Scene::unLoadGameLevel(const std::string& name)
	{

	}

	void Scene::unLoadWorldChunk(const std::string& name)
	{

	}

	void Scene::hideGameScreen(const std::string& name)
	{

	}

	void Scene::showGameScreen(const std::string& name)
	{

	}

	void Scene::enableObjectLayer(const std::string& name)
	{

	}

	void Scene::disableObjectLayer(const std::string& name)
	{

	}

	void Scene::enableGameChunk(const std::string& name)
	{

	}

	void Scene::disableGameChunk(const std::string& name)
	{

	}

	Scene::Context& Scene::getSceneContext()
	{
	  return m_SceneContext;
	}

	Scene::RenderTexturePtr Scene::getRenderTexture()
	{
		return m_SceneContext.renderTexture;
	}

	void Scene::setupLighting()
	{
		//create light manager
		m_LightManager = std::make_shared<ltbl::LightSystem>(false);
		m_LightManager->create({-1000.f, -1000.f, (float)m_SceneContext.renderTexture->getSize().x, (float)m_SceneContext.renderTexture->getSize().y }, m_SceneContext.renderTexture->getSize());

		m_LightManager->setAmbientColor(sf::Color(0, 12, 24));

	}

	void Scene::renderLighting()
	{
		m_LightManager->render(*m_SceneContext.renderTexture);
	}

	ResourceManager::Ptr Scene::getResourceHolder()
	{
		return m_SceneContext.resourceManager;
	}

}
