////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/editor/GameEditor.h>
#include <Nero/editor/EditorSetting.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/editor/EditorUI.h>
#include <Nero/core/cpp/utility/File.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    GameEditor::GameEditor()
        : CoreEngine(false)
        , m_EditorStarted(false)
    {
        // Initialize logging
        initLogging();

        // Load window and resource settings
        initSetting();

        // Create the rendering window
        createRenderWindow();

        // Create the loading screen
        createLoadingScreen();

        // Start a new thread and initialize the editor in background
        backgroundStartup();
    }

    GameEditor::~GameEditor()
    {
        destroy();
    }

    void GameEditor::destroy()
    {
        m_EditorTextureHolder->clear();
        m_EditorFontHolder->clear();
        m_EditorSoundHolder->clear();

        m_EditorStarted       = false;
        m_LoadingScreen       = nullptr;
        m_EditorTextureHolder = nullptr;
        m_EditorFontHolder    = nullptr;
        m_EditorSoundHolder   = nullptr;
        m_EditorSetting       = nullptr;
        m_EditorCamera        = nullptr;
        m_EditorUI            = nullptr;

        ImGui::SFML::Shutdown();
    }

    void GameEditor::handleEvent()
    {
        sf::Event event;
        while(m_RenderWindow.pollEvent(event))
        {
            if(!m_EditorStarted)
            {
                // loading screen event
                m_LoadingScreen->handleEvent(event);
            }
            else
            {
                // editor interface event
                m_EditorUI->handleEvent(event);
            }
        }
    }

    void GameEditor::update(const sf::Time& timeStep)
    {
        if(!m_EditorStarted)
        {
            // update loading screen
            m_LoadingScreen->update(timeStep);
        }
        else
        {
            // update interface
            m_EditorUI->update(timeStep);
            m_EditorUI->updateFrameRate(getFrameRate(), getFrameTime());
        }
    }

    void GameEditor::render()
    {
        if(!m_EditorStarted)
        {
            // render loading screen
            m_RenderWindow.clear(m_LoadingScreen->getCanvasColor());
            m_LoadingScreen->render();
            m_RenderWindow.display();
        }
        else
        {
            // render interface
            m_RenderWindow.clear(EngineConstant::COLOR_CANVAS);
            m_EditorUI->render();
            m_RenderWindow.display();
        }
    }

    void GameEditor::initLogging()
    {
        // Initialize the engine logger
        // TODO load and use logger settings
        logging::Logger::init();

        nero_log(EngineConstant::ENGINE_NAME + StringPool.SPACE + EngineConstant::ENGINE_VERSION);
        nero_log(EngineConstant::ENGINE_COPYRIGHT) nero_log(EngineConstant::LEARN_MORE_MESSAGE);
        nero_log("---") nero_log("Nero Game Editor starting");
    }

    void GameEditor::initSetting()
    {
        nero_log("Loading startup settings") m_EditorSetting = std::make_shared<Setting>();

        nero_log("-> Loading render window settings");

        std::string windowSetting = file::getPath({"setting", "window"}, StringPool.EXT_JSON);
        if(!file::fileExist(windowSetting))
        {
            file::saveFile(windowSetting, EditorSetting.windowSetting.dump(3));
        }
        m_EditorSetting->loadSetting(windowSetting, true);

        nero_log("-> Loading resource settings");

        std::string resourceSetting = file::getPath({"setting", "resource"}, StringPool.EXT_JSON);
        if(!file::fileExist(resourceSetting))
        {
            file::saveFile(resourceSetting, EditorSetting.resourceSetting.dump(3));
        }
        m_EditorSetting->loadSetting(resourceSetting, true);

        nero_log("-> Load startup setting completed");
    }

    void GameEditor::createRenderWindow()
    {
        nero_log("Creating render window");

        nero_log("-> Setting render window properties");

        const auto windowSetting = m_EditorSetting->getSetting("window");
        m_RenderWindow.create(
            sf::VideoMode(windowSetting.getUInt("width"), windowSetting.getUInt("height")),
            EngineConstant::ENGINE_WINDOW_TITLE,
            sf::Style::Default);
        m_RenderWindow.setPosition(
            sf::Vector2i(windowSetting.getInt("position_x"), windowSetting.getInt("position_y")));
        m_RenderWindow.setVerticalSyncEnabled(true);
        m_RenderWindow.resetGLStates();

        nero_log("-> Setting render window icon");
        CoreEngine::setWindowIcon(windowSetting.getString("icon"));

        nero_log("-> Initializing Imgui");
        ImGui::SFML::Init(m_RenderWindow, false);

        nero_log("-> Render window creation completed");
    }

    void GameEditor::createLoadingScreen()
    {
        nero_log("Creating loading screen");

        nero_log("-> Loading startup resources");
        ResourceManager::Ptr resourceManager =
            std::make_shared<ResourceManager>(m_EditorSetting->getSetting("resource"));
        resourceManager->loadDirectory("resource/startup");

        nero_log("-> Setting up loading screen");
        m_LoadingScreen = std::make_unique<LoadingScreen>();
        m_LoadingScreen->setRenderWindow(&m_RenderWindow);
        m_LoadingScreen->setResourceManager(resourceManager);

        nero_log("-> Initializing loading screen");
        m_LoadingScreen->init();

        nero_log("-> Loading screen creation completed");
    }

    void GameEditor::backgroundStartup()
    {
        nero_log("Starting background operations");
        m_StartupFuture = std::async(std::launch::async,
                                     &GameEditor::startEngine,
                                     this,
                                     std::ref(m_EditorStarted),
                                     m_LoadingScreen->getDuration());
    }

    int GameEditor::startEngine(bool& editorStarted, const int duration)
    {
        nero_log("Running background operations");

        nero_log("-> Starting a clock");
        sf::Clock clock;

        // Load setting
        loadSetting();

        // Check everionment variable
        checkEnvironmentVariable();

        // Load editor resource
        loadEditorResource();

        // Create camera
        createCamera();

        // Create editor interface
        createEditorInterface();

        // Open last project
        openLastProject();

        // Wait if necessary
        int waitTime =
            static_cast<int>(static_cast<float>(duration) - clock.getElapsedTime().asSeconds());
        std::this_thread::sleep_for(std::chrono::seconds(waitTime < 0 ? 0 : waitTime));

        // Commit
        editorStarted = true;

        nero_log("-> Background operations completed");
        nero_log("---");

        return 0;
    }

    void GameEditor::buildDirectory()
    {
        nero_log("-> Building editor directory");

        file::createDirectory(file::getPath({"setting"}));
        file::createDirectory(file::getPath({"logging"}));
        file::createDirectory(file::getPath({"plugin"}));
        // Template
        file::createDirectory(file::getPath({"template"}));
        file::createDirectory(file::getPath({"template", "cpp-project"}));
        file::createDirectory(file::getPath({"template", "lua-project"}));
        file::createDirectory(file::getPath({"template", "cpp-lua-project"}));
        // Resource
        file::createDirectory(file::getPath({"resource"}));
        // Resource/Editor
        file::createDirectory(file::getPath({"resource", "editor"}));
        // Resource/Startup
        file::createDirectory(file::getPath({"resource", "startup"}));
        file::createDirectory(file::getPath({"resource", "startup", "texture"}));
        file::createDirectory(file::getPath({"resource", "startup", "animation"}));
        file::createDirectory(file::getPath({"resource", "startup", "sound"}));
        file::createDirectory(file::getPath({"resource", "startup", "music"}));
        file::createDirectory(file::getPath({"resource", "startup", "font"}));
        file::createDirectory(file::getPath({"resource", "startup", "language"}));
        // Resource/Starterpack
        file::createDirectory(file::getPath({"resource", "starterpack"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "font"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "texture"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "animation"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "lightmap"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "sound"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "music"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "language"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "shader"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "luascript"}));
        file::createDirectory(file::getPath({"resource", "starterpack", "cppscript"}));
    }

    void GameEditor::loadSetting()
    {
        nero_log("-> Loading editor settings");

        nero_log("--> Loading recent project settings");
        checkRecentProject();
        m_EditorSetting->loadSetting(file::getPath({"setting", "recent_project"}));

        nero_log("--> Loading workspace settings");
        checkWorkspace();
        m_EditorSetting->loadSetting(file::getPath({"setting", "worksapce"}));

        nero_log("--> Loading dockspace settings");
        m_EditorSetting->loadSetting(file::getPath({"setting", "dockspace"}));
        m_EditorSetting->getSetting("dockspace")
            .setBool("imgui_setting_exist",
                     file::fileExist(file::getPath({"setting", EditorConstant.FILE_IMGUI_SETTING},
                                                   StringPool.EXT_INI)));

        nero_log("--> Loading startup settings");
        checkWorkspace();
        m_EditorSetting->loadSetting(file::getPath({"setting", "startup"}));
    }

    void GameEditor::checkRecentProject()
    {
        if(!file::fileExist(file::getPath({"setting", "recent_project"}, StringPool.EXT_JSON)))
        {
            file::saveFile(file::getPath({"setting", "recent_project"}, StringPool.EXT_JSON),
                           nlohmann::json::array().dump(3));

            return;
        }

        auto previousRecentProjectTable =
            file::loadJson(file::getPath({"setting", "recent_project"}));

        nlohmann::json recentProjectTable = nlohmann::json::array();

        for(auto project : previousRecentProjectTable)
        {
            if(file::fileExist(file::getPath({project["project_directory"], ".project"})))
            {
                recentProjectTable.push_back(project);
            }
        }

        file::saveFile(file::getPath({"setting", "recent_project"}, StringPool.EXT_JSON),
                       recentProjectTable.dump(3),
                       true);
    }

    void GameEditor::checkWorkspace()
    {
        if(!file::fileExist(file::getPath({"setting", "workspace"}, StringPool.EXT_JSON)))
        {
            file::saveFile(file::getPath({"setting", "workspace"}, StringPool.EXT_JSON),
                           nlohmann::json::array().dump(3));

            return;
        }

        auto previousWorkspaceTable = file::loadJson(file::getPath({"setting", "workspace"}));

        nlohmann::json workspaceTable;

        for(auto workspace : previousWorkspaceTable)
        {
            if(file::fileExist(file::getPath({workspace["workspace_directory"], ".workspace"})))
            {
                workspaceTable.push_back(workspace);
            }
        }

        file::saveFile(file::getPath({"setting", "workspace"}, StringPool.EXT_JSON),
                       workspaceTable.dump(3),
                       true);
    }

    void GameEditor::checkEnvironmentVariable()
    {
        nero_log("-> Checking environment variable");

        char*       env_home      = getenv("NERO_GAME_HOME");
        char*       env_vs        = getenv("NERO_GAME_VS");
        char*       env_qt        = getenv("NERO_GAME_QT");
        char*       env_tp        = getenv("NERO_GAME_TP");

        std::string neroGameHome  = env_home != nullptr ? std::string(env_home) : StringPool.BLANK;
        std::string visualStudio  = env_vs != nullptr ? std::string(env_vs) : StringPool.BLANK;
        std::string qtCreator     = env_qt != nullptr ? std::string(env_qt) : StringPool.BLANK;
        std::string texturePacker = env_tp != nullptr ? std::string(env_tp) : StringPool.BLANK;

        Setting     environment;

        environment.setString("nero_game_home", neroGameHome);
        environment.setString("visual_studio", visualStudio);
        environment.setString("qt_creator", qtCreator);
        environment.setString("texture_packer", texturePacker);

        m_EditorSetting->setSetting("environment", environment);

        if(env_home)
        {
            nero_log("--> NERO_GAME_HOME = " + neroGameHome);
        }
        else
        {
            nero_log("--> NERO_GAME_HOME not found");
        }

        if(env_vs)
        {
            nero_log("--> NERO_GAME_VS = " + visualStudio);
        }
        else
        {
            nero_log("--> NERO_GAME_VS not found");
        }

        if(env_qt)
        {
            nero_log("--> NERO_GAME_QT = " + qtCreator);
        }
        else
        {
            nero_log("--> NERO_GAME_QT not found");
        }

        if(env_tp)
        {
            nero_log("--> NERO_GAME_TP = " + texturePacker);
        }
        else
        {
            nero_log("--> NERO_GAME_TP not found");
        }

        env_home = nullptr;
        env_vs   = nullptr;
        env_qt   = nullptr;
        env_tp   = nullptr;
    }

    void GameEditor::loadEditorResource()
    {
        nero_log("-> Loading editor resource");

        m_EditorTextureHolder = std::make_shared<TextureHolder>(
            m_EditorSetting->getSetting("resource").getSetting("texture"));
        m_EditorTextureHolder->loadDirectory("resource/editor/texture");

        m_EditorSoundHolder = std::make_shared<SoundHolder>(
            m_EditorSetting->getSetting("resource").getSetting("sound"));
        m_EditorSoundHolder->loadDirectory("resource/editor/sound");

        m_EditorFontHolder = std::make_shared<FontHolder>(
            m_EditorSetting->getSetting("resource").getSetting("font"));
        m_EditorFontHolder->loadDirectory("resource/editor/font");
    }

    void GameEditor::createCamera()
    {
        m_EditorCamera = std::make_shared<EditorCamera>();
    }

    void GameEditor::createEditorInterface()
    {
        nero_log("-> Creating editor interface");

        m_EditorUI = std::make_unique<EditorUI>(m_RenderWindow,
                                                m_EditorCamera,
                                                m_EditorTextureHolder,
                                                m_EditorFontHolder,
                                                m_EditorSoundHolder,
                                                m_EditorSetting);

        // Set callback, allow interface to change window title
        m_EditorUI->setUpdateWindowTitleCallback(
            [this](const std::string& title)
            {
                setWindowTitle(EngineConstant::ENGINE_NAME + " - " + title);
            });

        m_EditorUI->init();
    }

    void GameEditor::openLastProject()
    {
        if(m_EditorSetting->getSetting("startup").getBool("open_last_project"))
        {
            nero_log("opening last project");

            auto recent_project = file::loadJson(file::getPath({"setting", "recent_project"}));

            if(!recent_project.empty())
            {
                std::string project_directory = recent_project.back()["project_directory"];

                // open project
                m_EditorUI->m_ProjectManager->openProject(project_directory);
            }
            else
            {
                nero_log("recent projects are emtpy");
            }
        }
    }
} // namespace nero
