////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/SceneRenderer.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    SceneRenderer::SceneRenderer(const float& windowWidth, const float& windowHeight, const sf::String& title):
         Engine(windowWidth, windowHeight, title)
        ,m_Camera(new Camera(sf::Vector2f(windowWidth, windowHeight)))
        ,m_ShapeRenderer(&m_Window)
        ,m_RestartScene(false)
    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();

        m_FrontView = m_Window.getView();
        m_FrontView.setCenter(windowWidth/2.f, windowHeight/2.f);
        m_FrontView.setSize(sf::Vector2f(windowWidth, windowHeight));
        m_Window.setPosition(sf::Vector2i(m_Window.getPosition().x, 15));
    }

    ////////////////////////////////////////////////////////////
    SceneRenderer::~SceneRenderer()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::handleEvent()
    {
        sf::Event event;

        while (m_Window.pollEvent(event))
        {
            m_Camera->handleEvent(event);
            m_Scene->handleEvent(event);

            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    m_Scene->getSoundManager()->stopMusic();
                    m_QuitFn();
                }break;
            }
        }

        if(m_RestartScene)
        {
            m_RestartScene = false;
            m_RestartSceneFn();
        }
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::update(const sf::Time& timeStep)
    {
        m_Camera->update(timeStep);
        m_Window.setView(m_Camera->getView());

        m_Scene->update(timeStep);
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::render()
    {
        m_Window.clear(m_Scene->m_CanvasColor);

            renderScene();

            m_Window.setView(m_FrontView);
                renderSceneFront();
            m_Window.setView(m_Camera->getView());

        m_Window.display();
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::setScene(Scene::Ptr scene)
    {
        //scene
        m_Scene = scene;
        //shape_renderer
        m_Scene->m_ShapeRenderer = m_ShapeRenderer;
        m_Scene->m_PhysicWorld->SetDebugDraw(&m_Scene->m_ShapeRenderer);
        //camera
        m_Camera->setPosition(m_Scene->m_CameraSetting.position);
        m_Camera->setRotation(m_Scene->m_CameraSetting.rotation);
        m_Camera->setZoom(m_Scene->m_CameraSetting.zoom);
        m_Scene->m_Context.camera = m_Camera;
        //m_Scene->m_Context.frontView = m_FrontView;
        m_Scene->m_Context.renderEngine = true;
        //call_back
        m_Scene->m_QuitEngine = [this]()
        {
            m_Scene->getSoundManager()->stopMusic();
            m_QuitFn();
        };

        m_Scene->m_ResetScene = [this]()
        {
            m_RestartScene = true;
        };
        //initialization
        m_Scene->m_ObjectManager->setPhysicWorld(m_Scene->m_PhysicWorld);
        m_Scene->checkSceneObject();
        m_Scene->init();
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::renderScene()
    {
        if(!m_Scene->m_HideWorld)
        {
            auto childTable = m_Scene->m_World->getAllChild();

            for(auto it = childTable->begin(); it != childTable->end(); it++)
            {
                LayerObject::Ptr layer_object = LayerObject::Cast(*it);

                if(layer_object->isVisible())
                    m_Window.draw(*layer_object);
            }

            m_Scene->m_PhysicWorld->DrawDebugData();

        }
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::renderSceneFront()
    {
        if(m_Scene->isRenderEngine())
        {
            for(auto screen : m_Scene->m_ScreenStack)
            {
                if(!screen->hide)
                {
                    auto childTable1 = screen->screen->getAllChild();
                    auto childTable2 = screen->screenUI->getAllChild();

                    for(auto it = childTable1->begin(); it != childTable1->end(); it++)
                    {
                        m_Window.draw(**it);
                    }

                    for(auto it = childTable2->begin(); it != childTable2->end(); it++)
                    {
                        m_Window.draw(**it);
                    }
                }
            }
        }

        if(!m_Scene->m_HideWorld)
        {
            m_Window.draw(m_Scene->m_Text);
        }
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::setRestartScene(std::function<void()> fn)
    {
        m_RestartSceneFn = fn;
    }

    ////////////////////////////////////////////////////////////
    void SceneRenderer::setQuit(std::function<void()> fn)
    {
        m_QuitFn = fn;
    }
}
