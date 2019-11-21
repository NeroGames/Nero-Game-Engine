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

    void AdvancedScene::handleEvent(const sf::Event& event)
    {

    }

    void AdvancedScene::update(const sf::Time& timeStep)
    {

    }

	sf::RenderTexture& AdvancedScene::render(const RenderContext& renderContext)
    {
		setRenderContext(renderContext);


        //render scene builder
		//m_Camera.setSize(renderContext.canvas_size.x, renderContext.canvas_size.y);
        //m_RenderTexture.setView(m_Camera);

        //render scene
        if(m_Scene)
        {
			m_Scene->renderScene();
			auto& renderTexture = m_Scene->getRenderTexture();
            //return m_Scene->getRenderTexture();

			m_RenderTexture.draw(sf::Sprite(renderTexture.getTexture()));
        }

		//m_LightEngine.render(m_RenderTexture);

        return  m_RenderTexture;
    }

    void AdvancedScene::setScene(Scene::Ptr scene)
    {
        m_Scene = scene;
    }

	void AdvancedScene::setRenderContext(const RenderContext& renderContext)
	{
		m_RenderContext = renderContext;
	}

	void AdvancedScene::handleSceneBuilderEvent(const sf::Event& event)
	{

	}

	void AdvancedScene::handleScreenBuilderEvent(const sf::Event& event)
	{

	}

	void AdvancedScene::handleSceneEvent(const sf::Event& event)
	{

	}

	void AdvancedScene::updateSceneBuilder(const sf::Time& timeStep)
	{

	}

	void AdvancedScene::updateScreenBuilder(const sf::Time& timeStep)
	{

	}

	void AdvancedScene::updateScene(const sf::Time& timeStep)
	{

	}

	void AdvancedScene::renderSceneBuilder(sf::RenderTexture& texture)
	{
		sf::CircleShape circle;

		circle.setRadius(50.f);
		circle.setOrigin(50.f, 50.f);

		sf::RectangleShape rectangle(sf::Vector2f(200.f, 50.f));

		rectangle.setPosition(100.f, 0.f);
		rectangle.setFillColor(sf::Color::Blue);

		texture.draw(circle);
		texture.draw(rectangle);
	}

	void AdvancedScene::renderScreenBuilder(sf::RenderTexture& texture)
	{

	}

	void AdvancedScene::renderScene(sf::RenderTexture& texture)
	{

	}

}

