////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Poco
#include <Poco/Environment.h>
//SFML
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
namespace  nero
{
    EditorUI::EditorUI(sf::RenderWindow& window,
                       AdvancedCamera::Ptr camera,
                       TextureHolder::Ptr textureHolder,
                       FontHolder::Ptr fontHolder,
                       SoundHolder::Ptr soundHolder,
                       Setting::Ptr setting)
        // Constructor paramater
        :m_RenderWindow(window)
        ,m_EditorCamera(camera)
        ,m_EditorTextureHolder(textureHolder)
        ,m_EditorFontHolder(fontHolder)
        ,m_EditorSoundHolder(soundHolder)
        ,m_EditorSetting(setting)
        // Main paramater
        ,m_ProjectManager(std::make_unique<ProjectManager>())
        ,m_EditorProxy(std::make_shared<EditorProxy>())
        ,m_RenderTexture(std::make_shared<sf::RenderTexture>())
        ,m_RenderContext(std::make_shared<RenderContext>())
        ,m_EditorContext(std::make_shared<EditorContext>(
             m_EditorProxy
            ,m_ProjectManager
            ,m_EditorTextureHolder
            ,m_EditorSetting
            ,m_RenderTexture
            ,m_RenderContext))
        ,m_EditorSetup(std::make_shared<EditorSetup>(m_EditorContext))
        // User Interface
        ,m_EditorDockspace(m_EditorContext)
        ,m_EditorToolbar(m_EditorContext)
        ,m_EditorSetupPopup(m_EditorContext, m_EditorSetup)
        ,m_ResourceSelectionWindow(m_EditorContext)
        ,m_ResourceBrowserWindow(m_EditorContext)
        ,m_SceneExplorerWindow(m_EditorContext)
        //
        ,m_InterfaceFirstDraw(true)
        ,m_SelectedScriptTypeIndex(0)
        ,m_SelectedGameLevelIndex(0)
        ,m_SelectedGameScreenIndex(0)
        ,g_Context(nullptr)
        ,m_BottomDockspaceTabBarSwitch()
        ,m_SelectedChunkNode(StringPool.BLANK)
        ,m_InputSelectedChunkId(-1)
        ,m_InputSelectedObjectLayerId(-1)
        ,m_InputSelectedGameLevelId(-1)
        ,m_InputSelectedGameScreenId(-1)
        ,m_MouseInformation("Mouse Position")
        ,m_ConsoleApplication()
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
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		io.Fonts->Clear();
		io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation-Vector.otf", 15.f);
		ImGui::SFML::UpdateFontTexture();

		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH  = true;
		config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf", 18.0f, &config, icon_ranges);
		ImGui::SFML::UpdateFontTexture();

		io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation.ttf", 17.f);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf", 18.0f, &config, icon_ranges);
		ImGui::SFML::UpdateFontTexture();

		io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation.ttf", 25.f);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf", 18.0f, &config, icon_ranges);
		ImGui::SFML::UpdateFontTexture();

		io.Fonts->AddFontFromFileTTF("resource/editor/font/Sansation.ttf", 14.f);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf", 14.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf", 14.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-brands-400.ttf", 14.0f, &config, icon_ranges);
		ImGui::SFML::UpdateFontTexture();

		g_Context = ax::NodeEditor::CreateEditor();

        m_BottomDockspaceTabBarSwitch.registerTab(
		{
			EditorConstant.WINDOW_RESOURCE,
			EditorConstant.WINDOW_LOGGING,
		});

		m_CameraXAxis.setSize(sf::Vector2f(20.f, -2.f));
		m_CameraXAxis.setFillColor(sf::Color::Red);
		m_CameraXAxis.setPosition(sf::Vector2f(20.f, 20.f));

		m_CameraYAxis.setSize(sf::Vector2f(20.f, -2.f));
		m_CameraYAxis.setFillColor(sf::Color::Green);
		m_CameraYAxis.setPosition(sf::Vector2f(20.f, 20.f));
		m_CameraYAxis.setRotation(90.f);

		m_CanvasXAxis.setSize(sf::Vector2f(100.f, -2.f));
		m_CanvasXAxis.setFillColor(sf::Color::Red);
		m_CanvasXAxis.setOrigin(sf::Vector2f(50.f, -1.f));
		m_CanvasXAxis.setPosition(sf::Vector2f(0.f, 0.f));
		m_CanvasYAxis.setSize(sf::Vector2f(100.f, -2.f));
		m_CanvasYAxis.setOrigin(sf::Vector2f(50.f, -1.f));
		m_CanvasYAxis.setFillColor(sf::Color::Green);
		m_CanvasYAxis.setPosition(sf::Vector2f(0.f, 0.f));
		m_CanvasYAxis.setRotation(90.f);



		//setup game mode information text
		m_GameModeInfo.setFont(m_EditorFontHolder->getDefaultFont());
		m_GameModeInfo.setCharacterSize(18.f);
		m_GameModeInfo.setFillColor(sf::Color::White);

		m_GameBuilderInfo.setFont(m_EditorFontHolder->getDefaultFont());
		m_GameBuilderInfo.setCharacterSize(18.f);
		m_GameBuilderInfo.setFillColor(sf::Color::White);

		//clear workspace input
		clearScriptWizardInput();

		m_ProjectManager->setSetting(m_EditorSetting);

		//register signal handler
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
        auto levelBuilder       = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(worldBuilder && editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::OBJECT && mouseOnCanvas())
            {
                worldBuilder->handleEvent(event);
            }
            else if(worldBuilder && editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::MESH && mouseOnCanvas())
            {
                worldBuilder->getMeshEditor()->handleEvent(event);
            }
        }


		switch(event.type)
		{
			case sf::Event::Closed:
                m_EditorProxy->closeEditor();
				break;

			//Keyboard
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

        auto levelBuilder       = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(editorMode == EditorMode::WORLD_BUILDER && builderMode == BuilderMode::OBJECT && worldBuilder)
            {
                worldBuilder->update(timeStep);
            }
        }
    }

    void EditorUI::render()
    {
        const auto editorMode   = m_EditorContext->getEditorMode();

        EASY_FUNCTION(profiler::colors::Red)

        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);

        m_EditorDockspace.render();

        // Central dockspcace
        m_EditorToolbar.render();
            //viewport
		showSceneWindow();
			//game project
		showGameProjectWindow();
			//game setting
		showGameSettingWindow();
			//visual script
		showVisualScriptWindow();
			//resource manager
        m_ResourceSelectionWindow.render();
			//imgui demo
		ImGui::ShowDemoWindow();

		//left dockspace
			//upper left
		showUtilityWindow();
        /*if(editorMode == EditorMode::SCREEN_BUILDER)
			showGameScreenWindow();*/

			//lower left
		showObjectLayerWindow();
        if(editorMode == EditorMode::WORLD_BUILDER)
			showWorldChunckWindow();

		//right dockspace
        m_SceneExplorerWindow.render();
		showHelpWindow();
        m_ResourceBrowserWindow.render();

		//bottom dockspacer
		showLoggingWindow();
		showConsoleWindow();
        if(m_EditorContext->getAdvancedScene() && editorMode == EditorMode::WORLD_BUILDER)
		{
			showGameLevelWindow();
		}

		//init
		interfaceFirstDraw();

		//background task
		showBackgroundTaskWindow();

        if(m_EditorSetup->initiateSetup())
        {
            m_EditorSetupPopup.render();
        }

		//commit
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

    void EditorUI::updateFrameRate(const float& frameRate, const float& frameTime)
    {
        m_FrameRate = frameRate;
        m_FrameTime = frameTime;
    }

    void EditorUI::setUpdateWindowTitleCallback(std::function<void (const std::string&)> callback)
    {
        m_UpdateWindowTitleCallback = callback;
    }

    void EditorUI::switchBuilderMode()
	{
        const auto editorMode   = m_EditorContext->getEditorMode();
        const auto builderMode  = m_EditorContext->getBuilderMode();

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

    void EditorUI::showSceneWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

			buildRenderContext();

			ImGui::SameLine();

            if(mouseOnCanvas() &&  ImGui::IsWindowFocused())
			{
                sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y), m_RenderTexture->getView());

                std::string canvas_pos_string = "Canvas x = " + toString(m_RenderContext->mousePosition.x) + " y = " + toString(m_RenderContext->mousePosition.y);
                std::string wrold_pos_string = "World x = " + toString(world_pos.x) + " y = "  + toString(world_pos.y);
                std::string camera_pos_string = "Camera x = " + toString(m_EditorCamera->getPosition().x) + " y = "  + toString(m_EditorCamera->getPosition().y);

				m_MouseInformation = canvas_pos_string + " | " + wrold_pos_string + " | " + camera_pos_string;
			}
			float start = (ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize(m_MouseInformation.c_str()).x)/2.f;
			ImGui::SetCursorPosX(start);
			ImGui::Text(m_MouseInformation.c_str());

			prepareRenderTexture();

            auto levelBuilder = m_EditorContext->getLevelBuilder();
            if(levelBuilder)
            {
                levelBuilder->render();
            }

			//Render on Front Screen
			m_RenderTexture->setView(m_RenderTexture->getDefaultView());
				/*if(m_AdvancedScene)
				{
					m_AdvancedScene->renderFrontScreen(m_EditorMode, m_BuilderMode);
				}*/
				renderCamera();
				renderGameModeInfo();
			m_RenderTexture->setView(m_EditorCamera->getView());

			m_CanvasXAxis.setRotation(m_EditorCamera->getView().getRotation());
			m_CanvasYAxis.setRotation(m_EditorCamera->getView().getRotation() + 90.f);

			//m_RenderTexture->draw(m_CanvasXAxis);
			//m_RenderTexture->draw(m_CanvasYAxis);


			ImGui::Image(flipTexture(m_RenderTexture->getTexture()));

			showCanvasMenu();

        ImGui::End();
    }

    void EditorUI::showCanvasMenu()
	{
		if (ImGui::BeginPopupCanvasWindow())
		{
			if (ImGui::BeginMenu("Editor Mode"))
			{
				//ImGui::MenuItem("(choose editor mode)", NULL, false, false);

				if (ImGui::MenuItem("World Builder"))
				{
                    m_EditorContext->setEditorMode(EditorMode::WORLD_BUILDER);
				}

				if (ImGui::MenuItem("Screen Builder"))
				{
                    m_EditorContext->setEditorMode(EditorMode::SCREEN_BUILDER);
				}

				if (ImGui::MenuItem("Object Builder"))
				{
                    m_EditorContext->setEditorMode(EditorMode::OBJECT_BUILDER);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Website"))
			{
				//ImGui::MenuItem("(useful links)", NULL, false, false);

				if (ImGui::MenuItem("Learn"))
				{

				}

				if (ImGui::MenuItem("Forum"))
				{

				}

				if (ImGui::MenuItem("Code Snippet"))
				{

				}

				if (ImGui::MenuItem("Engine API"))
				{

				}

				ImGui::EndMenu();
			}


			ImGui::EndPopup();
		}

	}

    std::string EditorUI::getString(const EditorMode& editorMode)
	{
		switch (editorMode)
		{
			case EditorMode::WORLD_BUILDER:
			{
                const auto builderMode = m_EditorContext->getBuilderMode();

                if(builderMode ==  BuilderMode::OBJECT)
				{
					return "World Builder - Object";
				}
                else if(builderMode ==  BuilderMode::MESH)
				{
					return "World Builder - Mesh";
				}
			}break;
			case EditorMode::SCREEN_BUILDER:	return "Screen Builder";	break;
			case EditorMode::OBJECT_BUILDER:	return "Object Builder";	break;
			case EditorMode::PLAY_GAME:			return "Play Game";			break;
			case EditorMode::RENDER_GAME:		return "Render Game";		break;

			default: return StringPool.BLANK; break;
		}
	}

    void EditorUI::renderGameModeInfo()
	{
        std::string gameMode = getString(m_EditorContext->getEditorMode());
		std::string frameRate = toString(m_FrameRate) + " fps";
		std::string frameTime = toString(m_FrameTime * 1000.f) + " ms";

		std::string info = gameMode + "  |  " + frameRate + "  |  " + frameTime;

		m_GameModeInfo.setString(info);
        m_GameBuilderInfo.setString(m_EditorContext->getOpengedGameLevelName());

		sf::Vector2f position;
		position.x = m_RenderTexture->getSize().x - m_GameModeInfo.getLocalBounds().width - 20.f;
		position.y = m_RenderTexture->getSize().y - m_GameModeInfo.getLocalBounds().height - 20.f;

		m_GameModeInfo.setPosition(position);

		m_GameBuilderInfo.setPosition(sf::Vector2f(20.f, position.y));

		m_RenderTexture->draw(m_GameModeInfo);
		m_RenderTexture->draw(m_GameBuilderInfo);
	}

    void EditorUI::renderCamera()
	{
		m_CameraXAxis.setRotation(m_EditorCamera->getView().getRotation());
		m_CameraYAxis.setRotation(m_EditorCamera->getView().getRotation() + 90.f);

		m_RenderTexture->draw(m_CameraXAxis);
		m_RenderTexture->draw(m_CameraYAxis);
	}



    void EditorUI::buildRenderContext()
	{
		sf::Vector2f window_position    = ImGui::GetWindowPos();
		sf::Vector2f window_size        = ImGui::GetWindowSize();
		sf::Vector2f mouse_position     = ImGui::GetMousePos();
		float title_bar_height          = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
		sf::Vector2f window_padding     = ImGui::GetStyle().WindowPadding;

		window_size.y -= 23.f;

		RenderContext renderContext;
        renderContext.canvasPosition   = sf::Vector2f(window_position.x + window_padding.x, window_position.y + title_bar_height + window_padding.y + 22.f);
        renderContext.canvasSize       = sf::Vector2f(window_size.x - window_padding.x * 2, window_size.y - title_bar_height - window_padding.y * 2);
        renderContext.mousePosition    = sf::Vector2f(mouse_position.x - renderContext.canvasPosition.x, mouse_position.y - renderContext.canvasPosition.y);
		renderContext.focus             = ImGui::IsWindowFocused();

        if(renderContext.canvasSize.x < 100.f)
		{
            renderContext.canvasSize.x = 100.f;
		}

        if(renderContext.canvasSize.y < 100.f)
		{
            renderContext.canvasSize.y = 100.f;
		}

		*m_RenderContext = renderContext;
	}

    void EditorUI::prepareRenderTexture()
	{
        if(m_RenderTexture->getSize().x != m_RenderContext->canvasSize.x ||
           m_RenderTexture->getSize().y != m_RenderContext->canvasSize.y)
		{
            m_RenderTexture->create(m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y);
            m_EditorCamera->updateView(sf::Vector2f(m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y));
		}

		sf::Color clearColor = sf::Color::Black;

        if(m_EditorContext->getEditorMode() == EditorMode::PLAY_GAME)
		{
			/*if(m_AdvancedScene && m_AdvancedScene->getSelectedGameLevel()->levelSetting->getBool("enable_lighting"))
			{
				clearColor = sf::Color::White;
			}*/
		}


		m_RenderTexture->clear(clearColor);
		m_RenderTexture->setView(m_EditorCamera->getView());
	}

    bool EditorUI::mouseOnCanvas()
	{
        sf::Rect<float> canvas(m_RenderContext->canvasPosition.x, m_RenderContext->canvasPosition.y, m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y);

		sf::Vector2i mousePosition = ImGui::GetMousePos();

		return canvas.contains(mousePosition.x, mousePosition.y);
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
		//Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		//Winsow size
		ImVec2 winsow_size              = EditorConstant.WINDOW_SCRIPT_WIZARD_SIZE;

		//Project manager window
		ImGui::SetNextWindowSize(winsow_size);
		//Begin window
		if(ImGui::BeginPopupModal(EditorConstant.WINDOW_SCRIPT_WIZARD.c_str(), nullptr, window_flags))
		{

			if (ImGui::BeginTabBar("##scrip_wizard_tabbar"))
			{
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				float wording_width = 130.f;
				float input_width = ImGui::GetWindowContentRegionWidth() - 130.f;

				if (ImGui::BeginTabItem("CPP Script"))
				{
					ImGui::Text("Class Name");
					ImGui::SameLine(wording_width);
					ImGui::SetNextItemWidth(input_width);
					ImGui::InputText("##class_name", m_InputClassName, sizeof(m_InputClassName));
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::Text("Parent Class");
					ImGui::SameLine(wording_width);
					ImGui::SetNextItemWidth(input_width - 110.f);
					ImGui::InputText("##parent_class", m_InputParentClass, sizeof(m_InputParentClass), ImGuiInputTextFlags_ReadOnly);
					ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 100.f);
					if(ImGui::Button("Select##select_script_parent_class", ImVec2(100.f, 0)))
					{
						ImGui::OpenPopup("Select Script Class");
					}


					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					//ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					/*if(std::string(m_InputParentClass) == "Game Level Script")
					{
						ImGui::Text("Game Level");
						ImGui::SameLine(wording_width);
						ImGui::SetNextItemWidth(input_width);

						//load workpsace
						std::vector<std::string> gameLevelNameTable = m_AdvancedScene->getGameLevelNameTable();

						gameLevelNameTable.insert(gameLevelNameTable.begin(), StringPool.BLANK);

						if(gameLevelNameTable.empty())
						{
							gameLevelNameTable.push_back("There is no Level availabled");
						}

						std::size_t levelCount = gameLevelNameTable.size();
						const char** levelComboTable = new const char* [levelCount];

						string::fillCharTable(levelComboTable, gameLevelNameTable);

						m_SelectedGameLevel = levelComboTable[m_SelectedGameLevelIndex];
						if (ImGui::BeginCombo("##game_level_combo", m_SelectedGameLevel, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
						{
							for (int n = 0; n < levelCount; n++)
							{
								bool is_selected = (m_SelectedGameLevel == levelComboTable[n]);


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
						std::vector<std::string> gameScreenNameTable = m_AdvancedScene->getGameScreenNameTable();

						if(gameScreenNameTable.empty())
						{
							gameScreenNameTable.push_back("There is no Level availabled");
						}

						std::size_t screenCount = gameScreenNameTable.size();
						const char** screenComboTable = new const char* [screenCount];

						string::fillCharTable(screenComboTable, gameScreenNameTable);

						m_SelectedGameScreen = screenComboTable[m_SelectedGameScreenIndex];
						if (ImGui::BeginCombo("##game_level_combo", m_SelectedGameScreen, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
						{
							for (int n = 0; n < screenCount; n++)
							{
								bool is_selected = (m_SelectedGameScreen == screenComboTable[n]);


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
						std::vector<std::string> script_type = {"Game Level Script", "Game Screen Script", "Startup Screen Script",
															   "Physic Script Object", "Simple Script Object", "Action Object",
															   "Action"};


						for(auto type : script_type)
						{
							ImGui::Text(type.c_str());
							ImGui::Separator();
							ImGui::Text("Use a Game Level Script to manage the behavior of a Game Level \n"
										"two line description");
							ImGui::Dummy(ImVec2(0.f, 2.f));
							if(ImGui::Button(type.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50.f)))
							{
								string::fillCharArray(m_InputParentClass, sizeof(m_InputParentClass), type);
								ImGui::CloseCurrentPopup();
							}


							ImGui::Dummy(ImVec2(0.f, 5.f));

						}

						ImGui::EndPopup();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Lua Script"))
				{

					ImGui::EndTabItem();
				}

				ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
				ImGui::SetCursorPosY(winsow_size.y * 0.75f);
				bool onCreate = ImGui::Button("Create##create_script_object", ImVec2(100, 0));

				bool error = false;

				if(onCreate)
				{

					if (error)
					{
						//ImGui::OpenPopup("Error Creating Project");
					}
					else
					{
						Parameter parameter;
						parameter.setString("class_name", std::string(m_InputClassName));
						parameter.setString("script_type", std::string(m_SelectedScriptType));

						if(std::string(m_SelectedScriptType) == "Game Level Script")
						{
                            parameter.setString("level_name", m_EditorContext->getSelectedGameLevelName());
						}
						else if(std::string(m_SelectedScriptType) == "Game Screen Script")
						{
							parameter.setString("screen_name", std::string(m_SelectedGameScreen));
						}

						clearScriptWizardInput();

						createScriptObject(parameter);


						//ImGui::OpenPopup("Script Created");
					}
				}

				ImGui::EndTabBar();
			}

			ImGui::SetCursorPosY(winsow_size.y - 38.f);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 4.0f));
			ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
			if (ImGui::Button("Close##close_script_wizard_window", ImVec2(100, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}


	}

    void EditorUI::createScriptObject(const Parameter& parameter)
	{
        auto gameProject = m_EditorContext->getGameProject();

        if(gameProject)
		{
            gameProject->createScriptObject(parameter);
		}
	}

    void EditorUI::playScene()
	{
        if(m_EditorContext->getAdvancedScene() && m_EditorContext->getEditorMode() != EditorMode::PLAY_GAME)
		{
			try
			{
				//m_AdvancedScene->playScene();
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

			gameLevel->levelSetting->setBool("pause_level", !gameLevel->levelSetting->getBool("pause_level"));
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
        auto gameProject = m_EditorContext->getGameProject();

        if(gameProject)
		{
            BTManager::startTask(&GameProject::compileProject, gameProject.get());
		}
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

            //gameProject->loadLibrary();
		}
	}



    sf::Sprite EditorUI::flipTexture(const sf::Texture& texture)
	{
		sf::Vector2u size = texture.getSize();
		sf::Sprite sprite(texture, sf::IntRect(0, size.y, size.x, -size.y));

		return sprite;
	}

    void EditorUI::showLoggingWindow()
	{
		// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
		std::string log = nero::logging::Logger::getString();
		if(log != StringPool.BLANK)
		{
			m_LoggerApplication.AddLog(log);
			nero::logging::Logger::clearStringStream();
		}

		m_LoggerApplication.Draw(EditorConstant.WINDOW_LOGGING.c_str());
	}

    void EditorUI::showConsoleWindow()
	{
		m_ConsoleApplication.Draw(EditorConstant.WINDOW_CONSOLE.c_str(), nullptr);
	}

    ImVec4 EditorUI::getLoggingColor(logging::LEVEL level)
	{
		switch (level)
		{
			case logging::LOG_INFO:
			{
				return ImVec4();
			}break;

			case logging::LOG_DEBUG:
			{
				return ImVec4();
			}break;

			case logging::LOG_TRACE:
			{
				return ImVec4();
			}break;

			case logging::LOG_WARNING:
			{
				return ImVec4();
			}break;

			case logging::LOG_ERROR:
			{
				return ImVec4();
			}break;

			case logging::LOG_NOTICE:
			{
				return ImVec4();
			}break;

			case logging::LOG_CRITICAL:
			{
				return ImVec4();
			}break;

			case logging::LOG_FATAL:
			{
				return ImVec4();
			}break;

			return ImGui::GetStyle().Colors[ImGuiCol_Text];
		}

	}

    void EditorUI::showUtilityWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_UTILITY.c_str());

			ImVec2 size = ImGui::GetWindowSize();

			ImGui::BeginChild("scene_mode", ImVec2(0.f, 105.f), true);
				ImGui::Text("Choose Scene Mode");
				ImGui::Separator();

                const auto builderMode = m_EditorContext->getBuilderMode();

				int e = 0;
                if(builderMode == BuilderMode::OBJECT) e = 0;
                else if(builderMode == BuilderMode::MESH) e = 1;
				ImGui::RadioButton("Object", &e, 0);
				if(ImGui::IsItemEdited())
				{
                    m_EditorContext->setBuilderMode(BuilderMode::OBJECT);
				}
				ImGui::RadioButton("Mesh", &e, 1);
				if(ImGui::IsItemEdited())
				{
                    m_EditorContext->setBuilderMode(BuilderMode::MESH);
				}
				ImGui::RadioButton("Play", &e, 2);


			ImGui::EndChild();


			ImGui::BeginChild("save_load", ImVec2(0.f, 85.f), true);
				ImGui::Text("Save & Load");
				ImGui::Separator();

				ImGui::Dummy(ImVec2(0.f, 2.f));

				static bool auto_save = false;
				ImGui::Checkbox("Auto save", &auto_save);

				ImVec2 button_size = ImVec2((ImGui::GetWindowContentRegionWidth()-8.f)/2.f, 0.f);

				 if(ImGui::Button("Save", button_size))
				 {
					onSaveProject();
				 }

				ImGui::SameLine();

				 if(ImGui::Button("Load", button_size))
				 {
					onLoadProject();
				 }

			ImGui::EndChild();



			ImGui::BeginChild("access_button", ImVec2(0.f, 90.f), true);
				ImGui::Text("Access Website");
				ImGui::Separator();

				ImGui::Dummy(ImVec2(0.f, 2.f));

				 if(ImGui::Button("Learn", button_size))
				 {
					cmd::launchBrowser("https://nero-games.com/learn/engine-v2");
				 }

				ImGui::SameLine();

				 if(ImGui::Button("Forum", button_size))
				 {
					 cmd::launchBrowser("https://nero-games.com/forum");
				 }


				 if(ImGui::Button("Snippet", button_size))
				 {
					 cmd::launchBrowser("https://nero-games.com/snippet/engine-v2");
				 }

				 ImGui::SameLine();

				 if(ImGui::Button("API", button_size))
				 {
					 cmd::launchBrowser("https://nero-games.com/learn/engine-v2/api");
				 }

			ImGui::EndChild();

		ImGui::End();
	}

    void EditorUI::onSaveProject()
	{
        auto gameProject = m_EditorContext->getGameProject();

        if(!gameProject)
            return;

        switch (m_EditorContext->getEditorMode())
        {
            case EditorMode::WORLD_BUILDER:
            {
                auto levelBuilder = m_EditorContext->getLevelBuilder();

                if(levelBuilder)
                {
                    levelBuilder->saveGameLevel();
                }
            }break;

            case EditorMode::SCREEN_BUILDER:
            {
                //m_GameProject->saveGameScreen();
            }break;
        }
	}

    void EditorUI::onLoadProject()
	{
        auto gameProject = m_EditorContext->getGameProject();

        if(!gameProject)
            return;


        switch (m_EditorContext->getEditorMode())
        {
            case EditorMode::WORLD_BUILDER:
            {
                //m_GameProject->loadGameLevel();
            }break;

            case EditorMode::SCREEN_BUILDER:
            {
                //m_GameProject->loadGameScreen();
            }break;
            case EditorMode::OBJECT_BUILDER:
            case EditorMode::PLAY_GAME:
            case EditorMode::RENDER_GAME:
                break;
        }
    }

    void EditorUI::autoSaveProject()
	{
		/*if(m_GameProject && m_AutoSaveClock.getElapsedTime() > sf::seconds(m_EditorSetting->getSetting("editor").getUInt("auto_save_interval")))
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

			ImVec2 button_size = ImVec2((ImGui::GetWindowContentRegionWidth()-8.f), 0.f);

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
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedGameScreenId, gameScreen->screenId);

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

    void EditorUI::removeGameScreen()
	{

	}



    void EditorUI::showObjectLayerWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_LAYER.c_str());

			ImGui::Text("Manage Object Layers");
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.f, 2.f));

			float width = 90.f;

			ImGui::Dummy(ImVec2(0.f, 2.f));

			ImVec2 button_size = ImVec2(width, 0.f);

			if(ImGui::Button("Add##add_object_layer", button_size))
			{
				addObjectLayer();
			}

			ImGui::SameLine();

			if(ImGui::Button("Remove##remove_object_layer", button_size))
			{
				removeObjectLayer();
			}

			ImGui::Dummy(ImVec2(0.f, 5.f));

			ImGui::BeginChild("##manage_object_layer", ImVec2(), true);

            auto levelBuilder       = m_EditorContext->getLevelBuilder();

            if(levelBuilder)
            {
                auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

                if(worldBuilder)
				{
                    m_InputSelectedObjectLayerId = worldBuilder->getSelectedLayer()->getObjectId();

                    for(const auto& objectLayer : worldBuilder->getLayerTable())
					{
						std::string itemId = "##select_layer" + toString(objectLayer->getObjectId());
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedObjectLayerId, objectLayer->getObjectId());

						if(ImGui::IsItemClicked())
						{
                            worldBuilder->setSelectedLayer(objectLayer);
						}

						ImGui::SameLine();

						itemId = "##layer_visible" + toString(objectLayer->getObjectId());
						ImGui::Checkbox(itemId.c_str(), &objectLayer->getVisibility());

						ImGui::SameLine();


						auto color = getLayerColor(objectLayer->getSecondType());

						ImGui::PushStyleColor(ImGuiCol_FrameBg,			std::get<0>(color));
						ImGui::PushStyleColor(ImGuiCol_TextSelectedBg,	std::get<1>(color));

						char layer_name[100];
						string::fillCharArray(layer_name, sizeof(layer_name), objectLayer->getName());
						ImGui::SetNextItemWidth(118.f);
						itemId = "##layer_name" + toString(objectLayer->getObjectId());
						ImGui::InputText(itemId.c_str(), layer_name, sizeof(layer_name));

						if(ImGui::IsItemEdited())
						{
							objectLayer->setName(std::string(layer_name));
						}

						ImGui::PopStyleColor(2);
					}
                }
            }
			ImGui::EndChild();

		ImGui::End();
	}

    std::tuple<ImVec4, ImVec4> EditorUI::getLayerColor(Object::Type type)
	{
		switch(type)
		{
			case Object::Sprite_Object:
				return {
							ImVec4(0.3f, 0.6f, 0.5f, 0.5f),
							ImVec4(0.3f, 0.6f, 0.5f, 1.f),
						} ;

			default:
				return {
							ImGui::GetStyle().Colors[ImGuiCol_FrameBg],
							ImGui::GetStyle().Colors[ImGuiCol_TextSelectedBg]
						};
		}
	}

    void EditorUI::addObjectLayer()
	{
        auto levelBuilder       = m_EditorContext->getLevelBuilder();

        if(!levelBuilder)
            return;

        levelBuilder->getSelectedChunk()->getWorldBuilder()->addLayer();
	}

    void EditorUI::removeObjectLayer()
	{
        auto levelBuilder       = m_EditorContext->getLevelBuilder();

        if(!levelBuilder)
            return;

        auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

        worldBuilder->deleteLayer(worldBuilder->getSelectedLayer()->getObjectId());
	}

    void EditorUI::showGameLevelWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_LEVEL.c_str());

			ImVec2 button_size = ImVec2(100.f, 0.f);
			if(ImGui::Button("Open##open_game_level", button_size))
			{
                m_EditorProxy->openGameLevel(m_EditorContext->getSelectedGameLevelName());
			}

			ImGui::SameLine();

			if(ImGui::Button("Edit##edit_game_level", button_size))
			{

			}

			ImGui::SameLine();

			if(ImGui::Button("Copy##copy_game_level", button_size))
			{

			}

			ImGui::SameLine();


			if(ImGui::Button("Close##close_game_level", button_size))
			{
				closeGameLevel();
			}


			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 93.f);

			if(ImGui::Button("Delete##delete_game_level", button_size))
			{

			}

			ImGui::Dummy(ImVec2(0.f, 2.f));

			ImGui::BeginChild("##show_game_level", ImVec2(), true);

                auto levelNameTable = m_EditorContext->getAdvancedScene()->getRegisteredLevelTable();
				ImGuiStyle& style = ImGui::GetStyle();
				float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth();

				int level_count		= levelNameTable.size();
				int count			= 0;
				auto printSameLine	= [&count, &level_count, &style, &window_visible_x2]()
				{
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.f;
					if (count++ + 1 < level_count && next_button_x2 < window_visible_x2)
					{
						ImGui::SameLine();
					}
					else
					{
						ImGui::Dummy(ImVec2(0.0f, 4.0f));
					}
				};

				for (const std::string& name :  levelNameTable)
				{
					ImVec2 button_size(200.f, 75.f);

                    pushGameLevelStyle(m_EditorContext->getSelectedGameLevelName() == name,
                                       m_EditorContext->getOpengedGameLevelName() == name);
					if(ImGui::Button(name.c_str(), button_size))
					{
                        m_EditorContext->setSelectedGameLevelName(name);
					}
					popGameLevelStyle();

					printSameLine();
				}

			ImGui::EndChild();

		ImGui::End();
	};


    void EditorUI::closeGameLevel()
	{
        auto advancedScene = m_EditorContext->getAdvancedScene();

        if(!advancedScene)
            return;

        m_EditorContext->setOpenedGameLevelName(StringPool.BLANK);
        advancedScene->closeSelectedLevel();
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
		//Window flags
        /*ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
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
			ImGui::InputText("##new_screen_name", m_NewGameScreenInput.name, sizeof(m_NewGameScreenInput.name));
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

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
					parameter.setString("screen_name", std::string(m_NewGameScreenInput.name));
					//parameter.setString("parent", std::string(m_NewGameScreenInput.parent));
					parameter.setString("template", std::string(m_NewGameScreenInput.prototype));

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

    void EditorUI::removeGameLevel()
	{
        //TODO
	}

    void EditorUI::editGameLevel()
	{
        //TODO
	}

    void EditorUI::showWorldChunckWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_CHUNCK.c_str());

			ImGui::Text("Manage World Chuncks");
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.f, 2.f));

			float width = 90.f;

			ImGui::Dummy(ImVec2(0.f, 2.f));

			ImVec2 button_size = ImVec2(width, 0.f);

			if(ImGui::Button("Add##add_world_chunk", button_size))
			{
				addWorldChunk();
			}

			ImGui::SameLine();

			if(ImGui::Button("Remove##remove_world_chunk", button_size))
			{
                //removeWorldChunk();
			}

			ImGui::Dummy(ImVec2(0.f, 5.f));

			ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##manage_world_chunk", ImVec2(), true);

            auto levelBuilder = m_EditorContext->getLevelBuilder();

                if(levelBuilder)
				{
                    auto& chunkTable = levelBuilder->getChunkTable();

                    auto selectedChunk		= levelBuilder->getSelectedChunk();
					m_InputSelectedChunkId	= selectedChunk ? selectedChunk->getChunkId() : -1;

					for(const auto& worldChunk : chunkTable)
					{
						std::string itemId = "##select_chunk" + toString(worldChunk->getChunkId());
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedChunkId, worldChunk->getChunkId());

						if(ImGui::IsItemClicked())
						{
                            levelBuilder->setSelectedChunk(worldChunk);
						}

						ImGui::SameLine();

						itemId = "##visible_chunk" + toString(worldChunk->getChunkId());
						bool visible = worldChunk->isVisible();
						ImGui::Checkbox(itemId.c_str(), &visible);
						worldChunk->setVisible(visible);

						ImGui::SameLine();

						char chunk_name[100];
						string::fillCharArray(chunk_name, sizeof(chunk_name), worldChunk->getChunkName());
						ImGui::SetNextItemWidth(118.f);
						itemId = "##chunk_name" + toString(worldChunk->getChunkId());
						ImGui::InputText(itemId.c_str(), chunk_name, sizeof(chunk_name));

						if(ImGui::IsItemEdited())
						{
							worldChunk->setChunkName(std::string(chunk_name));
						}
					}
				}

			ImGui::EndChild();

		ImGui::End();
	}

    void EditorUI::addWorldChunk()
	{
        auto levelBuilder = m_EditorContext->getLevelBuilder();

        if(levelBuilder &&
           m_EditorContext->getEditorMode() == EditorMode::WORLD_BUILDER &&
           m_EditorContext->getBuilderMode() == BuilderMode::OBJECT)
		{
            auto worldChunk = levelBuilder->addChunk();
		}
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

			//commit
			m_InterfaceFirstDraw = false;
		}
	}

    void EditorUI::showHelpWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_HELP.c_str());

		ImGui::End();
	}


    void EditorUI::showBackgroundTaskWindow()
	{
        if(!m_EditorContext->getGameProject())
            return;

		// FIXME-VIEWPORT: Select a default viewport
		const float DISTANCE = 10.0f;
		static int corner = 3;
		ImGuiIO& io = ImGui::GetIO();
		if (corner != -1)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 window_pos = ImVec2((corner & 1) ? (viewport->Pos.x + viewport->Size.x - DISTANCE) : (viewport->Pos.x + DISTANCE), (corner & 2) ? (viewport->Pos.y + viewport->Size.y - DISTANCE) : (viewport->Pos.y + DISTANCE));
			ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowViewport(viewport->ID);
		}
		//ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.000f, 1.000f, 1.000f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.911f, 0.499f, 0.146f, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.00f));
		if (ImGui::Begin("##background_task_window", nullptr, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			auto& taskTable = BTManager::getTaskTable();

			for(BackgroundTask::Ptr task : taskTable)
			{
				//BTManager::pauseTask(task->getName(), std::chrono::milliseconds(1));
				if(!task->isCompleted())
				{					
					ImGui::Text(task->printMessage().c_str());
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
	}


    void EditorUI::clearScriptWizardInput()
	{
		string::fillCharArray(m_InputClassName,			sizeof(m_InputClassName),		StringPool.BLANK);
	}

    void EditorUI::registerSignalHandler()
	{
		signal(SIGABRT, handleSignalAbnormalTermination);
        signal(SIGFPE,  handleSignalArithmeticError);
        signal(SIGILL,  handleSignalIllegalInstruction);
        signal(SIGINT,  handleSignalInteractiveAttention);
		signal(SIGSEGV, handleSignalInvalidStorageAccess);
		signal(SIGTERM, handleSignalTerminsationRequest);
	}

    void EditorUI::handleSignalAbnormalTermination(int signal)
	{
        //TODO
	}

    void EditorUI::handleSignalArithmeticError(int signal)
	{
        //TODO
	}

    void EditorUI::handleSignalIllegalInstruction(int signal)
	{
        //TODO
	}

    void EditorUI::handleSignalInteractiveAttention(int signal)
	{
        //TODO
	}

    void EditorUI::handleSignalInvalidStorageAccess(int signal)
	{
        //TODO
	}

    void EditorUI::handleSignalTerminsationRequest(int signal)
	{
        //TODO
	}

    void EditorUI::setupEditorProxy()
    {
        // Open project
        m_EditorProxy->m_OpenProjectCallback = [this](const std::string& projectDirectory)
        {
            //open new project
            if(m_ProjectManager)
            {
                m_ProjectManager->openProject(projectDirectory);
                auto advancedScene		= m_EditorContext->getAdvancedScene();
                advancedScene->setRenderTexture(m_RenderTexture);
                advancedScene->setRenderContext(m_RenderContext);

                //update editor window title
                m_UpdateWindowTitleCallback(m_EditorContext->getGameProject()->getProjectName());
            }
        };

        // Create project
        m_EditorProxy->m_CreateProjectCallback = [this](const Parameter& projectParameter, const unsigned int& projectCount)
        {
            nero_log("on create project in background")
            std::string taskName = EditorConstant.TASK_CREATE_PROJECT + toString(projectCount);
            BTManager::startTask(&ProjectManager::createProject, m_ProjectManager.get(), projectParameter, taskName);
        };

        m_EditorProxy->m_SaveProjectCallback = [this]()
        {
          //TODO
        };

        // Close project
        m_EditorProxy->m_CloseProjectCallback = [this]()
        {
            m_ProjectManager->closeProject();

            //update editor window title
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
            nero_log("Closing the Editor")

            nero_log("-> Saving window settings")
            std::string windowSettingFile   = file::getPath({"setting", "window"}, StringPool.EXT_JSON);
            Setting windowSetting           = m_EditorSetting->getSetting("window");
            sf::Vector2u windowSize         = m_RenderWindow.getSize();
            sf::Vector2i windowPosition     = m_RenderWindow.getPosition();

            if(windowPosition.x < -8) {windowPosition.x = -8;}
            if(windowPosition.y < 0) {windowPosition.y = 0;}

            windowSetting.setUInt("width",  windowSize.x);
            windowSetting.setUInt("height",  windowSize.y);
            windowSetting.setInt("position_x",  windowPosition.x);
            windowSetting.setInt("position_y",  windowPosition.y);

            file::saveFile(windowSettingFile, windowSetting.toJson().dump(3), true);

            if(m_EditorContext->getGameProject())
            {
                nero_log("-> Closing project")
                m_EditorProxy->closeProject();
            }

            nero_log("---")
            nero_log("Editor closed")

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
    }
}
