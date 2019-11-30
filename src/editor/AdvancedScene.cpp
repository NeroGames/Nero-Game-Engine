#include <Nero/editor/AdvancedScene.h>
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
		m_GameLevelTable.push_back(std::make_shared<GameLevel>(name));
		m_SelectedGameLevel = m_GameLevelTable.back();

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
			m_SelectedGameLevel->chunkTable.push_back(std::make_shared<WorldChunk>(name));
			m_SelectedWorldChunk = m_SelectedGameLevel->chunkTable.back();
			m_SelectedWorldBuilder = m_SelectedWorldChunk->sceneBuilder;
			m_SelectedWorldBuilder->setResourceManager(m_ResourceManager);
			m_SelectedWorldBuilder->setRenderTexture(m_RenderTexture);
			m_SelectedWorldBuilder->setRenderContext(m_RenderContext);
			m_SelectedWorldBuilder->addLayer();
		}
	}

	void AdvancedScene::addGameScreen(const std::string& name)
	{
		m_GameScreenTable.push_back(std::make_shared<GameScreen>(name));
		m_SelectedGameScreen = m_GameScreenTable.back();
		m_SelectedScreenBuilder = m_SelectedGameScreen->sceneBuilder;
		m_SelectedScreenBuilder->setResourceManager(m_ResourceManager);
		m_SelectedWorldBuilder->setRenderTexture(m_RenderTexture);
		m_SelectedWorldBuilder->setRenderContext(m_RenderContext);
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
			m_SelectedWorldBuilder->render();
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
}

