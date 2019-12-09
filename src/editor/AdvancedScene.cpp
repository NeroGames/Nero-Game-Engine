#include <Nero/editor/AdvancedScene.h>
#include <Nero/core/cpp/utility/MathUtil.h>
#include <SFML/Graphics/Sprite.hpp>

namespace nero
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
           //scene->jointDestroyed(joint);
       }
   }

	AdvancedScene::AdvancedScene():
		m_LightEngine(true)
	   ,m_RenderTexture(nullptr)
	   ,m_GameScreenCount(0)
	   ,m_GameLevelCount(0)
    {
		// Create the LightSystem
		m_LightEngine.create({ -1000.f, -1000.f, 2000.f, 2000.f }, sf::Vector2u(800.f, 800.f));

		// Load light texture
		/*sf::Texture pointLightTexture;
		pointLightTexture.loadFromFile("pointLightTexture.png");
		pointLightTexture.setSmooth(true);
		sf::Texture spookyLightTexture;
		spookyLightTexture.loadFromFile("spookyLightTexture.png");
		spookyLightTexture.setSmooth(true);
		sf::Texture backgroundTexture;
		backgroundTexture.loadFromFile("background.png");
		sf::Texture backgroundTextureNormals;
		backgroundTextureNormals.loadFromFile("background_NORMALS.png");
		sf::Texture headTexture;
		headTexture.loadFromFile("head.png");
		sf::Texture headTextureNormals;
		headTextureNormals.loadFromFile("head_NORMALS.png");

		// Add a sun light
		ltbl::LightDirectionEmission* sun = m_LightEngine.createLightDirectionEmission();
		sun->setColor(sf::Color(255, 255, 255, 50));

		// Add a light point
		ltbl::LightPointEmission* mlight = m_LightEngine.createLightPointEmission();
		mlight->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
		mlight->setTexture(pointLightTexture);
		mlight->setScale(3.f, 3.f);
		mlight->setColor(sf::Color::White);

		// Create a shape
		std::vector<sf::RectangleShape> shapes;
		sf::RectangleShape blocker;
		blocker.setSize({ 200.f, 50.f });
		blocker.setPosition({ 500.f, 300.f });
		blocker.setFillColor(sf::Color::Red);
		shapes.push_back(blocker);

		// Create a light shape with the same shape
		m_LightEngine.createLightShape(blocker);

		ltbl::Sprite background;
		background.setTexture(backgroundTexture);
		background.setNormalsTexture(backgroundTextureNormals);
		m_LightEngine.addSprite(background);

		ltbl::Sprite head;
		head.setTexture(headTexture);
		head.setNormalsTexture(headTextureNormals);
		head.setPosition(300.f, 200.f);
		m_LightEngine.addSprite(head);*/


    }

	void AdvancedScene::addGameLevel(const std::string& name)
	{
		m_GameLevelCount++;

		std::string levelName = name;
		if(levelName == StringPool.BLANK)
		{
			levelName = "game level " + toString(m_GameLevelCount);
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
				chunkName = "world chunk " + toString(m_SelectedGameLevel->chunkCount);
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
		}
	}

	void AdvancedScene::addGameScreen(const std::string& name)
	{
		m_GameScreenCount++;

		std::string screenName = name;
		if(screenName == StringPool.BLANK)
		{
			screenName = "game level " + toString(m_GameScreenCount);
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
		}

	}

	void AdvancedScene::setScene(Scene::Ptr scene)
	{
		m_Scene = scene;
    }

	void AdvancedScene::setRenderContext(const RenderContextPtr& renderContext)
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

	AdvancedScene::GameLevelPtr AdvancedScene::getSelectedGameLevel()
	{
		return m_SelectedGameLevel;
	}

	void AdvancedScene::setSetting(const Setting::Ptr& setting)
	{
		m_EngineSetting = setting;
	}

	AdvancedScene::WorldChunkPtr AdvancedScene::getSelectedWorldChunk()
	{
		return m_SelectedWorldChunk;
	}

	void AdvancedScene::setSelectedWorldChunk(AdvancedScene::WorldChunkPtr worldChunk)
	{
		m_SelectedWorldChunk	= worldChunk;
		m_SelectedWorldBuilder	= worldChunk->sceneBuilder;
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
			//destroy current scene
		}

		//creatte a new Scene
		//m_Scene = m_creat

		//advance scene things
		m_Scene->m_PhysicWorld->SetDestructionListener(&m_DestructionListener);
		b2BodyDef bodyDef;
		m_GroundBody = m_Scene->m_PhysicWorld->CreateBody(&bodyDef);

		//load the current level
		m_Scene->loadGameLevel(m_SelectedGameLevel->name);

	}

	void AdvancedScene::shiftMouseDown(const b2Vec2& p)
	{
		m_MouseWorld = p;

		if (m_MouseJoint != NULL)
		{
			return;
		}

		spawnBomb(p);
	}


	void AdvancedScene::mouseDown(const b2Vec2& p)
	{
		m_MouseWorld = p;

		if (m_MouseJoint != NULL)
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
			m_MouseJoint = NULL;
		}

		if (m_BombSpawning)
		{
			completeBombSpawn(p);
		}
	}

	void AdvancedScene::mouseMove(const b2Vec2& p)
	{
		m_MouseWorld = p;

		if (m_MouseJoint)
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
		b2Vec2 p(randomFloat(-15.0f, 15.0f), -30.0f);
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
		if (m_Bomb)
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
}

