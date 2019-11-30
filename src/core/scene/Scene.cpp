////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#include <Nero/core/scene/Scene.h>

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

	//TODO : remove
	Scene::Context::Context()
	{

	}

	Scene::Scene(const Context& context):
		m_SceneContext(context)
	{

	}

	Scene::~Scene()
    {
		destroy();
    }

	void Scene::destroy()
	{
		//destroy physic world
		m_PhysicWorld = nullptr;
		delete m_PhysicWorld;
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

	}

	void Scene::render()
	{

	}

	void Scene::renderShape()
	{

	}

	void Scene::renderFrontScreen()
	{

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
		  ContactPoint* cp = m_ContactPointTable + m_ContactPointCount;
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

	  Collision collision(contact, oldManifold, nullptr);

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

	  handleCollisionPreSolveContact(collision);
	}


	  void Scene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	  {
		  Collision collision(contact, nullptr, impulse);

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

		  handleCollisionPostSolveContact(collision);
	  }


	  void Scene::BeginContact(b2Contact* contact)
	  {
		  Collision collision(contact, nullptr, nullptr);

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

		  handleCollisionContactBegin(collision);
	  }

	  void Scene::EndContact(b2Contact* contact)
	  {
		  Collision collision(contact, nullptr, nullptr);

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

		  handleCollisionContactEnd(collision);
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

	  }

	  void Scene::hideGameWorld()
	  {

	  }

	  void Scene::showGameWorld()
	  {

	  }

	  void Scene::loadGameLevel(const std::string& name)
	  {

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
}
