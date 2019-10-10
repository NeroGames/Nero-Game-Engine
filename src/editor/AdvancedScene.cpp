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

    AdvancedScene::AdvancedScene()
    {

    }

    void AdvancedScene::handleEvent(const sf::Event& event)
    {

    }

    void AdvancedScene::update(const sf::Time& timeStep)
    {

    }

    sf::RenderTexture& AdvancedScene::render(const RenderContext& renderContext)
    {
        m_RenderContext = renderContext;

        if(m_RenderContext.canvas_size.x < 50.f ||  m_RenderContext.canvas_size.y < 50.f)
        {
            m_RenderContext.canvas_size.x = 50.f;
            m_RenderContext.canvas_size.y = 50.f;
        }

        //render scene builder
        m_RenderTexture.create(m_RenderContext.canvas_size.x, m_RenderContext.canvas_size.y);
        //m_Camera.setSize(renderContext.canvas_size.x, renderContext.canvas_size.y);
        //m_RenderTexture.setView(m_Camera);
        m_RenderTexture.clear(sf::Color::Black);

        //render scene
        if(m_Scene)
        {
            m_Scene->renderScene();
            auto& renderTexture = m_Scene->getRenderTexture();
            //return m_Scene->getRenderTexture();

            m_RenderTexture.draw(sf::Sprite(renderTexture.getTexture()));
        }


        return  m_RenderTexture;

    }

    void AdvancedScene::setScene(Scene::Ptr scene)
    {
        m_Scene = scene;
    }

}

