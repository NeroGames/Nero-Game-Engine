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
                       Setting::Ptr setting):
        // Constructor paramater
         m_RenderWindow(window)
        ,m_EditorCamera(camera)
        ,m_EditorTextureHolder(textureHolder)
        ,m_EditorFontHolder(fontHolder)
        ,m_EditorSoundHolder(soundHolder)
        ,m_EditorSetting(setting)
        // Main paramater
        ,m_ProjectManager(std::make_unique<ProjectManager>())
        ,m_EditorProxy(std::make_shared<EditorProxy>())
        ,m_EditorContext(std::make_shared<EditorContext>(
            m_EditorProxy,
            m_ProjectManager,
            m_EditorTextureHolder,
            m_EditorSetting))
        ,m_EditorSetup(std::make_shared<EditorSetup>(m_EditorContext))
        // User Interface
        ,m_EditorDockspace(m_EditorContext)
        ,m_EditorToolbar(m_EditorContext)
        ,m_EditorSetupPopup(m_EditorContext, m_EditorSetup)
        //
        ,m_InterfaceFirstDraw(true)
        ,m_SelectedScriptTypeIndex(0)
        ,m_SelectedGameLevelIndex(0)
        ,m_SelectedGameScreenIndex(0)
        ,m_AdvancedScene(nullptr)
        ,g_Context(nullptr)
        ,m_BottomDockspaceTabBarSwitch()
        ,m_ResourceBrowserType(ResourceType::None)
        ,m_EditorMode(EditorMode::WORLD_BUILDER)
        ,m_BuilderMode(BuilderMode::OBJECT)
        ,m_SelectedChunkNode(StringPool.BLANK)
        ,m_InputSelectedChunkId(-1)
        ,m_InputSelectedObjectLayerId(-1)
        ,m_InputSelectedGameLevelId(-1)
        ,m_InputSelectedGameScreenId(-1)
        ,m_MouseInformation("Mouse Position")
        ,m_ConsoleApplication()
        ,m_RenderTexture(std::make_shared<sf::RenderTexture>())
        ,m_RenderContext(std::make_shared<RenderContext>())
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

		if(m_EditorMode == EditorMode::WORLD_BUILDER && m_BuilderMode == BuilderMode::OBJECT && m_WorldBuilder && mouseOnCanvas())
		{
			m_WorldBuilder->handleEvent(event);
		}
		else if(m_EditorMode == EditorMode::WORLD_BUILDER && m_BuilderMode == BuilderMode::MESH && m_WorldBuilder && mouseOnCanvas())
		{
			m_WorldBuilder->getMeshEditor()->handleEvent(event);
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

		if(m_EditorMode == EditorMode::WORLD_BUILDER && m_BuilderMode == BuilderMode::OBJECT && m_WorldBuilder)
		{
			m_WorldBuilder->update(timeStep);
		}
    }

    void EditorUI::render()
    {
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
		showResourceWindow();
			//imgui demo
		ImGui::ShowDemoWindow();

		//left dockspace
			//upper left
		showUtilityWindow();
		/*if(m_EditorMode == EditorMode::SCREEN_BUILDER)
			showGameScreenWindow();*/

			//lower left
		showObjectLayerWindow();
		if(m_EditorMode == EditorMode::WORLD_BUILDER)
			showWorldChunckWindow();

		//right dockspace
		showExplorerWindow();
		showHelpWindow();
		showResourceBrowserWindow();

		//bottom dockspacer
		showLoggingWindow();
		showConsoleWindow();
		if(m_AdvancedScene && m_EditorMode == EditorMode::WORLD_BUILDER)
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
		if(m_EditorMode == EditorMode::WORLD_BUILDER)
		{
			if(m_BuilderMode == BuilderMode::OBJECT && !keyboard::CTRL_SHIFT_ALT())
			{
				m_BuilderMode = BuilderMode::MESH;
			}
			else if(m_BuilderMode != BuilderMode::OBJECT && !keyboard::CTRL_SHIFT_ALT())
			{
				m_BuilderMode = BuilderMode::OBJECT;
			}
		}
		else if(m_EditorMode == EditorMode::PLAY_GAME && !keyboard::CTRL_SHIFT_ALT())
		{
			m_EditorMode = EditorMode::WORLD_BUILDER;
			m_BuilderMode = BuilderMode::OBJECT;
		}
	}

    void EditorUI::showSceneWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

			buildRenderContext();

			sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(m_RenderContext->mouse_position.x, m_RenderContext->mouse_position.y), m_RenderTexture->getView());

			std::string canvas_pos_string = "Canvas x = " + toString(m_RenderContext->mouse_position.x) + " y = " + toString(m_RenderContext->mouse_position.y);
			std::string wrold_pos_string = "World x = " + toString(world_pos.x) + " y = "  + toString(world_pos.y);
			std::string camera_pos_string = "Camera x = " + toString(m_EditorCamera->getPosition().x) + " y = "  + toString(m_EditorCamera->getPosition().y);

			ImGui::SameLine();

			if(mouseOnCanvas())
			{
				m_MouseInformation = canvas_pos_string + " | " + wrold_pos_string + " | " + camera_pos_string;
			}
			float start = (ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize(m_MouseInformation.c_str()).x)/2.f;
			ImGui::SetCursorPosX(start);
			ImGui::Text(m_MouseInformation.c_str());

			prepareRenderTexture();

			if(m_WorldBuilder)
			{
				m_WorldBuilder->render();
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
					m_EditorMode = EditorMode::WORLD_BUILDER;
				}

				if (ImGui::MenuItem("Screen Builder"))
				{
				m_EditorMode = EditorMode::SCREEN_BUILDER;
				}

				if (ImGui::MenuItem("Object Builder"))
				{
					m_EditorMode = EditorMode::OBJECT_BUILDER;
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
				if(m_BuilderMode ==  BuilderMode::OBJECT)
				{
					return "World Builder - Object";
				}
				else if(m_BuilderMode ==  BuilderMode::MESH)
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
		std::string gameMode = getString(m_EditorMode);
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
		renderContext.canvas_position   = sf::Vector2f(window_position.x + window_padding.x, window_position.y + title_bar_height + window_padding.y + 22.f);
		renderContext.canvas_size       = sf::Vector2f(window_size.x - window_padding.x * 2, window_size.y - title_bar_height - window_padding.y * 2);
		renderContext.mouse_position    = sf::Vector2f(mouse_position.x - renderContext.canvas_position.x, mouse_position.y - renderContext.canvas_position.y);
		renderContext.focus             = ImGui::IsWindowFocused();

		if(renderContext.canvas_size.x < 100.f)
		{
			renderContext.canvas_size.x = 100.f;
		}

		if(renderContext.canvas_size.y < 100.f)
		{
			renderContext.canvas_size.y = 100.f;
		}

		*m_RenderContext = renderContext;
	}

    void EditorUI::prepareRenderTexture()
	{
		if(m_RenderTexture->getSize().x != m_RenderContext->canvas_size.x ||
		   m_RenderTexture->getSize().y != m_RenderContext->canvas_size.y)
		{
			m_RenderTexture->create(m_RenderContext->canvas_size.x, m_RenderContext->canvas_size.y);
			m_EditorCamera->updateView(sf::Vector2f(m_RenderContext->canvas_size.x, m_RenderContext->canvas_size.y));
		}

		sf::Color clearColor = sf::Color::Black;

		if(m_EditorMode == EditorMode::PLAY_GAME)
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
		sf::Rect<float> canvas(m_RenderContext->canvas_position.x, m_RenderContext->canvas_position.y, m_RenderContext->canvas_size.x, m_RenderContext->canvas_size.y);

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
		if(m_GameProject)
		{
			m_GameProject->createScriptObject(parameter);
		}
	}

    void EditorUI::playScene()
	{
		if(m_AdvancedScene && m_EditorMode != EditorMode::PLAY_GAME)
		{
			try
			{
				//m_AdvancedScene->playScene();
				m_EditorMode = EditorMode::PLAY_GAME;
			}
			catch(std::exception const& e)
			{

			}
		}
	}

    void EditorUI::pauseScene()
	{
		/*if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

			gameLevel->levelSetting->setBool("pause_level", !gameLevel->levelSetting->getBool("pause_level"));
		}*/
	}

    void EditorUI::stepScene()
	{

		/*if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

			gameLevel->levelSetting->setBool("pause_scene", true);
			gameLevel->levelSetting->setBool("single_step", true);
		}*/
	}

    void EditorUI::resetScene()
	{
		/*if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			m_AdvancedScene->playScene();
		}*/
	}

    void EditorUI::renderScene()
	{

	}

    void EditorUI::compileProject()
	{
		if(m_GameProject)
		{
			BTManager::startTask(&GameProject::compileProject, m_GameProject.get());
		}
	}

    void EditorUI::editProject()
	{
		if(m_GameProject)
		{
			m_GameProject->openEditor();

		}
	}

    void EditorUI::reloadProject()
	{

		if(m_GameProject)
		{
			nero_log("reloading project ...");

			//m_GameProject->loadLibrary();

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

    void EditorUI::showResourceWindow()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin("Resource", nullptr, flags);
		bool selected = false;

		int resource_count		= 9;
		int count				= 0;
		ImGuiStyle& style		= ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		auto printSameLine = [&count, &resource_count, &style, &window_visible_x2]()
		{
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.f;
			if (count++ + 1 < resource_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
		};


		selected = m_ResourceBrowserType == ResourceType::Texture;
		pushResourceStyle(selected);
		if(ImGui::Button("Sprite##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Texture;
		}
		popResourceStyle(selected);


		printSameLine();

		selected = m_ResourceBrowserType == ResourceType::Animation;
		pushResourceStyle(selected);
		if(ImGui::Button("Animation##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Animation;
		}
		popResourceStyle(selected);

		printSameLine();


		selected = m_ResourceBrowserType == ResourceType::Mesh;
		pushResourceStyle(selected);
		if(ImGui::Button("Mesh##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Mesh;
		}
		popResourceStyle(selected);


		printSameLine();

		/*if(ImGui::Button("Shape##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Shape;
		}

		printSameLine();

		if(ImGui::Button("Particle##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Particle;
		}

		printSameLine();*/

		selected = m_ResourceBrowserType == ResourceType::Font;
		pushResourceStyle(selected);
		if(ImGui::Button("Font##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Font;
		}
		popResourceStyle(selected);


		printSameLine();

		/*if(ImGui::Button("Composite##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Composite;
		}

		printSameLine();*/

		if(ImGui::Button("Sound##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Sound;
		}

		printSameLine();

		if(ImGui::Button("Music##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Music;
		}

		printSameLine();

		/*if(ImGui::Button("Script##open_script_object_resource", ImVec2(100.f, 100.f)))
		{

		}

		printSameLine();*/

		if(ImGui::Button("Light##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Lightmap;
		}

		printSameLine();

		if(ImGui::Button("Spawner##spawn_object", ImVec2(100.f, 100.f)))
		{

		}

		printSameLine();

		if(ImGui::Button("Factory##open_factory_resource", ImVec2(100.f, 100.f)))
		{

		}

		printSameLine();

		if(ImGui::Button("CPP Script##open_script_resource", ImVec2(100.f, 100.f)))
		{

		}

		printSameLine();

		if(ImGui::Button("Empty Object##add_empty_object", ImVec2(100.f, 100.f)))
		{

		}

		ImGui::End();
	}

    void EditorUI::showResourceBrowserWindow()
	{
        if(!m_ResourceManager || m_ResourceBrowserType == ResourceType::None)
            return;

        ImGui::Begin("Resource Browser", nullptr, ImGuiWindowFlags());

            if(m_GameProject &&
               (m_ResourceBrowserType == ResourceType::Texture	|| m_ResourceBrowserType == ResourceType::Animation ||
               m_ResourceBrowserType == ResourceType::Sound		|| m_ResourceBrowserType == ResourceType::Music		||
               m_ResourceBrowserType == ResourceType::Font		|| m_ResourceBrowserType == ResourceType::Particle	||
               m_ResourceBrowserType == ResourceType::Lightmap))
            {


                if(ImGui::Button("Import File##import_resource", ImVec2(100.f, 0.f)))
                {
                    nfdpathset_t pathSet;
                    nfdresult_t result = NFD_OpenDialogMultiple( nullptr, nullptr, &pathSet);

                    if ( result == NFD_OKAY )
                    {
                        std::vector<std::string> fileTable;

                        size_t i;
                        for ( i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i )
                        {
                            nfdchar_t *path = NFD_PathSet_GetPath(&pathSet, i);

                            fileTable.push_back(toString(path));
                        }

                        const std::vector<std::string> loadedFileTable = m_ResourceManager->loadFile(m_ResourceBrowserType, fileTable);

                        if(!loadedFileTable.empty())
                        {
                            saveResourceFile(m_ResourceBrowserType, loadedFileTable);
                        }

                        NFD_PathSet_Free(&pathSet);
                    }
                    else if ( result == NFD_CANCEL ) {
                        //puts("User pressed cancel.");
                    }
                    else {
                        //printf("Error: %s\n", NFD_GetError() );
                    }

                }
            }

            ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 100.f);


            if(ImGui::Button("Close##close_sprite_resource", ImVec2(100.f, 0.f)))
            {
                m_ResourceBrowserType = ResourceType::None;
            }

            ImGui::Separator();

            ImGui::BeginChild("browser");

                switch (m_ResourceBrowserType)
                {
                    case ResourceType::Texture:
                    {
                        showSpriteResource();
                    }break;

                    case ResourceType::Animation:
                    {
                        showAnimationResource();
                    }break;

                    case ResourceType::Mesh:
                    {
                        showMeshResource();
                    }break;

                    case ResourceType::Font:
                    {
                        showFontResource();
                    }break;

                    case ResourceType::Lightmap:
                    {
                        showLightmapResource();
                    }break;

                }

            ImGui::EndChild();

        ImGui::End();
	}

    void EditorUI::saveResourceFile(ResourceType type, const std::vector<std::string> loadedFileTable)
	{
		switch (m_ResourceBrowserType)
		{
			case ResourceType::Texture:
			{
				//copy texture file and json helper
				for(std::string file : loadedFileTable)
				{
                    file::copyFile(file, file::getPath({m_LevelBuilder->getResourceFoler(), "texture", file::getFileName(file, true)}));

					std::string jsonHelper  = file::replaceExtension(file, "json");
					if(file::fileExist(jsonHelper))
					{
                        file::copyFile(file, file::getPath({m_LevelBuilder->getResourceFoler(), "texture", file::getFileName(jsonHelper, true)}));
					}
				}

			}break;

			case ResourceType::Animation:
			{
				//copy texture file and json helper
				for(std::string file : loadedFileTable)
				{
					file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "animation", file::getFileName(file, true)}));

					std::string jsonHelper  = file::replaceExtension(file, "json");
					if(file::fileExist(jsonHelper))
					{
						file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "animation", file::getFileName(jsonHelper, true)}));
					}
				}

			}break;

			case ResourceType::Font:
			{
				for(std::string file : loadedFileTable)
				{
					file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "font", file::getFileName(file, true)}));
				}

			}break;

			case ResourceType::Sound:
			{
				for(std::string file : loadedFileTable)
				{
					file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "sound", file::getFileName(file, true)}));
				}

			}break;

			case ResourceType::Music:
			{
				for(std::string file : loadedFileTable)
				{
					file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "music", file::getFileName(file, true)}));
				}

			}break;

			case ResourceType::Lightmap:
			{
				for(std::string file : loadedFileTable)
				{
					file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "lightmap", file::getFileName(file, true)}));
				}

			}break;
		}
	}

    void EditorUI::showMeshResource()
	{
		int resource_count		= 3;
		int count				= 0;
		ImGuiStyle& style		= ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		auto printSameLine = [&count, &resource_count, &style, &window_visible_x2]()
		{
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.f;
			if (count++ + 1 < resource_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
		};

		if(ImGui::Button("Polygon", ImVec2(100.f, 100.f)))
		{
			if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
				m_WorldBuilder->addObject(Object::Mesh_Object, "Polygon", getAddObjectPosition());
		}

		printSameLine();

		if(ImGui::Button("Circle", ImVec2(100.f, 100.f)))
		{
			if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
				m_WorldBuilder->addObject(Object::Mesh_Object, "Circle", getAddObjectPosition());
		}

		printSameLine();

		if(ImGui::Button("Line", ImVec2(100.f, 100.f)))
		{
			if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
				m_WorldBuilder->addObject(Object::Mesh_Object, "Line", getAddObjectPosition());
		}
	}

    sf::Vector2f EditorUI::getAddObjectPosition()
	{
		//sf::Vector2f screen_pos    = sf::Vector2f(mouse.x - m_RenderContext->canvas_position.x, mouse.y - m_RenderContext->canvas_position.y);
		//sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(screen_pos.x, screen_pos.y), m_RenderTexture->getView());

		sf::Vector2f screen_pos     = sf::Vector2f(m_RenderContext->canvas_size.x / 2.f, 150.f);
		sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(screen_pos.x, screen_pos.y), m_RenderTexture->getView());

		return world_pos;
	}

    void EditorUI::showSpriteResource()
	{
		auto& spriteTable = m_ResourceManager->getTextureHolder()->getSpriteTable();
		int sprite_count = spriteTable.size();
		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		for (int n = 0; n < sprite_count; n++)
		{
			sf::Vector2u boo = m_ResourceManager->getTextureHolder()->getSpriteTexture(spriteTable[n]).getSize();
			sf::Vector2u zoo = boo;
			if(n < sprite_count-1)
			{
				zoo = m_ResourceManager->getTextureHolder()->getSpriteTexture(spriteTable[n+1]).getSize();
			}



			sf::Vector2f sprite_size(boo.x, boo.y);
			sprite_size = formatSize(sprite_size, 250);

			sf::Vector2f next_sprite_size(zoo.x, zoo.y);
			next_sprite_size = formatSize(next_sprite_size, 250);


			ImVec2 button_size(sprite_size.x, sprite_size.y);


			if(ImGui::ImageButton(m_ResourceManager->getTextureHolder()->getSpriteTexture(spriteTable[n]), button_size))
			{
				if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
					m_WorldBuilder->addObject(Object::Sprite_Object, spriteTable[n], getAddObjectPosition());
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Delete"))
				{
					ImGui::CloseCurrentPopup();
				}

				/*if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();*/
				ImGui::EndPopup();
			}

			if(ImGui::IsItemHovered())
			{
				//nero_log("hover animation button");
			}

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + next_sprite_size.x;
			if (n + 1 < sprite_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
		}
	}

    void EditorUI::showLightmapResource()
	{
		auto& spriteTable = m_ResourceManager->getLightmapHolder()->getSpriteTable();
		int sprite_count = spriteTable.size();
		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		for (int n = 0; n < sprite_count; n++)
		{
			sf::Vector2u boo = m_ResourceManager->getLightmapHolder()->getSpriteTexture(spriteTable[n]).getSize();
			sf::Vector2u zoo = boo;
			if(n < sprite_count-1)
			{
				zoo = m_ResourceManager->getLightmapHolder()->getSpriteTexture(spriteTable[n+1]).getSize();
			}



			sf::Vector2f sprite_size(boo.x, boo.y);
			sprite_size = formatSize(sprite_size, 250);

			sf::Vector2f next_sprite_size(zoo.x, zoo.y);
			next_sprite_size = formatSize(next_sprite_size, 250);


			ImVec2 button_size(sprite_size.x, sprite_size.y);


			if(ImGui::ImageButton(m_ResourceManager->getLightmapHolder()->getSpriteTexture(spriteTable[n]), button_size))
			{
				if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				{
					//m_AdvancedScene->addObject(Object::Light_Object, spriteTable[n], getAddObjectPosition(), m_EditorMode);
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Delete"))
				{
					ImGui::CloseCurrentPopup();
				}

				/*if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();*/
				ImGui::EndPopup();
			}

			if(ImGui::IsItemHovered())
			{
				//nero_log("hover animation button");
			}

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + next_sprite_size.x;
			if (n + 1 < sprite_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
		}
	}

    void EditorUI::showAnimationResource()
	{
		auto& animationTable = m_ResourceManager->getAnimationHolder()->getAnimationTable();
		int animationCount = animationTable.size();

		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		for (int n = 0; n < animationCount; n++)
		{
			sf::Texture& texture = m_ResourceManager->getAnimationHolder()->getTexture(animationTable[n]);
			sf::IntRect bound = m_ResourceManager->getAnimationHolder()->getAnimationBound(animationTable[n]);

			sf::Vector2u boo = sf::Vector2u(bound.width, bound.height);
			sf::Vector2u zoo = boo;
			if(n < animationCount-1)
			{
				sf::IntRect bound2 = m_ResourceManager->getAnimationHolder()->getAnimationBound(animationTable[n+1]);

				zoo = sf::Vector2u(bound2.width, bound2.height);
			}

			sf::Vector2f sprite_size(boo.x, boo.y);
			sprite_size = formatSize(sprite_size, 250);

			sf::Vector2f next_sprite_size(zoo.x, zoo.y);
			next_sprite_size = formatSize(next_sprite_size, 250);


			ImVec2 button_size(sprite_size.x, sprite_size.y);

			sf::Sprite sprite(texture, bound);
			sprite.scale(2.f, 2.f);

			if(ImGui::ImageButton(sprite, button_size))
			{
				/*if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
					m_AdvancedScene->addObject(Object::Animation_Object, animationTable[n], getAddObjectPosition(), m_EditorMode);*/
			}

			if(ImGui::IsItemHovered())
			{
				//nero_log("hover animation button");
			}


			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + next_sprite_size.x;
			if (n + 1 < animationCount && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
		}
	}

    void EditorUI::showFontResource()
	{
		auto& fontTable = m_ResourceManager->getFontHolder()->getFontTable();
		int count = fontTable.size();

		for (int i = 0; i < count; i++)
		{
			if(ImGui::ImageButton(getFontTexture(fontTable.at(i))))
			{
				if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				{
					//m_AdvancedScene->addObject(Object::Animation_Object, animationTable[n], getAddObjectPosition(), m_EditorMode);
				}
			}
		}

	}

    sf::Texture& EditorUI::getFontTexture(const std::string& fontName)
	{
		std::string file_name = file::getPath({"resource/editor/texture", fontName}, StringPool.EXT_PNG);

		if(!file::fileExist(file_name))
		{
			sf::RenderTexture renderTexture;
			renderTexture.create(300.f, 100.f);
			renderTexture.clear(sf::Color::White);
			sf::Text text;
			text.setFont(m_ResourceManager->getFontHolder()->getFont(fontName));
			text.setString(fontName);
			text.setOrigin(text.getGlobalBounds().width/2.f, text.getGlobalBounds().height/2.f);
			text.setFillColor(sf::Color::Black);
			text.setPosition(150.f, 50.f);
			text.setScale(1.f, -1.f);
			renderTexture.draw(text);

			renderTexture.getTexture().copyToImage().saveToFile(file_name);

			m_EditorTextureHolder->loadFile(file_name);
		}

		return m_EditorTextureHolder->getTexture(fontName);
	}

    void EditorUI::showUtilityWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_UTILITY.c_str());

			ImVec2 size = ImGui::GetWindowSize();

			ImGui::BeginChild("scene_mode", ImVec2(0.f, 105.f), true);
				ImGui::Text("Choose Scene Mode");
				ImGui::Separator();

				int e = 0;
				if(m_BuilderMode == BuilderMode::OBJECT) e = 0;
				else if(m_BuilderMode == BuilderMode::MESH) e = 1;
				ImGui::RadioButton("Object", &e, 0);
				if(ImGui::IsItemEdited())
				{
					m_BuilderMode = BuilderMode::OBJECT;
				}
				ImGui::RadioButton("Mesh", &e, 1);
				if(ImGui::IsItemEdited())
				{
					m_BuilderMode = BuilderMode::MESH;
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
		if(m_GameProject)
		{
			switch (m_EditorMode)
			{
				case EditorMode::WORLD_BUILDER:
				{
                    if(m_LevelBuilder)
                        m_LevelBuilder->saveGameLevel();
				}break;

				case EditorMode::SCREEN_BUILDER:
				{
					//m_GameProject->saveGameScreen();
				}break;
			}
		}
	}

    void EditorUI::onLoadProject()
	{
		if(m_GameProject)
		{
			switch (m_EditorMode)
			{
				case EditorMode::WORLD_BUILDER:
				{
					//m_GameProject->loadGameLevel();
				}break;

				case EditorMode::SCREEN_BUILDER:
				{
					//m_GameProject->loadGameScreen();
				}break;
			}
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

				if(m_WorldBuilder)
				{
					m_InputSelectedObjectLayerId = m_WorldBuilder->getSelectedLayer()->getObjectId();

					for(const auto& objectLayer : m_WorldBuilder->getLayerTable())
					{
						std::string itemId = "##select_layer" + toString(objectLayer->getObjectId());
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedObjectLayerId, objectLayer->getObjectId());

						if(ImGui::IsItemClicked())
						{
							m_WorldBuilder->setSelectedLayer(objectLayer);
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
		if(m_WorldBuilder)
		{
			m_WorldBuilder->addLayer();
		}
	}

    void EditorUI::removeObjectLayer()
	{
		if(m_AdvancedScene)
		{
			m_WorldBuilder->deleteLayer(m_WorldBuilder->getSelectedLayer()->getObjectId());
		}
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

				auto levelNameTable = m_AdvancedScene->getRegisteredLevelTable();
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
        m_EditorContext->setOpenedGameLevelName(StringPool.BLANK);
        m_LevelBuilder	= nullptr;
		m_ResourceManager	= nullptr;
		m_AdvancedScene->closeSelectedLevel();
	}

    void EditorUI::createGameScreen(const Parameter& parameter)
	{
		if(m_AdvancedScene)
		{
			m_AdvancedScene->createScreen(parameter);
		}
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
		if(m_AdvancedScene)
		{

		}
	}

    void EditorUI::editGameLevel()
	{
		if(m_AdvancedScene)
		{

		}
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
				removeWorldChunk();
			}

			ImGui::Dummy(ImVec2(0.f, 5.f));

			ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##manage_world_chunk", ImVec2(), true);

                if(m_LevelBuilder)
				{
                    auto& chunkTable = m_LevelBuilder->getChunkTable();

                    auto selectedChunk		= m_LevelBuilder->getSelectedChunk();
					m_InputSelectedChunkId	= selectedChunk ? selectedChunk->getChunkId() : -1;

					for(const auto& worldChunk : chunkTable)
					{
						std::string itemId = "##select_chunk" + toString(worldChunk->getChunkId());
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedChunkId, worldChunk->getChunkId());

						if(ImGui::IsItemClicked())
						{
                            m_LevelBuilder->setSelectedChunk(worldChunk);
							m_WorldBuilder = worldChunk->getWorldBuilder();
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
        if(m_LevelBuilder && m_EditorMode == EditorMode::WORLD_BUILDER && m_BuilderMode == BuilderMode::OBJECT)
		{
            auto worldChunk = m_LevelBuilder->addChunk();
			m_WorldBuilder  = worldChunk->getWorldBuilder();
		}
	}

    void EditorUI::removeWorldChunk()
	{
		if(m_AdvancedScene)
		{
			//m_AdvancedScene->removeWorldChunk();
		}
	}

    void EditorUI::showToggleButton(bool toggle, const std::string& label, std::function<void()> callback)
	{
		if(toggle)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
			ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
			if(ImGui::Button(label.c_str()))
			{
				callback();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}
		else
		{
			if(ImGui::Button(label.c_str()))
			{
				callback();
			}
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


    void EditorUI::showExplorerWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_EXPLORER.c_str());

		if (ImGui::CollapsingHeader("Scene", m_AdvancedScene ? ImGuiTreeNodeFlags_DefaultOpen :ImGuiTreeNodeFlags_None))
		{
            if(m_LevelBuilder)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				float game_world_window_height = viewport->Size.y * 0.25f;
				viewport = nullptr;

				ImGui::BeginChild("game_world", ImVec2(0.f, game_world_window_height), true);
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()*1.5);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);

				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                if(ImGui::TreeNode(std::string(ICON_FA_FOLDER_OPEN " " + m_LevelBuilder->getLevelName()).c_str()))
				{
					int			chunk_node_clicked		= -1;
                    static int	chunk_selection_mask	= (1 << m_LevelBuilder->getChunkTable().size());
                    int			selectedWorldChunkId	= m_LevelBuilder->getSelectedChunk()->getChunkId();

					int loop_chunk = 0;
                    for(const auto& worldChunk : m_LevelBuilder->getChunkTable())
					{
						ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

						if(chunk_selection_mask & (1 << loop_chunk))
						{
							node_flags |= ImGuiTreeNodeFlags_Selected;
						}

						ImGui::SetNextItemOpen(true, ImGuiCond_Once);
						bool chunk_node_open = ImGui::TreeNodeEx((void*)(intptr_t)loop_chunk, node_flags, std::string(ICON_FA_FOLDER " " + worldChunk->getChunkName()).c_str(), loop_chunk);

						if(worldChunk->getChunkId() == selectedWorldChunkId)
						{
							chunk_node_clicked = loop_chunk;
						}
						if (ImGui::IsItemClicked())
						{
							chunk_node_clicked = loop_chunk;
                            m_LevelBuilder->setSelectedChunk(worldChunk);
							m_WorldBuilder = worldChunk->getWorldBuilder();
							selectedWorldChunkId = worldChunk->getChunkId();
						}

						if (chunk_node_open)
						{
							//display chunk layer here
							int			layer_node_clicked		= -1;
							int			selectedObjectLayerId	= m_WorldBuilder->getSelectedLayer()->getObjectId();

							int loop_layer = 0;
							for(const auto& objectLayer : worldChunk->getWorldBuilder()->getLayerTable())
							{
								ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

								if (objectLayer->getObjectId() == selectedObjectLayerId)
								{
									node_flags |= ImGuiTreeNodeFlags_Selected;
								}

								ImGui::SetNextItemOpen(true, ImGuiCond_Once);
								bool layer_node_open = ImGui::TreeNodeEx((void*)(intptr_t)loop_layer, node_flags, std::string(ICON_FA_FILE " " + objectLayer->getName()).c_str(), loop_layer);

								if(objectLayer->getObjectId() == selectedObjectLayerId && worldChunk->getChunkId() == selectedWorldChunkId)
								{
									layer_node_clicked = loop_layer;
									chunk_node_clicked = loop_chunk;
								}
								else if(ImGui::IsItemClicked())
								{
									layer_node_clicked = loop_layer;
									chunk_node_clicked = loop_chunk;
                                    m_LevelBuilder->setSelectedChunk(worldChunk);
									m_WorldBuilder = worldChunk->getWorldBuilder();
									worldChunk->getWorldBuilder()->setSelectedLayer(objectLayer);
									selectedWorldChunkId = worldChunk->getChunkId();
									selectedObjectLayerId = objectLayer->getObjectId();
								}

								if (layer_node_open)
								{
									int			object_node_clicked		= -1;
									int			selectedGameObjectId	= -1;

									if(worldChunk->getWorldBuilder()->getSelectedObject())
									{
										selectedGameObjectId = worldChunk->getWorldBuilder()->getSelectedObject()->getObjectId();
									}

									int loop_object = 0;
									for(const auto& gameObject : *objectLayer->getAllChild())
									{
										ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
										node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

										if(gameObject->getObjectId() == selectedGameObjectId)
										{
											node_flags |= ImGuiTreeNodeFlags_Selected;
										}

										std::string object_name = std::string(gameObject->getName());

										ImGui::TreeNodeEx((void*)(intptr_t)loop_object, node_flags, std::string(ICON_FA_CUBE " " + object_name).c_str(), loop_object);

										if(gameObject->getObjectId() == selectedGameObjectId && objectLayer->getObjectId() == selectedObjectLayerId && worldChunk->getChunkId() == selectedWorldChunkId)
										{
											object_node_clicked = loop_object;
											layer_node_clicked	= loop_layer;
											chunk_node_clicked	= loop_chunk;
										}
										else if (ImGui::IsItemClicked())
										{
											object_node_clicked = loop_object;
											layer_node_clicked	= loop_layer;
											chunk_node_clicked	= loop_chunk;

                                            m_LevelBuilder->setSelectedChunk(worldChunk);
											m_WorldBuilder = worldChunk->getWorldBuilder();
											m_WorldBuilder->setSelectedLayer(objectLayer);
											m_WorldBuilder->setSelectedObject(gameObject);

											selectedWorldChunkId	= worldChunk->getChunkId();
											selectedObjectLayerId	= objectLayer->getObjectId();
											selectedGameObjectId	= gameObject->getObjectId();
										}

										loop_object++;
									}

									ImGui::TreePop();
								}

								loop_layer++;
							}

							ImGui::TreePop();
						}

						loop_chunk++;
					}

					if (chunk_node_clicked != -1)
					{
						chunk_selection_mask = (1 << chunk_node_clicked);

						//if (ImGui::GetIO().KeyCtrl)
						//{
							//chunk_selection_mask ^= (1 << chunk_node_clicked);
						//}
						//else
						//{
							//chunk_selection_mask = (1 << chunk_node_clicked);
						//}
					}

					ImGui::TreePop();
				}
				ImGui::PopFont();

				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
		}

		if (ImGui::CollapsingHeader("Game Level"))
		{
			if(m_AdvancedScene)
			{
				/*ImGuiViewport* viewport = ImGui::GetMainViewport();
				float window_height = viewport->Size.y * 0.25f;
				viewport = nullptr;

				ImGui::BeginChild("game_level", ImVec2(0.f, window_height), true);

					auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

					ImGui::Text("Lighting");
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.f, 5.f));
					static bool enable_lighting = gameLevel->levelSetting->getBool("enable_lighting");
					ImGui::Checkbox("Enable Lighting##enable_lighting", &enable_lighting);
					gameLevel->levelSetting->setBool("enable_lighting", enable_lighting);

					ImGui::ColorEdit4("Ambient Light", &ambient_light.x, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop);

					ImGui::Dummy(ImVec2(0.f, 5.f));

					ImGui::Text("Draw");
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.f, 5.f));

					static bool draw_axis = gameLevel->levelSetting->getBool("draw_axis");
					ImGui::Checkbox("Draw Axis##draw_axis", &draw_axis);
					gameLevel->levelSetting->setBool("draw_axis", draw_axis);

					static bool draw_grid = gameLevel->levelSetting->getBool("draw_grid");
					ImGui::Checkbox("Draw Grid##draw_grid", &draw_grid);
					gameLevel->levelSetting->setBool("draw_grid", draw_grid);

					static bool draw_shape = gameLevel->levelSetting->getBool("draw_shape");
					ImGui::Checkbox("Draw Shape##draw_shape", &draw_shape);
					gameLevel->levelSetting->setBool("draw_shape", draw_shape);

					static bool draw_joint = gameLevel->levelSetting->getBool("draw_joint");
					ImGui::Checkbox("Draw Joint##draw_joint", &draw_joint);
					gameLevel->levelSetting->setBool("draw_joint", draw_joint);

					static bool draw_aabb = gameLevel->levelSetting->getBool("draw_aabb");
					ImGui::Checkbox("Draw AABB##draw_aabb", &draw_aabb);
					gameLevel->levelSetting->setBool("draw_aabb", draw_aabb);

					static bool draw_contact_point = gameLevel->levelSetting->getBool("draw_contact_point");
					ImGui::Checkbox("Draw Contact Point##draw_contact_point", &draw_contact_point);
					gameLevel->levelSetting->setBool("draw_contact_point", draw_contact_point);

					static bool draw_contact_normal = gameLevel->levelSetting->getBool("draw_contact_normal");
					ImGui::Checkbox("Draw Contact Normal##draw_contact_normal", &draw_contact_normal);
					gameLevel->levelSetting->setBool("draw_contact_normal", draw_contact_normal);

					static bool draw_contact_impulse = gameLevel->levelSetting->getBool("draw_contact_impulse");
					ImGui::Checkbox("Draw Contact Impulse##draw_contact_impulse", &draw_contact_impulse);
					gameLevel->levelSetting->setBool("draw_contact_impulse", draw_contact_impulse);

					static bool draw_friction_impulse = gameLevel->levelSetting->getBool("draw_friction_impulse");
					ImGui::Checkbox("Draw Friction Impulse##draw_friction_impulse", &draw_friction_impulse);
					gameLevel->levelSetting->setBool("draw_friction_impulse", draw_friction_impulse);

					static bool draw_center_of_mass = gameLevel->levelSetting->getBool("draw_center_of_mass");
					ImGui::Checkbox("Draw Centroid##draw_center_of_mass", &draw_center_of_mass);
					gameLevel->levelSetting->setBool("draw_center_of_mass", draw_center_of_mass);

					static bool draw_statistic = gameLevel->levelSetting->getBool("draw_statistic");
					ImGui::Checkbox("Draw Statistic##draw_statistic", &draw_statistic);
					gameLevel->levelSetting->setBool("draw_statistic", draw_statistic);

					static bool draw_profile = gameLevel->levelSetting->getBool("draw_profile");
					ImGui::Checkbox("Draw Profile##draw_profile", &draw_profile);
					gameLevel->levelSetting->setBool("draw_profile", draw_profile);

					//
					//levelSetting->setBool("enable_warm_starting", true);
					//levelSetting->setBool("enable_continous", true);
					//levelSetting->setBool("enable_sub_stepping", false);
					//levelSetting->setBool("enable_sleep", true);
					//
					//levelSetting->setBool("pause_level", false);
					//levelSetting->setBool("single_step", false);

				ImGui::EndChild();*/
			}


		}

		if (ImGui::CollapsingHeader("World Chunk"))
		{

		}

		if (ImGui::CollapsingHeader("Object Layer"))
		{

		}



		if(m_WorldBuilder)
		{
			Object::Ptr selectedObject = m_WorldBuilder->getSelectedObject();

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.000f, 0.000f, 0.000f, 0.675f));

			if (ImGui::CollapsingHeader("Game Object", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				float height = viewport->Size.y * 0.50f;
				viewport = nullptr;
				ImGui::BeginChild("game_object", ImVec2(), true);

				ImGui::SetCursorPosX(10.f);
				if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::BeginChild("object_general", ImVec2(0.f, 110.f), true);
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					float wording_width = 70.f;
					float input_width = ImGui::GetWindowContentRegionWidth() - 70.f;

					ImGui::Text("Name");
					ImGui::SameLine(wording_width);
					ImGui::SetNextItemWidth(input_width);
					char object_name[100];
					string::fillCharArray(object_name, sizeof(object_name), selectedObject ? selectedObject->getName() : "");
					std::string input_id = selectedObject ? std::string("##object_name") + toString(selectedObject->getObjectId()) : std::string("##object_name");
					ImGui::InputText(input_id.c_str(), object_name, sizeof(object_name));
					if(selectedObject && ImGui::IsItemEdited())
					{
						selectedObject->setName(std::string(object_name));
					}
					ImGui::Dummy(ImVec2(0.0f, 1.0f));



					ImGui::Text("Type");
					ImGui::SameLine(wording_width);
					ImGui::SetNextItemWidth(input_width);
					char object_type[100];
					string::fillCharArray(object_type, sizeof(object_type), selectedObject ? selectedObject->getTypeString() : "");
					ImGui::InputText("##object_type", object_type, sizeof(object_type), ImGuiInputTextFlags_ReadOnly);
					ImGui::Dummy(ImVec2(0.0f, 1.0f));

					ImGui::Text("Category");
					ImGui::SameLine(wording_width);
					ImGui::SetNextItemWidth(input_width);
					if(ImGui::BeginCombo("##object_category", {}, ImGuiComboFlags()))
					{
						ImGui::EndCombo();
					}
					ImGui::EndChild();
				}

				ImGui::SetCursorPosX(10.f);
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::BeginChild("object_transfrom", ImVec2(0.f, 150.f), true);
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					float wording_width = 70.f;
					float input_width = ImGui::GetWindowContentRegionWidth() - 70.f;

					ImGui::Text("Position");
					ImGui::SameLine(wording_width);
					ImGui::Text(" x ");
					ImGui::SameLine(wording_width + 30.f);
					ImGui::SetNextItemWidth(input_width - 30.f);
					float positionx = selectedObject ? selectedObject->getPosition().x : 0.00f;
					ImGui::InputFloat("##position_x", &positionx, 1.f, 1.0f, "%.3f");
					bool posxchanged = ImGui::IsItemEdited();
					ImGui::Text("");
					ImGui::SameLine(wording_width);
					ImGui::Text(" y ");
					ImGui::SameLine(wording_width + 30.f);
					ImGui::SetNextItemWidth(input_width - 30.f);
					float positiony = selectedObject ? selectedObject->getPosition().y : 0.00f;
					ImGui::InputFloat("##position_y", &positiony, 1.f, 1.0f, "%.3f");
					ImGui::Dummy(ImVec2(0.0f, 1.0f));
					bool posychanged = ImGui::IsItemEdited();
					if(selectedObject && (posxchanged || posychanged))
					{
						selectedObject->setPosition(positionx, positiony);
					}

					ImGui::Text("Scale");
					ImGui::SameLine(wording_width);
					ImGui::Text(" x ");
					ImGui::SameLine(wording_width + 30.f);
					ImGui::SetNextItemWidth(input_width - 30.f);
					float scalex = selectedObject ? selectedObject->getScale().x : 0.00f;
					float scaley = selectedObject ? selectedObject->getScale().y : 0.00f;
					ImGui::InputFloat("##scale_x", &scalex, 0.1f, 1.0f, "%.3f");
					bool scalexchanged = ImGui::IsItemEdited();
					ImGui::Text("");
					ImGui::SameLine(wording_width);
					ImGui::Text(" y ");
					ImGui::SameLine(wording_width + 30.f);
					ImGui::SetNextItemWidth(input_width - 30.f);
					ImGui::InputFloat("##scale_y", &scaley, 0.1f, 1.0f, "%.3f");
					bool scaleychanged = ImGui::IsItemEdited();
					ImGui::Dummy(ImVec2(0.0f, 1.0f));

					if(selectedObject && (scalexchanged || scaleychanged))
					{
						selectedObject->setScale(scalex, scaley);
					}

					ImGui::Text("Rotation");
					ImGui::SameLine(wording_width + 30.f);
					ImGui::SetNextItemWidth(input_width - 30.f);
					float rotation = selectedObject ? selectedObject->getRotation() : 0.00f;
					ImGui::InputFloat("", &rotation, 1.f, 1.0f, "%.3f");
					if(selectedObject && ImGui::IsItemEdited())
					{
						selectedObject->setRotation(rotation);
					}

					ImGui::EndChild();
				}

				if(selectedObject)
				{
					std::vector<Object::Ptr> componentTable = getComponentTable(selectedObject);
					for(Object::Ptr component : componentTable)
					{
							switch(component->getFirstType())
							{
								case Object::Sprite_Object:
								{
									ImGui::SetCursorPosX(10.f);
									if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
									{
										ImGui::BeginChild("object_sprite", ImVec2(0.f, 70.f), true);

										float wording_width = 70.f;
										float input_width = ImGui::GetWindowContentRegionWidth() - 70.f;

										SpriteObject::Ptr spriteObject = SpriteObject::Cast(component);

										ImGui::Text("Sprite");
										ImGui::SameLine(wording_width);
										ImGui::SetNextItemWidth(input_width);
										char* object_sprite = "";
										ImGui::InputText("##object_sprite", object_sprite, sizeof(object_sprite), ImGuiInputTextFlags_ReadOnly);
										ImGui::Dummy(ImVec2(0.0f, 1.0f));
										ImGui::Text("Texture");
										ImGui::SameLine(wording_width);
										ImGui::SetNextItemWidth(input_width);
										char* sprite_texture = "";
										ImGui::InputText("##sprite_texture", object_sprite, sizeof(sprite_texture), ImGuiInputTextFlags_ReadOnly);
										ImGui::Dummy(ImVec2(0.0f, 1.0f));

										ImGui::EndChild();
									}
								}break;

								case Object::Mesh_Object:
								{
									if(ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
									{
										ImGui::BeginChild("mesh_object", ImVec2(0.f, 100.f), true);

										float wording_width = 70.f;
										float input_width = ImGui::GetWindowContentRegionWidth() - 70.f;
										MeshObject::Ptr meshObject = MeshObject::Cast(component);

										ImGui::Text("Shape");
										ImGui::SameLine(wording_width);
										ImGui::SetNextItemWidth(input_width);
										char* object_sprite = "";
										ImGui::InputText("##mesh_shape", object_sprite, sizeof(object_sprite), ImGuiInputTextFlags_ReadOnly);
										ImGui::Dummy(ImVec2(0.0f, 1.0f));
										ImGui::Text("Type");
										ImGui::SameLine(wording_width);
										ImGui::SetNextItemWidth(input_width);
										char* sprite_texture = "";
										ImGui::InputText("##mesh_type", object_sprite, sizeof(sprite_texture), ImGuiInputTextFlags_ReadOnly);
										ImGui::Dummy(ImVec2(0.0f, 1.0f));

										ImGui::EndChild();
									}

									if(ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
									{
										ImGui::BeginChild("physics_data", ImVec2(0.f, 100.f), true);

										MeshObject::Ptr meshObject = MeshObject::Cast(component);

										ImGui::EndChild();
									}
								}break;

							}

					}
				}

				ImGui::EndChild();
			}

			ImGui::PopStyleColor();


		}

		ImGui::End();
	}

    std::vector<Object::Ptr> EditorUI::getComponentTable(Object::Ptr root)
	{
		std::vector<Object::Ptr> result;

		getComponentTable(root, result);

		return result;
	}

    void EditorUI::getComponentTable(Object::Ptr object, std::vector<Object::Ptr>& result)
	{

		if(object != nullptr)
		{
			result.push_back(object);
		}

		auto childTab = object->getAllChild();
		for (auto it = childTab->rbegin(); it != childTab->rend(); it++)
		{
			getComponentTable(*it, result);
		}
	}

    void EditorUI::showBackgroundTaskWindow()
	{
		if(!m_GameProject)
		{
			return;
		}

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
                m_GameProject		= m_ProjectManager->openProject(projectDirectory);
                m_AdvancedScene		= m_GameProject->getAdvancedScene();
                m_AdvancedScene->setRenderTexture(m_RenderTexture);
                m_AdvancedScene->setRenderContext(m_RenderContext);

                //update editor window title
                m_UpdateWindowTitleCallback(m_GameProject->getProjectName());
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
            m_ResourceManager	= nullptr;
            m_AdvancedScene		= nullptr;
            m_GameProject		= nullptr;
            m_LevelBuilder	= nullptr;
            m_WorldBuilder		= nullptr;
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

            if(m_GameProject)
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
            // Advanced Scene not available
            if(!m_AdvancedScene)
                return;

            // Create new Game Level
            const std::string levelName = m_AdvancedScene->createLevel(levelParameter);

            if(levelName != StringPool.BLANK)
            {
                m_EditorContext->setSelectedGameLevelName(levelName);
                m_EditorProxy->openGameLevel(levelName);
            }

        };

        m_EditorProxy->m_OpenGameLevelCallback = [this](const std::string levelName)
        {
            // Selected game level is already open
            if(m_LevelBuilder && m_LevelBuilder->getLevelName() == levelName)
            {
                return;
            }

            m_LevelBuilder      = m_AdvancedScene->openLevel(levelName);
            m_ResourceManager   = m_LevelBuilder->getResourceManager();
            m_WorldBuilder      = m_LevelBuilder->getSelectedChunk()->getWorldBuilder();
            m_EditorContext->setOpenedGameLevelName(levelName);
        };
    }
}
