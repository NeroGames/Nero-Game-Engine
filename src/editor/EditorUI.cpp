////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Poco
#include <Poco/Environment.h>
// SFML
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/editor/EditorUI.h>
#include <SFML/Window/Event.hpp>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/File.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Nero/editor/EditorConstant.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
#include <functional>
#include <vector>
#include <iconfont/IconsFontAwesome5.h>
#include <easy/profiler.h>
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
        , m_ProjectManager(std::make_unique<ProjectManager>(m_EditorSetting))
        , m_EditorProxy(std::make_shared<EditorProxy>())
        , m_RenderTexture(std::make_shared<sf::RenderTexture>())
        , m_RenderContext(std::make_shared<RenderContext>())
        , m_EditorContext(std::make_shared<EditorContext>(m_EditorProxy,
                                                          m_ProjectManager,
                                                          m_EditorTextureHolder,
                                                          m_EditorFontHolder,
                                                          m_EditorSetting,
                                                          m_RenderTexture,
                                                          m_RenderContext,
                                                          m_EditorCamera))
        , m_EditorSetup(std::make_shared<EditorSetup>(m_EditorContext))
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
        //
        , m_InterfaceFirstDraw(true)
        , m_SelectedScriptTypeIndex(0)
        , m_SelectedGameLevelIndex(0)
        , m_SelectedGameScreenIndex(0)
        , g_Context(nullptr)
        , m_BottomDockspaceTabBarSwitch()
        , m_InputSelectedGameScreenId(-1)
    {
        setupEditorProxy();
    }

    EditorUI::~EditorUI()
    {
        destroy();
    }

    void EditorUI::destroy()
    {
        nero_log("destroying engine interface ...");
        ax::NodeEditor::DestroyEditor(g_Context);
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

        g_Context = ax::NodeEditor::CreateEditor();

        m_BottomDockspaceTabBarSwitch.registerTab({
            EditorConstant.WINDOW_RESOURCE,
            EditorConstant.WINDOW_LOGGING,
        });

        // clear workspace input
        clearScriptWizardInput();

        // register signal handler
        registerSignalHandler();
    }

    void EditorUI::handleEvent(const sf::Event& event)
    {
        if(mouseOnCanvas())
        {
            m_EditorCamera->handleEvent(event);
        }

        const auto editorMode   = m_EditorContext->getEditorMode();
        const auto builderMode  = m_EditorContext->getBuilderMode();
        auto       levelBuilder = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(worldBuilder && editorMode == EditorMode::WORLD_BUILDER &&
               builderMode == BuilderMode::OBJECT && mouseOnCanvas())
            {
                worldBuilder->handleEvent(event);
            }
            else if(worldBuilder && editorMode == EditorMode::WORLD_BUILDER &&
                    builderMode == BuilderMode::MESH && mouseOnCanvas())
            {
                worldBuilder->getMeshEditor()->handleEvent(event);
            }
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

        const auto editorMode   = m_EditorContext->getEditorMode();
        const auto builderMode  = m_EditorContext->getBuilderMode();

        auto       levelBuilder = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::OBJECT &&
               worldBuilder)
            {
                worldBuilder->update(timeStep);
            }
        }
    }

    void EditorUI::render()
    {
        const auto editorMode = m_EditorContext->getEditorMode();

        EASY_FUNCTION(profiler::colors::Red)

        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);

        m_EditorDockspace.render();

        // Central dockspcace
        m_EditorToolbar.render();
        // viewport
        m_RenderCanvasWindow.render();
        // game project
        showGameProjectWindow();
        // game setting
        showGameSettingWindow();
        // visual script
        showVisualScriptWindow();
        // resource manager
        m_ResourceSelectionWindow.render();
        // imgui demo
        ImGui::ShowDemoWindow();

        // left dockspace
        // upper left
        m_EditorUtilityWindow.render();
        /*if(editorMode == EditorMode::SCREEN_BUILDER)
                        showGameScreenWindow();*/

        // lower left
        m_ObjectLayerWindow.render();
        if(editorMode == EditorMode::WORLD_BUILDER)
            m_WorldChunkWindow.render();

        // right dockspace
        m_SceneExplorerWindow.render();
        m_EngineHelpWindow.render();
        m_ResourceBrowserWindow.render();

        // bottom dockspacer
        m_LoggerWindow.render();
        m_ConsoleWindow.render();

        if(m_EditorContext->getAdvancedScene() && editorMode == EditorMode::WORLD_BUILDER)
        {
            m_GameLevelWindow.render();
        }

        // init
        interfaceFirstDraw();

        // background task
        showBackgroundTaskWindow();

        if(m_EditorSetup->initiateSetup())
        {
            m_EditorSetupPopup.render();
        }

        // commit
        ImGui::SFML::Render(m_RenderWindow);
    }

    void EditorUI::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        if(isPressed)
        {
            if(key == sf::Keyboard::Space && !keyboard::CTRL_SHIFT_ALT() && mouseOnCanvas())
                switchBuilderMode();
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
        m_UpdateWindowTitleCallback = callback;
    }

    void EditorUI::switchBuilderMode()
    {
        const auto editorMode  = m_EditorContext->getEditorMode();
        const auto builderMode = m_EditorContext->getBuilderMode();

        if(editorMode == EditorMode::WORLD_BUILDER)
        {
            if(builderMode == BuilderMode::OBJECT && !keyboard::CTRL_SHIFT_ALT())
            {
                m_EditorContext->setBuilderMode(BuilderMode::MESH);
            }
            else if(builderMode != BuilderMode::OBJECT && !keyboard::CTRL_SHIFT_ALT())
            {
                m_EditorContext->setBuilderMode(BuilderMode::OBJECT);
            }
        }
        else if(editorMode == EditorMode::PLAY_GAME && !keyboard::CTRL_SHIFT_ALT())
        {
            m_EditorContext->setEditorMode(EditorMode::WORLD_BUILDER);
            m_EditorContext->setBuilderMode(BuilderMode::OBJECT);
        }
    }

    void EditorUI::showGameSettingWindow()
    {
        ImGui::Begin(EditorConstant.WINDOW_GAME_SETTING.c_str());

        ImGui::End();
    }

    void EditorUI::showVisualScriptWindow()
    {
        ImGui::Begin(EditorConstant.WINDOW_VISUAL_SCRIPT.c_str());

        ax::NodeEditor::SetCurrentEditor(g_Context);

        ax::NodeEditor::Begin("My Editor");

        int uniqueId = 1;

        // Start drawing nodes.
        ax::NodeEditor::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
        ImGui::Text("-> In");
        ax::NodeEditor::EndPin();
        ImGui::SameLine();
        ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
        ImGui::Text("Out ->");
        ax::NodeEditor::EndPin();
        ax::NodeEditor::EndNode();

        ax::NodeEditor::End();

        ImGui::End();
    }

    void EditorUI::showGameProjectWindow()
    {
        ImGui::Begin(EditorConstant.WINDOW_GAME_PROJECT.c_str());

        ImGui::End();
    }

    void EditorUI::showScriptCreationWindow()
    {
        // Window flags
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoScrollbar;
        // Winsow size
        ImVec2 winsow_size = EditorConstant.WINDOW_SCRIPT_WIZARD_SIZE;

        // Project manager window
        ImGui::SetNextWindowSize(winsow_size);
        // Begin window
        if(ImGui::BeginPopupModal(EditorConstant.WINDOW_SCRIPT_WIZARD.c_str(),
                                  nullptr,
                                  window_flags))
        {

            if(ImGui::BeginTabBar("##scrip_wizard_tabbar"))
            {
                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                float wording_width = 130.f;
                float input_width   = ImGui::GetWindowContentRegionWidth() - 130.f;

                if(ImGui::BeginTabItem("CPP Script"))
                {
                    ImGui::Text("Class Name");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width);
                    ImGui::InputText("##class_name", m_InputClassName, sizeof(m_InputClassName));
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Parent Class");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width - 110.f);
                    ImGui::InputText("##parent_class",
                                     m_InputParentClass,
                                     sizeof(m_InputParentClass),
                                     ImGuiInputTextFlags_ReadOnly);
                    ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 100.f);
                    if(ImGui::Button("Select##select_script_parent_class", ImVec2(100.f, 0)))
                    {
                        ImGui::OpenPopup("Select Script Class");
                    }

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    // ImGui::Separator();

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    /*if(std::string(m_InputParentClass) == "Game Level Script")
                    {
                            ImGui::Text("Game Level");
                            ImGui::SameLine(wording_width);
                            ImGui::SetNextItemWidth(input_width);

                            //load workpsace
                            std::vector<std::string> gameLevelNameTable =
                    m_AdvancedScene->getGameLevelNameTable();

                            gameLevelNameTable.insert(gameLevelNameTable.begin(), StringPool.BLANK);

                            if(gameLevelNameTable.empty())
                            {
                                    gameLevelNameTable.push_back("There is no Level availabled");
                            }

                            std::size_t levelCount = gameLevelNameTable.size();
                            const char** levelComboTable = new const char* [levelCount];

                            string::fillCharTable(levelComboTable, gameLevelNameTable);

                            m_SelectedGameLevel = levelComboTable[m_SelectedGameLevelIndex];
                            if (ImGui::BeginCombo("##game_level_combo", m_SelectedGameLevel,
                    ImGuiComboFlags())) // The second parameter is the label previewed before
                    opening the combo.
                            {
                                    for (int n = 0; n < levelCount; n++)
                                    {
                                            bool is_selected = (m_SelectedGameLevel ==
                    levelComboTable[n]);


                                            if (ImGui::Selectable(levelComboTable[n], is_selected))
                                            {
                                                    m_SelectedGameLevel = levelComboTable[n];
                                                    m_SelectedGameLevelIndex = n;

                                            }

                                            if (is_selected)
                                            {
                                                     ImGui::SetItemDefaultFocus();
                                            }
                                    }
                                    ImGui::EndCombo();
                            }

                            //delete array
                            for (std::size_t i = 0 ; i < levelCount; i++)
                            {
                                    delete[] levelComboTable[i] ;
                            }
                            delete[] levelComboTable ;
                            levelComboTable = nullptr;

                    }
                    else if(std::string(m_InputParentClass) == "Game Screen Script")
                    {
                            ImGui::Text("Screen Level");
                            ImGui::SameLine(wording_width);
                            ImGui::SetNextItemWidth(input_width);

                            //load workpsace
                            std::vector<std::string> gameScreenNameTable =
                    m_AdvancedScene->getGameScreenNameTable();

                            if(gameScreenNameTable.empty())
                            {
                                    gameScreenNameTable.push_back("There is no Level availabled");
                            }

                            std::size_t screenCount = gameScreenNameTable.size();
                            const char** screenComboTable = new const char* [screenCount];

                            string::fillCharTable(screenComboTable, gameScreenNameTable);

                            m_SelectedGameScreen = screenComboTable[m_SelectedGameScreenIndex];
                            if (ImGui::BeginCombo("##game_level_combo", m_SelectedGameScreen,
                    ImGuiComboFlags())) // The second parameter is the label previewed before
                    opening the combo.
                            {
                                    for (int n = 0; n < screenCount; n++)
                                    {
                                            bool is_selected = (m_SelectedGameScreen ==
                    screenComboTable[n]);


                                            if (ImGui::Selectable(screenComboTable[n], is_selected))
                                            {
                                                    m_SelectedGameScreen = screenComboTable[n];
                                                    m_SelectedGameScreenIndex = n;

                                            }

                                            if (is_selected)
                                            {
                                                     ImGui::SetItemDefaultFocus();
                                            }
                                    }
                                    ImGui::EndCombo();
                            }

                            //delete array
                            for (std::size_t i = 0 ; i < screenCount; i++)
                            {
                                    delete[] screenComboTable[i] ;
                            }
                            delete[] screenComboTable ;
                            screenComboTable = nullptr;

                    }*/

                    ImGui::SetNextWindowSize(ImVec2(500.f, 400.f));
                    if(ImGui::BeginPopupModal("Select Script Class", nullptr, window_flags))
                    {
                        std::vector<std::string> script_type = {"Game Level Script",
                                                                "Game Screen Script",
                                                                "Startup Screen Script",
                                                                "Physic Script Object",
                                                                "Simple Script Object",
                                                                "Action Object",
                                                                "Action"};

                        for(auto type : script_type)
                        {
                            ImGui::Text(type.c_str());
                            ImGui::Separator();
                            ImGui::Text(
                                "Use a Game Level Script to manage the behavior of a Game Level \n"
                                "two line description");
                            ImGui::Dummy(ImVec2(0.f, 2.f));
                            if(ImGui::Button(type.c_str(),
                                             ImVec2(ImGui::GetWindowContentRegionWidth(), 50.f)))
                            {
                                string::fillCharArray(m_InputParentClass,
                                                      sizeof(m_InputParentClass),
                                                      type);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::Dummy(ImVec2(0.f, 5.f));
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::EndTabItem();
                }

                if(ImGui::BeginTabItem("Lua Script"))
                {

                    ImGui::EndTabItem();
                }

                ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
                ImGui::SetCursorPosY(winsow_size.y * 0.75f);
                bool onCreate = ImGui::Button("Create##create_script_object", ImVec2(100, 0));

                bool error    = false;

                if(onCreate)
                {

                    if(error)
                    {
                        // ImGui::OpenPopup("Error Creating Project");
                    }
                    else
                    {
                        Parameter parameter;
                        parameter.setString("class_name", std::string(m_InputClassName));
                        parameter.setString("script_type", std::string(m_SelectedScriptType));

                        if(std::string(m_SelectedScriptType) == "Game Level Script")
                        {
                            parameter.setString("level_name",
                                                m_EditorContext->getSelectedGameLevelName());
                        }
                        else if(std::string(m_SelectedScriptType) == "Game Screen Script")
                        {
                            parameter.setString("screen_name", std::string(m_SelectedGameScreen));
                        }

                        clearScriptWizardInput();

                        // createScriptObject(parameter);

                        // ImGui::OpenPopup("Script Created");
                    }
                }

                ImGui::EndTabBar();
            }

            ImGui::SetCursorPosY(winsow_size.y - 38.f);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 4.0f));
            ImGui::SetCursorPosX(winsow_size.x / 2.f - 50.f);
            if(ImGui::Button("Close##close_script_wizard_window", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void EditorUI::playScene()
    {
        if(m_EditorContext->getAdvancedScene() &&
           m_EditorContext->getEditorMode() != EditorMode::PLAY_GAME)
        {
            try
            {
                // m_AdvancedScene->playScene();
                m_EditorContext->setEditorMode(EditorMode::PLAY_GAME);
            }
            catch(std::exception const& e)
            {
            }
        }
    }

    void EditorUI::pauseScene()
    {
        /*if(m_AdvancedScene && editorMode == EditorMode::PLAY_GAME)
                {
                        auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

                        gameLevel->levelSetting->setBool("pause_level",
           !gameLevel->levelSetting->getBool("pause_level"));
                }*/
    }

    void EditorUI::stepScene()
    {

        /*if(m_AdvancedScene && editorMode == EditorMode::PLAY_GAME)
                {
                        auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

                        gameLevel->levelSetting->setBool("pause_scene", true);
                        gameLevel->levelSetting->setBool("single_step", true);
                }*/
    }

    void EditorUI::resetScene()
    {
        /*if(m_AdvancedScene && editorMode == EditorMode::PLAY_GAME)
                {
                        m_AdvancedScene->playScene();
                }*/
    }

    void EditorUI::renderScene()
    {
    }

    void EditorUI::compileProject()
    {
        /*auto gameProject = m_EditorContext->getGameProject();

        if(gameProject)
                {
            BTManager::startTask(&GameProject::compileProject, gameProject.get());
        }*/
    }

    void EditorUI::editProject()
    {
        auto gameProject = m_EditorContext->getGameProject();

        if(gameProject)
        {
            gameProject->openEditor();
        }
    }

    void EditorUI::reloadProject()
    {
        auto gameProject = m_EditorContext->getGameProject();

        if(gameProject)
        {
            nero_log("reloading project ...");

            // gameProject->loadLibrary();
        }
    }

    void EditorUI::onSaveProject()
    {
        auto gameProject = m_EditorContext->getGameProject();

        if(!gameProject)
            return;

        switch(m_EditorContext->getEditorMode())
        {
        case EditorMode::WORLD_BUILDER: {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder)
            {
                levelBuilder->saveGameLevel();
            }
        }
        break;

        case EditorMode::SCREEN_BUILDER: {
            // m_GameProject->saveGameScreen();
        }
        break;
        }
    }

    void EditorUI::onLoadProject()
    {
        auto gameProject = m_EditorContext->getGameProject();

        if(!gameProject)
            return;

        switch(m_EditorContext->getEditorMode())
        {
        case EditorMode::WORLD_BUILDER: {
            // m_GameProject->loadGameLevel();
        }
        break;

        case EditorMode::SCREEN_BUILDER: {
            // m_GameProject->loadGameScreen();
        }
        break;
        case EditorMode::OBJECT_BUILDER:
        case EditorMode::PLAY_GAME:
        case EditorMode::RENDER_GAME:
            break;
        }
    }

    void EditorUI::autoSaveProject()
    {
        /*if(m_GameProject && m_AutoSaveClock.getElapsedTime() >
        sf::seconds(m_EditorSetting->getSetting("editor").getUInt("auto_save_interval")))
        {
                m_GameProject->saveProject();
                m_AutoSaveClock.restart();
        }*/
    }

    void EditorUI::showGameScreenWindow()
    {
        ImGui::Begin(EditorConstant.WINDOW_SCREEN.c_str());

        ImGui::Text("Manage Game Screen");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.f, 4.f));

        ImVec2 button_size = ImVec2((ImGui::GetWindowContentRegionWidth() - 8.f), 0.f);

        if(ImGui::Button("New Game Screen##add_game_screen", button_size))
        {
            ImGui::OpenPopup("Create Game Screen");
        }

        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImGui::BeginChild("##manage_game_screen", ImVec2(), true);

        /*if(m_AdvancedScene)
        {
                m_InputSelectedGameScreenId = m_AdvancedScene->getSelectedGameScreen()->screenId;

                for(const auto& gameScreen : m_AdvancedScene->getGameScreenTable())
                {
                        std::string itemId = "##select_screen" + toString(gameScreen->screenId);
                        ImGui::RadioButton(itemId.c_str(), &m_InputSelectedGameScreenId,
        gameScreen->screenId);

                        if(ImGui::IsItemClicked())
                        {
                                m_AdvancedScene->setSelectedGameScreen(gameScreen);
                        }

                        ImGui::SameLine();

                        itemId = "##screen_visible" + toString(gameScreen->screenId);
                        ImGui::Checkbox(itemId.c_str(), &gameScreen->visible);

                        ImGui::SameLine();

                        char screen_name[100];
                        string::fillCharArray(screen_name, sizeof(screen_name), gameScreen->name);
                        ImGui::SetNextItemWidth(118.f);
                        itemId = "##screen_name" + toString(gameScreen->screenId);
                        ImGui::InputText(itemId.c_str(), screen_name, sizeof(screen_name));

                        if(ImGui::IsItemEdited())
                        {
                                gameScreen->name = std::string(screen_name);
                        }
                }
        }*/

        ImGui::EndChild();

        showNewGameScreenPopup();

        ImGui::End();
    }

    void EditorUI::createGameScreen(const Parameter& parameter)
    {
        auto advancedScene = m_EditorContext->getAdvancedScene();

        if(!advancedScene)
            return;

        advancedScene->createScreen(parameter);
    }

    void EditorUI::showNewGameScreenPopup()
    {
        // Window flags
        /*ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
                //Winsow size
                ImVec2 winsow_size = ImVec2(400.f, 200.f);
                ImGui::SetNextWindowSize(winsow_size);

                //Begin window
                if(ImGui::BeginPopupModal("Create Game Screen", nullptr, window_flags))
                {
                        float wording_width = 130.f;
                        float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;

                        ImGui::Text("Screen Name");
                        ImGui::SameLine(wording_width);
                        ImGui::InputText("##new_screen_name", m_NewGameScreenInput.name,
        sizeof(m_NewGameScreenInput.name)); ImGui::Dummy(ImVec2(0.0f, 1.0f));

                        ImGui::Text("parent");
                        ImGui::SameLine(wording_width);
                        const char* parents[] = { "None"};
                        static int parent = 0;
                        ImGui::Combo("parent", &parent, parents, IM_ARRAYSIZE(parents));
                        ImGui::Dummy(ImVec2(0.0f, 1.0f));


                        ImGui::Text("Template");
                        ImGui::SameLine(wording_width);
                        const char* prototypes[] = { "None"};
                        static int prototype = 0;
                        ImGui::Combo("template", &prototype, prototypes, IM_ARRAYSIZE(prototypes));
                        ImGui::Dummy(ImVec2(0.0f, 1.0f));

                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
                        ImGui::SetCursorPosY(winsow_size.y * 0.85f - 40.f);
                        bool onCreate = ImGui::Button("Create", ImVec2(100, 0));
                        ImGui::Dummy(ImVec2(0.0f, 4.0f));

                        bool error = false;

                        if(onCreate)
                        {
                                if(error)
                                {

                                }
                                else
                                {
                                        Parameter parameter;
                                        parameter.setString("screen_name",
        std::string(m_NewGameScreenInput.name));
                                        //parameter.setString("parent",
        std::string(m_NewGameScreenInput.parent)); parameter.setString("template",
        std::string(m_NewGameScreenInput.prototype));

                                        createGameScreen(parameter);
                                }
                        }

                        ImGui::Separator();
                        ImGui::Dummy(ImVec2(0.0f, 4.0f));
                        ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
                        if (ImGui::Button("Close##close_new_screen", ImVec2(100, 0)))
                        {
                                ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
        }*/
    }

    void EditorUI::interfaceFirstDraw()
    {
        if(m_InterfaceFirstDraw)
        {
            ImGui::SetWindowFocus(EditorConstant.WINDOW_UTILITY.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_LAYER.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_RESOURCE.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_EXPLORER.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_CONSOLE.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_LOGGING.c_str());
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());

            // commit
            m_InterfaceFirstDraw = false;
        }
    }

    void EditorUI::showBackgroundTaskWindow()
    {
        if(!m_EditorContext->getGameProject())
            return;

        // FIXME-VIEWPORT: Select a default viewport
        const float DISTANCE = 10.0f;
        static int  corner   = 3;
        if(corner != -1)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2         window_pos =
                ImVec2((corner & 1) ? (viewport->Pos.x + viewport->Size.x - DISTANCE)
                                    : (viewport->Pos.x + DISTANCE),
                       (corner & 2) ? (viewport->Pos.y + viewport->Size.y - DISTANCE)
                                    : (viewport->Pos.y + DISTANCE));
            ImVec2 window_pos_pivot =
                ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
        }
        // ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.000f, 1.000f, 1.000f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.911f, 0.499f, 0.146f, 1.000f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.00f));
        if(ImGui::Begin("##background_task_window",
                        nullptr,
                        (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDocking |
                            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            auto& taskTable = BTManager::getTaskTable();

            for(BackgroundTask::Ptr task : taskTable)
            {
                if(!task->completed())
                {
                    ImGui::Text(task->getMessage().c_str());
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
    }

    void EditorUI::clearScriptWizardInput()
    {
        string::fillCharArray(m_InputClassName, sizeof(m_InputClassName), StringPool.BLANK);
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
            // open new project
            if(m_ProjectManager)
            {
                m_ProjectManager->openProject(projectDirectory);
                auto advancedScene = m_EditorContext->getAdvancedScene();
                advancedScene->setRenderTexture(m_RenderTexture);
                advancedScene->setRenderContext(m_RenderContext);

                // update editor window title
                m_UpdateWindowTitleCallback(m_EditorContext->getGameProject()->getProjectName());
            }
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
            // TODO
        };

        // Close project
        m_EditorProxy->m_CloseProjectCallback = [this]()
        {
            m_ProjectManager->closeProject();

            // update editor window title
            m_UpdateWindowTitleCallback(EngineConstant.ENGINE_WINDOW_TITLE);
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

            BTManager::startTask(
                [this](BackgroundTask::Ptr backgroundTask)
                {
                    GameProject::compileProject(
                        m_EditorContext->getGameProject()->getProjectDirectory(),
                        backgroundTask);
                });
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
    }
} // namespace nero
