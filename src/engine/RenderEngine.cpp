////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/RenderEngine.h>
#include <Nero/utility/Utility.h>
//SFML
#include <SFML/Window/Context.hpp>
//SFGUI
#include <SFGUI/Window.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    ////////////////////////////////////////////////////////////
    RenderEngine::RenderEngine(StartupScreen::Ptr startupScreen, const float& windowWidth, const float& windowHeight, const sf::String& title):
        Engine(windowWidth, windowHeight, title)
        ,m_StartupScreen(startupScreen)
        ,m_Sfgui()
        ,m_EngineStarted(false)
        ,m_Camera(nullptr)
        ,m_ResourceManager(nullptr)
        ,m_SceneBuilder(nullptr)
    {
         //Setup the SFML window
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();

        //Build the Loading Screen
        m_StartupScreen->setRenderWindow(&m_Window);
        m_StartupScreen->init();

        //Load resource in a separated  thread
        m_StartEngineFuture = std::async(std::launch::async, &RenderEngine::startEngine, this, std::ref(m_EngineStarted), m_StartupScreen->getMinTime());
    }

    ////////////////////////////////////////////////////////////
    RenderEngine::RenderEngine(const float& windowWidth, const float& windowHeight, const sf::String& title):
        Engine(windowWidth, windowHeight, title)
        ,m_StartupScreen(nullptr)
        ,m_Sfgui()
        ,m_EngineStarted(true)
        ,m_Camera(nullptr)
        ,m_ResourceManager(nullptr)
        ,m_SceneBuilder(nullptr)
    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates(); //if not, SFGUI will have some undetermined behaviour


        startEngine(m_EngineStarted, 0.f);
    }

    ////////////////////////////////////////////////////////////
    RenderEngine::~RenderEngine()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::handleEvent()
    {
        sf::Event event;

        while (m_Window.pollEvent(event))
        {
            if(!m_EngineStarted && m_StartupScreen) //Render StartupScreen
            {
                m_StartupScreen->handleEvent(event);
            }
            else
            {
                m_Desktop.HandleEvent(event);
                m_Camera->handleEvent(event);
                m_Scene->handleEvent(event);

                switch(event.type)
                {
                    case sf::Event::Closed:
                        m_Window.close();
                        break;
                    //Keyboard events
                    case sf::Event::KeyPressed:
                        handleKeyboardInput(event.key.code, true);
                        break;
                    case sf::Event::KeyReleased:
                        handleKeyboardInput(event.key.code, false);
                        break;
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::update(const sf::Time& timeStep)
    {
        if(!m_EngineStarted && m_StartupScreen) //Render StartupScreen
        {
             m_StartupScreen->update(timeStep);
        }
        else
        {
            m_Desktop.Update(timeStep.asSeconds());

            m_Camera->update(timeStep);
            m_RenderCanvas->SetView(m_Camera->getView());

            m_Scene->update(timeStep);
        }
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::render()
    {
        if(!m_EngineStarted && m_StartupScreen) //Render StartupScreen
        {
            m_Window.clear(m_StartupScreen->getBackgroundColor());

                m_StartupScreen->render();

            m_Window.display();
        }
        else
        {
            m_Window.clear();

                m_RenderCanvas->Clear(m_Scene->m_SceneSetting.canvasColor);

                    m_Scene->render();
                    m_Scene->renderShape();

                    m_RenderCanvas->SetView(m_FrontView);
                        m_Scene->renderFrontScreen();
                    m_RenderCanvas->SetView(m_Camera->getView());

                    m_RenderCanvas->Display();

                m_Sfgui.Display(m_Window);

            m_Window.display();
        }
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //Handle only key pressed inputs
        if(isPressed)
        {
            // R to reset the scene
            if(key == sf::Keyboard::R && CTRL())
            {
                m_Camera->reinitialize();
                m_Scene = m_CreateScene();
                m_SceneBuilder->loadScene(m_SceneJson);
                m_SceneBuilder->setPhysicWorld(m_Scene->m_PhysicWorld);
                m_SceneBuilder->buildScene(m_Scene->m_RootObject);
                m_Scene->init();
            }

            // P to pause the scene
            else if(key == sf::Keyboard::P && CTRL())
                m_Scene->m_SceneSetting.pause = !m_Scene->m_SceneSetting.pause;
        }
    }

    ////////////////////////////////////////////////////////////
    int RenderEngine::startEngine(bool& engineStarted, const float minTime)
    {
        //Start a clock
        sf::Clock clock;

        //Activate a context, in case none exist
        sf::Context context;

        //Load all resources
        m_ResourceManager = ResourceManager::Ptr(new ResourceManager());
        m_ResourceManager->Font.load();
        m_ResourceManager->Sound.load();
        m_ResourceManager->Music.load();
        m_ResourceManager->Shader.load();
        m_ResourceManager->Script.load();
        m_ResourceManager->Texture.load();
        m_ResourceManager->Animation.load();

        // Create our SFML canvas window
        m_RenderCanvas = sfg::Canvas::Create();
        m_RenderCanvas->SetRequisition(sf::Vector2f(m_WinWidth, m_WinHeight));
        m_Desktop.Add(m_RenderCanvas);

        m_FrontView = m_RenderCanvas->GetView();
        m_FrontView.setCenter(m_WinWidth/2.f, m_WinHeight/2.f);
        m_FrontView.setSize(sf::Vector2f(m_WinWidth, m_WinHeight-22.f));


        m_Camera = Camera::Ptr(new Camera(sf::Vector2f(m_WinWidth, m_WinHeight)));
        m_SceneBuilder = SceneBuilder::Ptr(new SceneBuilder(m_RenderCanvas, m_ResourceManager));

        //If the startup was too fast, wait a time before exit
        int time = minTime - clock.getElapsedTime().asSeconds();
        std::this_thread::sleep_for(std::chrono::seconds(time < 0 ? 0 : time));

        //Register the  default Scene
        setScene<Scene>(DEFAULT_SCENE);

        //exit
        engineStarted = true;
        return 0;
    }
}
