////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/DevEngine.h>
#include <Nero/utility/Utility.h>
//SFML
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Context.hpp>
//EASY_LOG
#include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    DevEngine::DevEngine(const float& winWidth, const float& winHeight, const sf::String& title):
        Engine(winWidth, winHeight, title)
        //
        ,m_LoadingScreen(nullptr)
        ,m_EngineUI(nullptr)
        ,m_EngineStarted(false)
        //
        ,m_ResourceManager(nullptr)
        ,m_SceneManager(nullptr)
        ,m_SoundManager(nullptr)
        ,m_Camera(nullptr)
        ,m_Grid(nullptr)
        ,m_EngineSetting()
    {
        //Setup the SFML window
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();

        //Build the Loading Screen
        m_LoadingScreen = make_unique<LoadingScreen>();
        m_LoadingScreen->setRenderWindow(&m_Window);
        m_LoadingScreen->init();

        //Load resource in a separated  thread
        m_StartEngineFuture = std::async(std::launch::async, &DevEngine::startEngine, this, std::ref(m_EngineStarted), m_LoadingScreen->getMinTime());
    }

    ////////////////////////////////////////////////////////////
    DevEngine::~DevEngine()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::handleEvent()
    {
        sf::Event event;
        while(m_Window.pollEvent(event))
        {
            //EngineUI
            if(!m_EngineStarted) //Render StartupScreen
            {
                m_LoadingScreen->handleEvent(event);
            }
            else
            {
                //EngineUI
                m_EngineUI->handleEvent(event);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::update(const sf::Time& timeStep)
    {
        if(!m_EngineStarted) //Render StartupScreen
        {
             m_LoadingScreen->update(timeStep);
        }
        else
        {
            //Render EngineUI
            m_EngineUI->update(timeStep);
        }

    }

    ////////////////////////////////////////////////////////////
    void DevEngine::render()
    {
        if(!m_EngineStarted) //Render StartupScreen
        {
            m_Window.clear(m_LoadingScreen->getBackgroundColor());

                m_LoadingScreen->render();

            m_Window.display();
        }
        else
        {
            m_Window.clear(BACKGROUND_CLEAR_COLOR);

                m_EngineUI->render();

            m_Window.display();
        }
    }

    ////////////////////////////////////////////////////////////
    int DevEngine::startEngine(bool& engineStarted, const float minTime)
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

        //load Engine Settings
        nlohmann::json engine_setting = loadConfiguration(ENGINE_CONFIGURATION);
        m_EngineSetting = EngineSetting::fromJson(engine_setting);

        //Build the Engine UI
        m_EngineUI = make_unique<DevEngineUI>(m_Window, m_ResourceManager);
        m_EngineUI->build();

        //Build the Camera
        m_Camera = AdvancedCamera::Ptr(new AdvancedCamera(sf::Vector2f(m_WinWidth*0.15f*3.63f, m_WinHeight*0.75f), m_EngineUI->getRenderCanvas()));

        //Build the Scene Manager
        m_SceneManager = SceneManager::Ptr(new SceneManager(Context(m_EngineUI->getRenderCanvas(), m_EngineUI->getCanvasFrontView(), m_Camera, m_ResourceManager)));

        //Build the Engine Sound Manager
        m_SoundManager = SoundManager::Ptr(new SoundManager(m_ResourceManager->Music, m_ResourceManager->Sound));

        //Build the Grid
        m_Grid = Grid::Ptr(new Grid(sf::Vector2f(m_WinWidth*0.15f*3.63f, m_WinHeight*0.75f)));


        //Let the EngineUI access some Engine resources
        m_EngineUI->setSceneManager(m_SceneManager);
        m_EngineUI->setSoundManager(m_SoundManager);
        m_EngineUI->setEngineSetting(m_EngineSetting);
        m_EngineUI->setCamera(m_Camera);
        m_EngineUI->setGrid(m_Grid);

        //If the startup was too fast, wait a time before exit
        int time = minTime - clock.getElapsedTime().asSeconds();
        std::this_thread::sleep_for(std::chrono::seconds(time < 0 ? 0 : time));

        //Register the  default Scene
        addScene<Scene>(DEFAULT_SCENE);

        //Load and build all Scenes
        m_TaskTable.back()(); //We begin by the default Scene

        for(int i=0; i < m_TaskTable.size()-1; i++)
        {
            m_TaskTable[i]();
        }

        //Select the last active Scene
        m_EngineUI->selectScene(m_EngineSetting.lastScene);

        //First load
        m_EngineUI->updateLog("Nero Game Engine --- Log file " + getdate() + " " + getTime() + "\n");
        m_EngineUI->updateLog("current scene : " + m_EngineSetting.lastScene);

        //exit
        engineStarted = true;
        return 0;
    }
}
