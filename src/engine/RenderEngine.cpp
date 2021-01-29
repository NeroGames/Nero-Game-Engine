////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/RenderEngine.h>
#include <Nero/utility/Utility.h>Copyright (c) 2016-2021 Sanou A. K. Landry
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
        ,m_RestartScene(false)
    {
        //Setup the SFML window
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();
        //Build the Loading Screen
        m_StartupScreen->setRenderWindow(&m_Window);
        m_StartupScreen->init();
        m_Task = [](){};
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
    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates(); //if not, SFGUI will have some undetermined behaviour

        m_Task = [](){};

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

                if(m_RestartScene)
                {
                    m_Task();
                    m_RestartScene = false;
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
            m_Scene->m_FrameRate = getFrameRate();
            m_Scene->m_FrameTime = getFrameTime();
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

                m_RenderCanvas->Clear(m_Scene->m_CanvasColor);

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
        m_ResourceManager->font.load();
        m_ResourceManager->sound.load();
        m_ResourceManager->music.load();
        m_ResourceManager->shader.load();
        m_ResourceManager->script.load();
        m_ResourceManager->texture.load();
        m_ResourceManager->animation.load();

        // Create our SFML canvas window
        m_RenderCanvas = sfg::Canvas::Create();
        m_RenderCanvas->SetRequisition(sf::Vector2f(getWindowWidth(), getWindowHeight()));
        m_Desktop.Add(m_RenderCanvas);

        m_FrontView = m_RenderCanvas->GetView();
        m_FrontView.setCenter(getWindowWidth()/2.f, getWindowHeight()/2.f);
        m_FrontView.setSize(sf::Vector2f(getWindowWidth(), getWindowHeight()));

        m_Camera = Camera::Ptr(new Camera(sf::Vector2f(getWindowWidth(), getWindowHeight())));

        //If the startup was too fast, wait a time before exit
        int time = minTime - clock.getElapsedTime().asSeconds();
        std::this_thread::sleep_for(std::chrono::seconds(time < 0 ? 0 : time));

        //Call set scene task
        m_Task();

        //Register the  default Scene if none exist
        if(!m_Scene)
        {
            setScene<Scene>(DEFAULT_SCENE);
            m_Task();
        }

        m_Camera->setPosition(m_Scene->m_CameraSetting.defaultPosition);
        m_Camera->setRotation(m_Scene->m_CameraSetting.defaultRotation);
        m_Camera->setZoom(m_Scene->m_CameraSetting.defaultZoom);

        //exit
        engineStarted = true;
        return 0;
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::resetScene()
    {
       m_RestartScene = true;
    }
}
////////////////////////////////////////////////////////////
