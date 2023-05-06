////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Poco
#include <Poco/Environment.h>
// Nero
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/editor/EditorUI.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
// SFML
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Icon Font
#include <iconfont/IconsFontAwesome5.h>
// Profiler
#include <easy/profiler.h>
// Std
#include <functional>
#include <vector>
#include <exception>
#include <csignal>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorUI::EditorUI(sf::RenderWindow&   window,
                       AdvancedCamera::Ptr camera,
                       TextureHolder::Ptr  textureHolder,
                       FontHolder::Ptr     fontHolder,
                       SoundHolder::Ptr    soundHolder,
                       Setting::Ptr        setting)
        // Constructor paramater
        : m_RenderWindow(window)
        , m_EditorCamera(camera)
        , m_EditorTextureHolder(textureHolder)
        , m_EditorFontHolder(fontHolder)
        , m_EditorSoundHolder(soundHolder)
        , m_EditorSetting(setting)
        // Main paramater
        , m_ProjectManager(std::make_shared<ProjectManager>(m_EditorSetting, m_EditorFontHolder))
        , m_NotificationManager(std::make_shared<NotificationManager>())
        , m_EditorProxy(std::make_shared<EditorProxy>())
        , m_RenderTexture(std::make_shared<sf::RenderTexture>())
        , m_RenderContext(std::make_shared<RenderContext>())
        , m_EditorContext(std::make_shared<EditorContext>(m_EditorProxy,
                                                          m_ProjectManager,
                                                          m_NotificationManager,
                                                          m_EditorTextureHolder,
                                                          m_EditorFontHolder,
                                                          m_EditorSetting,
                                                          m_RenderTexture,
                                                          m_RenderContext,
                                                          m_EditorCamera))
        , m_EditorSetup(std::make_shared<EditorSetup>(m_EditorContext))
        // Auto Save
        , m_AutoSaveClock()
        , m_AutoSaveTimeInterval(60)
        // Node Editor
        , m_NodeEditorContext(ax::NodeEditor::CreateEditor())
        // Initial Draw
        , m_EditorInitialDraw(true)
        // User Interface
        , m_EditorDockspace(m_EditorContext)
        , m_EditorToolbar(m_EditorContext)
        , m_EditorSetupPopup(m_EditorContext, m_EditorSetup)
        , m_ResourceSelectionWindow(m_EditorContext)
        , m_ResourceBrowserWindow(m_EditorContext)
        , m_SceneExplorerWindow(m_EditorContext)
        , m_EngineHelpWindow(m_EditorContext)
        , m_EditorUtilityWindow(m_EditorContext)
        , m_WorldChunkWindow(m_EditorContext)
        , m_ObjectLayerWindow(m_EditorContext)
        , m_GameLevelWindow(m_EditorContext)
        , m_ConsoleWindow(m_EditorContext)
        , m_LoggerWindow(m_EditorContext)
        , m_RenderCanvasWindow(m_EditorContext)
        , m_GameProjectWindow(m_EditorContext)
        , m_GameSettingWindow(m_EditorContext)
        , m_NotificationWindow(m_EditorContext)
        , m_NodeEditorWindow(m_EditorContext, m_NodeEditorContext)
    {
        setupEditorProxy();
    }

    EditorUI::~EditorUI()
    {
        destroy();
    }

    void EditorUI::destroy()
    {
        ax::NodeEditor::DestroyEditor(m_NodeEditorContext);
    }

    void EditorUI::init()
    {
        ImGuiIO& io                          = ImGui::GetIO();
        io.ConfigFlags                       |= ImGuiConfigFlags_DockingEnable;
        io.ConfigWindowsMoveFromTitleBarOnly = true;

        io.Fonts->Clear();
        io.Fonts->AddFontDefault();
        ImGui::SFML::UpdateFontTexture();

        ImFontConfig config;
        config.MergeMode                   = true;
        config.PixelSnapH                  = true;
        config.GlyphMinAdvanceX            = 13.0f; // Use if you want to make the icon monospaced
        static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        ImGui::SFML::UpdateFontTexture();

        io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation.ttf", 17.f);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        ImGui::SFML::UpdateFontTexture();

        io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation.ttf", 25.f);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf",
                                     18.0f,
                                     &config,
                                     icon_ranges);
        ImGui::SFML::UpdateFontTexture();

        io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation.ttf", 14.f);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf",
                                     14.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf",
                                     14.0f,
                                     &config,
                                     icon_ranges);
        io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf",
                                     14.0f,
                                     &config,
                                     icon_ranges);
        ImGui::SFML::UpdateFontTexture();

        // register signal handler
        registerSignalHandler();
    }

    void EditorUI::handleEvent(const sf::Event& event)
    {
        const bool mouseHoverCanvas = mouseOnCanvas();

        if(mouseHoverCanvas)
        {
            m_EditorCamera->handleEvent(event);
        }

        const auto editorMode   = m_EditorContext->getEditorMode();
        const auto builderMode  = m_EditorContext->getBuilderMode();
        auto       levelBuilder = m_EditorContext->getLevelBuilder();

        if(editorMode == EditorMode::World_Builder && levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(worldBuilder && builderMode == BuilderMode::Object && mouseHoverCanvas)
            {
                worldBuilder->handleEvent(event);
            }
            else if(worldBuilder && builderMode == BuilderMode::Mesh && mouseHoverCanvas)
            {
                worldBuilder->getMeshEditor()->handleEvent(event);
            }
        }
        else if(editorMode == EditorMode::Play_Game)
        {
            m_EditorContext->getAdvancedScene()->handleEvent(event);
        }

        switch(event.type)
        {
            case sf::Event::Closed:
                m_EditorProxy->closeEditor();
                break;

                // Keyboard
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;
        }

        ImGui::SFML::ProcessEvent(event);
    }

    void EditorUI::update(const sf::Time& timeStep)
    {
        m_EditorCamera->update(timeStep);

        m_EditorMode      = m_EditorContext->getEditorMode();
        m_BuilderMode     = m_EditorContext->getBuilderMode();

        auto levelBuilder = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(m_EditorMode == EditorMode::World_Builder && m_BuilderMode == BuilderMode::Object &&
               worldBuilder)
            {
                worldBuilder->update(timeStep);
            }
        }

        if(levelBuilder &&
           (m_EditorMode == EditorMode::World_Builder ||
            m_EditorMode == EditorMode::Screen_Builder || m_EditorMode == EditorMode::Factory))
        {
            m_EditorProxy->autoSave();
        }

        m_NotificationManager->update(timeStep);

        if(m_EditorMode == EditorMode::Play_Game)
        {
            m_EditorContext->getAdvancedScene()->update(timeStep);
        }

        m_RenderCanvasWindow.update(timeStep);
        m_GameLevelWindow.update(timeStep);
        m_EditorToolbar.update(timeStep);
        m_SceneExplorerWindow.update(timeStep);
    }

    void EditorUI::render()
    {
        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);

        // Head
        m_EditorDockspace.render();
        m_EditorToolbar.render();

        // Center
        m_RenderCanvasWindow.render();
        m_GameProjectWindow.render();
        m_LoggerWindow.render();

        if(m_EditorMode != EditorMode::Play_Game)
        {

            m_ResourceSelectionWindow.render();

            if(m_EditorContext->getAdvancedScene())
            {
                m_GameLevelWindow.render();
            }

            // Left
            m_EditorUtilityWindow.render();

            if(m_EditorContext->getLevelBuilder())
            {
                m_WorldChunkWindow.render();
            }

            if(m_EditorMode != EditorMode::World_Builder)
            {
                // m_GameScreenWindow.render();
            }

            if(m_EditorContext->getLevelBuilder())
            {
                m_ObjectLayerWindow.render();
            }

            // Far left
            m_ResourceBrowserWindow.render();
        }

        m_SceneExplorerWindow.render();

        // First Draw Setup
        editorInitialDraw();

        // Notification & Background task
        m_NotificationWindow.render();

        // Startup Popup
        if(m_EditorSetup->initiateSetup())
            m_EditorSetupPopup.render();

        // Actual Rendering
        ImGui::SFML::Render(m_RenderWindow);
    }

    void EditorUI::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        const bool mouseHoverCanvas = mouseOnCanvas() && m_RenderContext->focus;

        if(isPressed)
        {
            // Switch between Builder Modes
            if(key == sf::Keyboard::Space && mouseHoverCanvas)
            {
                switchBuilderMode();
            }
            // Play Game
            else if(key == sf::Keyboard::Enter && !keyboard::CTRL_SHIFT_ALT() && mouseHoverCanvas)
            {
                m_EditorProxy->playGameScene();
            }
            // Stop Play Game
            else if(key == sf::Keyboard::Escape && !keyboard::CTRL_SHIFT_ALT() && mouseHoverCanvas)
            {
                m_EditorProxy->stopPlayGameScene();
            }
            // Render Game
            else if(key == sf::Keyboard::Enter && keyboard::CTRL() && mouseHoverCanvas)
            {
                // TODO
                // m_EditorProxy->renderGameScene();
            }
        }
    }

    bool EditorUI::mouseOnCanvas()
    {
        sf::Rect<float> canvas(m_RenderContext->canvasPosition.x,
                               m_RenderContext->canvasPosition.y,
                               m_RenderContext->canvasSize.x,
                               m_RenderContext->canvasSize.y);

        sf::Vector2i    mousePosition = ImGui::GetMousePos();

        return canvas.contains(mousePosition.x, mousePosition.y);
    }

    void EditorUI::updateFrameRate(const float& frameRate, const float& frameTime)
    {
        m_EditorContext->setFrameRate(frameRate);
        m_EditorContext->setFrameTime(frameTime);
    }

    void EditorUI::setUpdateWindowTitleCallback(std::function<void(const std::string&)> callback)
    {
        m_WindowTitleCallback = callback;
    }

    void EditorUI::switchBuilderMode()
    {
        const auto editorMode  = m_EditorContext->getEditorMode();
        const auto builderMode = m_EditorContext->getBuilderMode();

        if(editorMode == EditorMode::World_Builder || editorMode == EditorMode::Factory)
        {
            switch(builderMode)
            {
                case BuilderMode::Object:
                {
                    if(!keyboard::CTRL_SHIFT_ALT())
                        m_EditorContext->setBuilderMode(BuilderMode::Mesh);
                    // TODO
                    /*else if(keyboard::CTRL())
                        m_EditorContext->setBuilderMode(BuilderMode::Joint);*/
                }
                break;
                case BuilderMode::Mesh:
                {
                    if(!keyboard::CTRL_SHIFT_ALT())
                        m_EditorContext->setBuilderMode(BuilderMode::Object);
                    // TODO
                    /*else if(keyboard::CTRL())
                        m_EditorContext->setBuilderMode(BuilderMode::Joint);*/
                }
                break;
                case BuilderMode::Joint:
                {
                    if(!keyboard::CTRL_SHIFT_ALT())
                        m_EditorContext->setBuilderMode(BuilderMode::Object);
                }
                break;
                case BuilderMode::None:
                    break;
            }
        }
    }

    void EditorUI::editorInitialDraw()
    {
        if(m_EditorInitialDraw)
        {
            ImGui::SetWindowFocus(EditorConstant.WINDOW_UTILITY.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_LAYER.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_RESOURCE.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_EXPLORER.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_CONSOLE.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_LOGGING.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());

            // commit
            m_EditorInitialDraw = false;
        }
    }

    void EditorUI::registerSignalHandler()
    {
        signal(SIGABRT, handleSignalAbnormalTermination);
        signal(SIGFPE, handleSignalArithmeticError);
        signal(SIGILL, handleSignalIllegalInstruction);
        signal(SIGINT, handleSignalInteractiveAttention);
        signal(SIGSEGV, handleSignalInvalidStorageAccess);
        signal(SIGTERM, handleSignalTerminsationRequest);
    }

    void EditorUI::handleSignalAbnormalTermination(int signal)
    {
        // TODO
    }

    void EditorUI::handleSignalArithmeticError(int signal)
    {
        // TODO
    }

    void EditorUI::handleSignalIllegalInstruction(int signal)
    {
        // TODO
    }

    void EditorUI::handleSignalInteractiveAttention(int signal)
    {
        // TODO
    }

    void EditorUI::handleSignalInvalidStorageAccess(int signal)
    {
        // TODO
    }

    void EditorUI::handleSignalTerminsationRequest(int signal)
    {
        // TODO
    }

    void EditorUI::setupEditorProxy()
    {
        // Open project
        m_EditorProxy->m_OpenProjectCallback = [this](const std::string& projectDirectory)
        {
            m_ProjectManager->openProject(projectDirectory);
            auto gameProject   = m_EditorContext->getGameProject();
            auto advancedScene = gameProject->getAdvancedScene();
            advancedScene->setRenderTexture(m_RenderTexture);
            advancedScene->setRenderContext(m_RenderContext);
            advancedScene->setEditorCamera(m_EditorCamera);
            advancedScene->init();
            gameProject->loadLibrary();

            // Update Editor window title
            m_WindowTitleCallback(gameProject->getProjectName());

            // Update Editor mode
            m_EditorContext->setEditorMode(EditorMode::World_Builder);
            m_EditorContext->setBuilderMode(BuilderMode::Object);

            // Restart auto save clock count
            m_AutoSaveClock.restart();
        };

        // Create project
        m_EditorProxy->m_CreateProjectCallback =
            [this](const Parameter& projectParameter, const unsigned int& projectCount)
        {
            nero_log("on create project in background") std::string taskName =
                EditorConstant.TASK_CREATE_PROJECT + toString(projectCount);
            BTManager::startTask(&ProjectManager::createProject,
                                 m_ProjectManager.get(),
                                 projectParameter,
                                 taskName);
        };

        m_EditorProxy->m_SaveProjectCallback = [this]()
        {
            // Saving game level
            auto levelBuilder = m_EditorContext->getLevelBuilder();
            if(levelBuilder)
            {
                levelBuilder->saveGameLevel();
            }
        };

        // Close project
        m_EditorProxy->m_CloseProjectCallback = [this]()
        {
            m_ProjectManager->closeProject();

            // update editor window title
            m_WindowTitleCallback(EngineConstant.ENGINE_WINDOW_TITLE);
        };

        // Create workspace
        m_EditorProxy->m_CreateWorkspaceCallback = [this](const Parameter& workspaceParameter)
        {
            m_ProjectManager->createWorkspace(workspaceParameter);
        };

        // Import workspace
        m_EditorProxy->m_ImportWorkspaceCallback = [this](const std::string& workspaceDirectory)
        {
            m_ProjectManager->importWorkspace(workspaceDirectory);
        };

        // Close editor
        m_EditorProxy->m_CloseEditorCallback = [this]()
        {
            nero_log("Closing the Editor");

            nero_log("-> Saving window settings");
            std::string windowSettingFile =
                file::getPath({"setting", "window"}, StringPool.EXT_JSON);
            Setting      windowSetting  = m_EditorSetting->getSetting("window");
            sf::Vector2u windowSize     = m_RenderWindow.getSize();
            sf::Vector2i windowPosition = m_RenderWindow.getPosition();

            if(windowPosition.x < -8)
            {
                windowPosition.x = -8;
            }
            if(windowPosition.y < 0)
            {
                windowPosition.y = 0;
            }

            windowSetting.setUInt("width", windowSize.x);
            windowSetting.setUInt("height", windowSize.y);
            windowSetting.setInt("position_x", windowPosition.x);
            windowSetting.setInt("position_y", windowPosition.y);

            file::saveFile(windowSettingFile, windowSetting.toJson().dump(3), true);

            if(m_EditorContext->getGameProject())
            {
                nero_log("-> Closing project");
                m_EditorProxy->closeProject();
            }

            nero_log("---");
            nero_log("Editor closed");

            m_RenderWindow.close();
        };

        // Create workspace
        m_EditorProxy->m_CreateGameLevelCallback = [this](const Parameter& levelParameter)
        {
            auto advancedScene = m_EditorContext->getAdvancedScene();

            // Advanced Scene not available
            if(!advancedScene)
                return;

            // Create new Game Level
            const std::string levelName = advancedScene->createLevel(levelParameter);

            if(levelName != StringPool.BLANK)
            {
                m_EditorContext->setSelectedGameLevelName(levelName);
                m_EditorProxy->openGameLevel(levelName);
            }
        };

        m_EditorProxy->m_OpenGameLevelCallback = [this](const std::string levelName)
        {
            auto advancedScene = m_EditorContext->getAdvancedScene();

            // Advanced Scene not available
            if(!advancedScene)
                return;

            // Selected game level is already open
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder && levelBuilder->getLevelName() == levelName)
            {
                return;
            }
            advancedScene->openLevel(levelName);
            m_EditorContext->setOpenedGameLevelName(levelName);
        };

        m_EditorProxy->m_RemoveGameLevelCallback = [this](const std::string levelName)
        {
            if(levelName.empty())
                return;

            auto advancedScene = m_EditorContext->getAdvancedScene();

            // Advanced Scene not available
            if(!advancedScene)
                return;

            if(advancedScene->removeLevel(levelName))
            {
                m_EditorProxy->compileProject();
            }
        };

        m_EditorProxy->m_OpenCodeEditorCallback = [this]()
        {
            auto gameProject = m_EditorContext->getGameProject();

            if(gameProject)
            {
                gameProject->openEditor();
                m_EditorContext->getNotificationManager()->notify("Code Editor Opened");
            }
        };

        m_EditorProxy->m_CompileProjectCallback = [this]()
        {
            if(BTManager::CompilingProject)
                return;

            auto gameProject = m_EditorContext->getGameProject();

            if(gameProject)
            {
                BTManager::startTask(
                    [gameProject](BackgroundTask::Ptr backgroundTask)
                    {
                        GameProject::compileProject(gameProject->getProjectDirectory(),
                                                    backgroundTask);
                    });
            }
        };

        m_EditorProxy->m_ReloadProjectLibraryCallback = [this]()
        {
            auto gameProject = m_EditorContext->getGameProject();

            if(!gameProject)
                return;

            if(gameProject->loadLibrary())
            {
                m_EditorContext->getNotificationManager()->notify("DLL loaded", 10.f);
            }
            else
            {
                m_EditorContext->getNotificationManager()->notify("Failed to load DLL");
            }
        };

        m_EditorProxy->m_PlayGameSceneCallback = [this]()
        {
            if(m_EditorContext->getEditorMode() == EditorMode::World_Builder)
            {
                m_EditorContext->getAdvancedScene()->buildGameScene();
                m_EditorContext->setEditorMode(EditorMode::Play_Game);
            }
        };

        m_EditorProxy->m_StopPlayGameSceneCallback = [this]()
        {
            if(m_EditorContext->getEditorMode() == EditorMode::Play_Game)
            {
                m_EditorContext->setEditorMode(EditorMode::World_Builder);
            }
        };

        m_EditorProxy->m_AutoSaveCallback = [this]()
        {
            if(m_EditorContext->getGameProject() &&
               m_AutoSaveClock.getElapsedTime() > sf::seconds(m_AutoSaveTimeInterval))
            {
                m_AutoSaveClock.restart();

                if(m_EditorContext->autoSaveEnabled())
                {
                    m_EditorProxy->saveProject();
                    m_EditorContext->getNotificationManager()->notify("Project auto saved");
                }
            }
        };
    }
} // namespace nero
