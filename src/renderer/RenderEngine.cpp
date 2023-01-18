////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/renderer/RenderEngine.h>
#include <Nero/renderer/NoGameFound.h>
// Boost
#include <boost/dll.hpp>
/////////////////////////////////////////////////////////////

namespace nero
{
    RenderEngine::RenderEngine()
        : CoreEngine(true)
        , m_GameScene(nullptr)
        , m_StartupScreen(nullptr)
        , m_EngineStarted(false)
        , m_GameSetting(nullptr)
        , m_GameWorldResourceManager(nullptr)
        , m_GameScreenResourceManager(nullptr)
        , m_StartupScreenResourceManager(nullptr)
    {
        bool everythingGood = checkDirectory();

        if(everythingGood)
        {
            loadGame();
        }
        else
        {
            noGameFound();
        }
    }

    RenderEngine::~RenderEngine()
    {
    }

    bool RenderEngine::checkDirectory()
    {
        return false;
    }

    void RenderEngine::noGameFound()
    {
        // create window
        m_RenderWindow.create(sf::VideoMode(
                                  EngineConstant.ENGINE_WINDOW_WIDTH,
                                  EngineConstant.ENGINE_WINDOW_HEIGHT),
                              EngineConstant.NO_GAME_FOUND,
                              sf::Style::Close);

        // build NoGameScene
        m_GameScene     = NoGameFound::Ptr(new NoGameFound(Scene::Context(
            EngineConstant.NO_GAME_FOUND,
            m_RenderTexture,
            m_GameWorldResourceManager,
            m_Camera,
            m_GameSetting,
            Scene::EngineType::RENDER_ENGINE,
            Scene::PlatformType::WINDOWS)));

        // set quit engine callback
        // m_GameScene->m_QuitEngine = [this](){ m_RenderWindow.close();};

        // skip startup screen display
        m_EngineStarted = true;
    }

    void RenderEngine::loadGame()
    {
        // load game setting
        m_GameSetting->loadSetting(file::getPath({"Game", "Setting", "game_setting"}));

        // create Render_Window
        createRenderWindow();

        // load statup screen
        if(m_GameSetting->getBool("enable_startup_screen"))
        {
            loadStartupScreen();

            startEngineInBackground();
        }
        else
        {
            startEngine(m_EngineStarted, EngineConstant.NUMBER_ZERO);
        }

        // start bacground initialization
    }

    void RenderEngine::createRenderWindow()
    {
        m_RenderWindow.create(sf::VideoMode(
                                  m_GameSetting->getUInt("window_width"),
                                  m_GameSetting->getUInt("window_height")),
                              m_GameSetting->getString("game_name"),
                              getWindowStyle(m_GameSetting->getString("window_style")));
    }

    void RenderEngine::startEngineInBackground()
    {
        m_StartEngineFuture = std::async(std::launch::async, &RenderEngine::startEngine, this, std::ref(m_EngineStarted), m_StartupScreen->getDuration());
    }

    void RenderEngine::loadStartupScreen()
    {
        boost::dll::fs::path game_library_path(file::removeFileExtension(m_GameSetting->getString("game_library_file")));

        m_CreateCppStartupScreen = boost::dll::import_alias<CreateCppStartupScreen>(
            game_library_path,
            EngineConstant.DLL_CREATE_STARTUP_SCREEN,
            boost::dll::load_mode::append_decorations);

        if(!m_CreateCppStartupScreen.empty())
        {
            // create startup screen object
            m_StartupScreen = m_CreateCppStartupScreen();
            // provide render window
            m_StartupScreen->setRenderWindow(&m_RenderWindow);
            // load resource
            m_StartupScreenResourceManager->loadDirectory(m_GameSetting->getString("startup_screen_resource_directory"));
            // provide resource
            m_StartupScreen->setResourceManager(m_StartupScreenResourceManager);
        }
        else
        {
            // TODO
            // handle error
        }
    }

    int RenderEngine::startEngine(bool& engineStarted, const unsigned int duration)
    {
        // load scene class
        boost::dll::fs::path game_library_path(file::removeFileExtension(m_GameSetting->getString("game_library_file")));

        m_CreateCppScene = boost::dll::import_alias<CreateCppScene>(
            game_library_path,
            EngineConstant.DLL_CREATE_SCENE,
            boost::dll::load_mode::append_decorations);
        // create scene object
        if(!m_CreateCppScene.empty())
        {
            m_GameScene = m_CreateCppScene(Scene::Context(
                m_GameSetting->getString("game_name"),
                m_RenderTexture,
                m_GameWorldResourceManager,
                m_Camera,
                m_GameSetting,
                Scene::EngineType::RENDER_ENGINE,
                Scene::PlatformType::WINDOWS));
        }

        // load all game screen script class
        for(const std::string& script_class : m_GameSetting->getStringTable("game_screen_script_table"))
        {
            /*m_GameScene->m_CreateGameScreenMap[script_class] = boost::dll::import_alias<CreateCppScene>(
                                                                                             game_library_path,
                                                                                             std::string("create" + script_class),
                                                                                             boost::dll::load_mode::append_decorations);*/
        }

        // load all game object simple script class
        for(const std::string& screen_class : m_GameSetting->getStringTable("game_object_simple_script_table"))
        {
            /*m_CreateSimpleScriptMap[script_class] = boost::dll::import_alias<CreateCppScene>(
                                                                                             game_library_path,
                                                                                             std::string("create" + script_class),
                                                                                             boost::dll::load_mode::append_decorations);*/
        }

        // load all game object physic script class
        for(const std::string& screen_class : m_GameSetting->getStringTable("game_object_physic_script_table"))
        {
            /*m_CreatePhysicScriptMap[script_class] = boost::dll::import_alias<CreateCppScene>(
                                                                                             game_library_path,
                                                                                             std::string("create" + script_class),
                                                                                             boost::dll::load_mode::append_decorations);*/
        }

        //
        // m_GameScene->m_SceneBuilder = std::make_shared<SceneBuilder>();

        // provide callback
        // m_GameScene->m_QuitEngine = [this](){m_RenderWindow.close();};

        // initialize game scene
        // m_GameScene->init();

        return EngineConstant.NUMBER_ZERO;
    }

    void RenderEngine::handleEvent()
    {
        sf::Event event;
        while(m_RenderWindow.pollEvent(event))
        {
            if(m_StartupScreen && !m_EngineStarted)
            {
                m_GameScene->handleEvent(event);
            }
            else
            {
                m_GameScene->handleEvent(event);
            }
        }
    }

    void RenderEngine::update(const sf::Time& timeStep)
    {
        if(m_StartupScreen && !m_EngineStarted)
        {
            m_StartupScreen->update(timeStep);
        }
        else
        {
            m_GameScene->update(timeStep);
        }
    }

    void RenderEngine::render()
    {
        if(m_StartupScreen && !m_EngineStarted)
        {
            m_StartupScreen->render();
        }
        else
        {
            m_GameScene->render();
        }
    }

    sf::Uint32 RenderEngine::getWindowStyle(const std::string& style)
    {
        if(style == "window_style_default")
        {
            return sf::Style::Default;
        }
        else if(style == "window_style_fullscreen")
        {
            return sf::Style::Fullscreen;
        }
        else if(style == "window_style_none")
        {
            return sf::Style::None;
        }
        else if(style == "window_style_titlebar")
        {
            return sf::Style::Titlebar;
        }
        else if(style == "window_style_resize")
        {
            return sf::Style::Resize;
        }
        else if(style == "window_style_close")
        {
            return sf::Style::Close;
        }
        else
        {
            if(style.find("window_style_titlebar") != std::string::npos &&
               style.find("window_style_resize") != std::string::npos)
            {
                return sf::Style::Titlebar | sf::Style::Resize;
            }

            if(style.find("window_style_titlebar") != std::string::npos &&
               style.find("window_style_close") != std::string::npos)
            {
                return sf::Style::Titlebar | sf::Style::Close;
            }

            if(style.find("window_style_resize") != std::string::npos &&
               style.find("window_style_close") != std::string::npos)
            {
                return sf::Style::Resize | sf::Style::Close;
            }
        }

        return sf::Style::Close;
    }

} // namespace nero
