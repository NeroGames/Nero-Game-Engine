////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016 - 2019 Sanou A. K. Landry (sk-landry)
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
    DevEngine::DevEngine(const unsigned int& windowWidth, const unsigned int& windowHeight, const std::string& windowTitle):
        Engine(windowWidth, windowHeight, windowTitle)
        ,m_LoadingScreen(nullptr)
        ,m_EngineUI(nullptr)
        ,m_ResourceManager(nullptr)
        ,m_EngineStarted(false)
        ,m_EngineSetting()
    {
        std::cout << "----------------------------------------------------------------------------\n\n";
        nero_log("nero game engine " + ENGINE_VERSION + " " + ENGINE_COPYRIGHT + "\n");
        nero_log("nero development engine starting");

        nero_log("loading engine configuration");
        loadEngineSetting();

        nero_log("setting up sfml render window");
        setupRenderWindow();

        nero_log("building startup screen");
        buildStartupScreen();

        //Load resource in a separated  thread
        nero_log("starting background task");
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
            if(!m_EngineStarted)
            {
                //StartupScreen Event
                m_LoadingScreen->handleEvent(event);
            }
            else
            {
                //EngineUI Event
                m_EngineUI->handleEvent(event);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::update(const sf::Time& timeStep)
    {
        if(!m_EngineStarted)
        {
            //Update StartupScreen
             m_LoadingScreen->update(timeStep);
        }
        else
        {
            //Render EngineUI
            m_EngineUI->update(timeStep);
            m_EngineUI->updateFrameRate(getFrameRate(), getFrameTime());
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::render()
    {
        //Render StartupScreen
        if(!m_EngineStarted)
        {
            m_Window.clear(m_LoadingScreen->getBackgroundColor());

                m_LoadingScreen->render();

            m_Window.display();
        }
        //Render EngineUI
        else
        {
            m_Window.clear(BACKGROUND_CLEAR_COLOR);

                m_EngineUI->render();

            m_Window.display();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::loadEngineSetting()
    {
        if(fileExist(CONFIGURATION_FOLDER + "/" + ENGINE_CONFIGURATION + ".json"))
        {
            m_EngineSetting = EngineSetting::fromJson(loadConfiguration(ENGINE_CONFIGURATION));
        }
        else
        {
            saveFile(CONFIGURATION_FOLDER + "/" + ENGINE_CONFIGURATION + ".json", m_EngineSetting.toJson().dump(3));
        }
    }

    void DevEngine::setupRenderWindow()
    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();
        m_Window.setPosition(sf::Vector2i(m_Window.getPosition().x, 15));

        //setup IMGUI
        ImGui::SFML::Init(m_Window);
    }

    void DevEngine::buildStartupScreen()
    {
        m_LoadingScreen = make_unique<LoadingScreen>();
        m_LoadingScreen->setRenderWindow(&m_Window);
        m_LoadingScreen->init();

    }
    ////////////////////////////////////////////////////////////
    int DevEngine::startEngine(bool& engineStarted, const float minTime)
    {
        //Start a clock
        sf::Clock clock;
        //Activate a context, in case none exist
        sf::Context context;
        //Load all resources
        nero_log("building resource manager");
        m_ResourceManager = ResourceManager::Ptr(new ResourceManager());
        nero_log("loading font resources");
        m_ResourceManager->font.load();
        nero_log("loading sound resources");
        m_ResourceManager->sound.load();
        nero_log("loading music resources");
        m_ResourceManager->music.load();
        nero_log("loading texture resources");
        m_ResourceManager->texture.load();
        nero_log("loading animation resources");
        m_ResourceManager->animation.load();

        //Script and Shader will be part of version 2.0
        //nero_log("Loading Shader Resources");
        //m_ResourceManager->shader.load();
        //nero_log("Loading Script Resources");
        //m_ResourceManager->script.load();

        //Build the Engine UI
        nero_log("building engine-ui");
        m_EngineUI = make_unique<DevEngineUI>(m_Window);
        nero_log("providing engine setting to engine-ui");
        m_EngineUI->setEngineSetting(m_EngineSetting);
        nero_log("providing resource manager to engine-ui");
        m_EngineUI->setResourceManager(m_ResourceManager);
        nero_log("building engine interface");
        m_EngineUI->buildInterface();
        nero_log("building camera");
        m_EngineUI->buildCamera();
        nero_log("building scene manager");
        m_EngineUI->buildSceneManger();

        nero_log("adjusting background task time");
        //If the startup was too fast, wait a time before exit
        int time = minTime - clock.getElapsedTime().asSeconds();
        std::this_thread::sleep_for(std::chrono::seconds(time < 0 ? 0 : time));

        //Register the default Scene
        nero_log("adding the default scene");
        addScene<Scene>(DEFAULT_SCENE);

        nero_log("registering all scenes");
        for(int i=0; i < m_TaskTable.size(); i++)
        {
            m_TaskTable[i]();
        }

        nero_log("setting up the engine-ui");
        m_EngineUI->setup();

        //Select the last active Scene
        nero_log("restoring the last Scene : " + m_EngineSetting.lastScene);
        m_EngineUI->selectScene(m_EngineSetting.lastScene);

        //First log
        nero_log("update engine-ui log");
        m_EngineUI->updateLog("Nero Game Engine --- Log file " + getdate() + " " + getTime() + "\n");

        nero_log("background task completed");
        nero_log("nero development engine started successfully");

        //exit
        engineStarted = true;
        return 0;
    }
}
