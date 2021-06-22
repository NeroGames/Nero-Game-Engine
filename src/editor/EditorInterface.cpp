////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Poco
#include <Poco/Environment.h>
//SFML
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/editor/EditorInterface.h>
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
    EditorInterface::EditorInterface(sf::RenderWindow& window):
		 m_RenderWindow(window)
		,m_InterfaceFirstDraw(true)
		,m_SelectedScriptTypeIndex(0)
		,m_SelectedGameLevelIndex(0)
		,m_SelectedGameScreenIndex(0)
		,m_AdvancedScene(nullptr)
		,g_Context(nullptr)
		,m_BuildDockspaceLayout(true)
		,m_SetupDockspaceLayout(true)
		,m_ProjectManagerTabBarSwitch()
		,m_BottomDockspaceTabBarSwitch()
		,m_EditorSetting(nullptr)
		,m_ResourceBrowserType(ResourceType::None)
		,m_EditorMode(EditorMode::WORLD_BUILDER)
		,m_BuilderMode(BuilderMode::OBJECT)
		,m_EditorCamera(nullptr)
		,m_SelectedChunkNode(StringPool.BLANK)
		,m_InputSelectedWorldChunkId(-1)
		,m_InputSelectedObjectLayerId(-1)
		,m_InputSelectedGameLevelId(-1)
		,m_InputSelectedGameScreenId(-1)
		,m_MouseInformation("Mouse Position")
		,m_CountCreateProject(0)
		,m_ConsoleApplication()
		,m_EnvironmentSetup()
		,m_SelectedResourceManager(nullptr)
    {
		//empty
	}

    EditorInterface::~EditorInterface()
    {
	   destroy();
    }

	void EditorInterface::destroy()
	{
		nero_log("destroying engine interface ...");
		ax::NodeEditor::DestroyEditor(g_Context);
	}

	void EditorInterface::init()
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

		m_RenderTexture = std::make_shared<sf::RenderTexture>();
		m_ProjectManager = std::make_unique<ProjectManager>();
		m_RenderContext = std::make_shared<RenderContext>();

		g_Context = ax::NodeEditor::CreateEditor();

		m_ProjectManagerTabBarSwitch.registerTabTable(
		{
			EditorConstant.TAB_RECENT_PROJECT,
			EditorConstant.TAB_CREATE_PROJECT,
			EditorConstant.TAB_OPEN_PROJECT,
			EditorConstant.TAB_WORKSPACE
		});

		m_BottomDockspaceTabBarSwitch.registerTabTable(
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


		//setup game mode information text
		m_GameModeInfo.setFont(m_EditorFontHolder->getDefaultFont());
		m_GameModeInfo.setCharacterSize(18.f);
		m_GameModeInfo.setFillColor(sf::Color::White);

		//clear workspace input
		m_WorkspaceInput.clear();
		m_ProjectInput.clear();
		m_ProjectInput.update(m_ProjectManager->getWorkspaceTable());
		clearScriptWizardInput();


		//
		m_ProjectManager->setSetting(m_EditorSetting);

		//register signal handler
		registerSignalHandler();

		//check environment
		Setting envSetting = m_EditorSetting->getSetting("environment");
			//welcome
		m_EnvironmentSetup.viewTable.push_back([this](){showConfigurationWelcome();});
			//editor
		if(envSetting.getString("qt_creator") == StringPool.BLANK &&
		   envSetting.getString("visual_studio") == StringPool.BLANK)
		{
			m_EnvironmentSetup.setupCodeEditor = true;
			m_EnvironmentSetup.viewTable.push_back([this](){showConfigurationEditor();});
		}
			//texture packer
		if(envSetting.getString("texture_packer") == StringPool.BLANK)
		{
			m_EnvironmentSetup.setupTexturePacker = true;
			m_EnvironmentSetup.viewTable.push_back([this](){showConfigurationTexturePacker();});
		}
			//workspace
		if(m_ProjectManager->getWorkspaceTable().empty())
		{
			m_EnvironmentSetup.setupWorkspace = true;
			m_EnvironmentSetup.viewTable.push_back([this](){showConfigurationWorksapce();});
		}
			//finish
		m_EnvironmentSetup.viewTable.push_back([this](){showConfigurationFinish();});
			//clear inputs
		m_EnvironmentSetup.clearInput();
	}

	void EditorInterface::closeEditor()
	{
		nero_log("closing the editor ...");

		nero_log("saving window settings");
		std::string file = file::getPath({"setting", "window"}, StringPool.EXT_JSON);

		Setting window_setting = m_EditorSetting->getSetting("window");

		sf::Vector2u windowSize		= m_RenderWindow.getSize();
		sf::Vector2i windowPosition = m_RenderWindow.getPosition();

		if(windowPosition.x < -8)
		{
			windowPosition.x = -8;
		}

		if(windowPosition.y < 0)
		{
			windowPosition.y = 0;
		}

		window_setting.setUInt("width",  windowSize.x);
		window_setting.setUInt("height",  windowSize.y);
		window_setting.setInt("position_x",  windowPosition.x);
		window_setting.setInt("position_y",  windowPosition.y);

		file::saveFile(file, window_setting.toJson().dump(3), true);

		if(m_GameProject)
		{
			nero_log("closing project")
			closeProject();
		}

		nero_log("---------------------------------------------------------------------");
		nero_log("Engine Editor closed");

		m_RenderWindow.close();
	}

    void EditorInterface::handleEvent(const sf::Event& event)
    {
		if(mouseOnCanvas())
		{
			m_EditorCamera->handleEvent(event);
		}

		/*if(m_AdvancedScene)
		{
			m_AdvancedScene->handleEvent(event, m_EditorMode, m_BuilderMode);
		}*/

		//Demo
		if(m_GameProject)
		{
			try
			{
				m_GameProject->handleEvent(event);
			}
			catch (std::exception e)
			{
				m_GameProject->destroyScene();
			}
		}


		switch(event.type)
		{
			case sf::Event::Closed:
				closeEditor();
				break;

			//Keyboard
			case sf::Event::KeyPressed:
				handleKeyboardInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				handleKeyboardInput(event.key.code, false);
				break;

			//Mouse movements event
			/*case sf::Event::MouseMoved:
				handleMouseMoveInput(event.mouseMove);
				break;

			//Mouse wheel event
			case sf::Event::MouseWheelScrolled:
				handleMouseWheelInput(event.mouseWheelScroll);
				break;

			//Mouse button event
			case sf::Event::MouseButtonPressed:
				handleMouseButtonInput(event.mouseButton, true);
				break;
			case sf::Event::MouseButtonReleased:
				handleMouseButtonInput(event.mouseButton, false);
				break;*/
		}


        ImGui::SFML::ProcessEvent(event);
    }

    void EditorInterface::update(const sf::Time& timeStep)
	{
		m_EditorCamera->update(timeStep);

		/*if(m_AdvancedScene)
		{
			m_AdvancedScene->update(timeStep, m_EditorMode, m_BuilderMode);
		}*/

		//Demo
		if(m_GameProject)
		{
			try
			{
				m_GameProject->update(timeStep);
			}
			catch (std::exception e)
			{
				m_GameProject->destroyScene();
			}

		}
    }

    void EditorInterface::render()
    {
		EASY_FUNCTION(profiler::colors::Red);

        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);

		//create editor dockspace & display menubar
		createDockSpace();

		//central dockspcace
			//display toolbar
		showToolbarWindow();
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

		//show
		if(m_EnvironmentSetup.configure())
		{
			showConfigurationWindow();
		}


		//commit
		ImGui::SFML::Render(m_RenderWindow);
	}

	void EditorInterface::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
	{
		if(isPressed)
		{
			if(key == sf::Keyboard::Space && !keyboard::CTRL_SHIFT_ALT() && mouseOnCanvas())
				switchBuilderMode();
		}
	}

	void EditorInterface::updateFrameRate(const float& frameRate, const float& frameTime)
	{
		m_FrameRate = frameRate;
		m_FrameTime = frameTime;
	}

	void EditorInterface::setEditorSetting(Setting::Ptr setting)
	{
		m_EditorSetting = setting;
	}

	void EditorInterface::setEditorCamera(const AdvancedCamera::Ptr& camera)
	{
		m_EditorCamera = camera;
	}

	void EditorInterface::setEditorTextureHolder(TextureHolder::Ptr textureHolder)
	{
		m_EditorTextureHolder = textureHolder;
	}

	void EditorInterface::setEditorSoundHolder(SoundHolder::Ptr soundHolder)
	{
		m_EditorSoundHolder = soundHolder;
	}

	void EditorInterface::setEditorFontHolder(FontHolder::Ptr fontHolder)
	{
		m_EditorFontHolder = fontHolder;
	}

	void EditorInterface::setCallbackWindowTitle(std::function<void (const std::string&)> callback)
	{
		m_UpdateWindowTile = callback;
	}

	void EditorInterface::updateWindowTitle(const std::string& title)
	{
		 m_UpdateWindowTile(title);
	}

	void EditorInterface::switchBuilderMode()
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

    void EditorInterface::showSceneWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

			buildRenderContext();

			sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(m_RenderContext->mouse_position.x, m_RenderContext->mouse_position.y), m_RenderTexture->getView());

			std::string canvas_pos_string = "Canvas x = " + toString(m_RenderContext->mouse_position.x) + " y = " + toString(m_RenderContext->mouse_position.y);
			std::string wrold_pos_string = "World x = " + toString(world_pos.x) + " y = "  + toString(world_pos.y);
			std::string camera_pos_string = "Camera x = " + toString(m_EditorCamera->getPosition().x) + " y = "  + toString(m_EditorCamera->getPosition().y);

			/*if(ImGui::Button("Black", ImVec2(50.f, 0.f)))
			{

			}

			ImGui::SameLine();

			if(ImGui::Button("Gray", ImVec2(50.f, 0.f)))
			{

			}*/

			ImGui::SameLine();

			if(mouseOnCanvas())
			{
				m_MouseInformation = canvas_pos_string + " | " + wrold_pos_string + " | " + camera_pos_string;
			}
			float start = (ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize(m_MouseInformation.c_str()).x)/2.f;
			ImGui::SetCursorPosX(start);
			ImGui::Text(m_MouseInformation.c_str());

			if(m_AdvancedScene)
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 95.f);

				if(m_EditorMode == EditorMode::WORLD_BUILDER)
				{
					if(ImGui::Button("New Level", ImVec2(100.f, 0.f)))
					{
						ImGui::OpenPopup("Create Game Level");
					}
				}
				else if(m_EditorMode == EditorMode::SCREEN_BUILDER)
				{
					if(ImGui::Button("New Screen", ImVec2(100.f, 0.f)))
					{
						ImGui::OpenPopup("Create Game Screen");
					}
				}
				else if(m_EditorMode == EditorMode::OBJECT_BUILDER)
				{
					if(ImGui::Button("New Object", ImVec2(100.f, 0.f)))
					{

					}
				}
			}



			prepareRenderTexture();

			/*if(m_AdvancedScene)
			{
				m_AdvancedScene->render(m_EditorMode, m_BuilderMode);
			}*/

			//Demo
			if(m_GameProject)
			{
				try
				{
					m_GameProject->render();
				}
				catch(std::exception e)
				{
					m_GameProject->destroyScene();
					nero_log("failed to render scene");
					nero_log("please rebuild the scene")
				}
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


			ImGui::Image(flipTexture(m_RenderTexture->getTexture()));

			showCanvasMenu();

			showNewGameLevelPopup();
			showNewGameScreenPopup();


        ImGui::End();
    }

	void EditorInterface::showCanvasMenu()
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

	std::string EditorInterface::getString(const EditorMode& editorMode)
	{
		switch (editorMode)
		{
			case EditorMode::WORLD_BUILDER:		return "World Builder";		break;
			case EditorMode::SCREEN_BUILDER:	return "Screen Builder";	break;
			case EditorMode::OBJECT_BUILDER:	return "Object Builder";	break;
			case EditorMode::PLAY_GAME:			return "Play Game";			break;
			case EditorMode::RENDER_GAME:		return "Render Game";		break;

			default: return StringPool.BLANK; break;
		}
	}

	void EditorInterface::renderGameModeInfo()
	{
		std::string gameMode = getString(m_EditorMode);
		std::string frameRate = toString(m_FrameRate) + " fps";
		std::string frameTime = toString(m_FrameTime * 1000.f) + " ms";

		std::string info = gameMode + "  |  " + frameRate + "  |  " + frameTime;

		m_GameModeInfo.setString(info);

		sf::Vector2f position;
		position.x = m_RenderTexture->getSize().x - m_GameModeInfo.getLocalBounds().width - 20.f;
		position.y = m_RenderTexture->getSize().y - m_GameModeInfo.getLocalBounds().height - 20.f;

		m_GameModeInfo.setPosition(position);

		m_RenderTexture->draw(m_GameModeInfo);
	}

	void EditorInterface::renderCamera()
	{
		m_CameraXAxis.setRotation(m_EditorCamera->getView().getRotation());
		m_CameraYAxis.setRotation(m_EditorCamera->getView().getRotation() + 90.f);

		m_RenderTexture->draw(m_CameraXAxis);
		m_RenderTexture->draw(m_CameraYAxis);
	}



	void EditorInterface::buildRenderContext()
	{
		sf::Vector2f window_position    = ImGui::GetWindowPos();
		sf::Vector2f window_size        = ImGui::GetWindowSize();
		sf::Vector2f mouse_position     = ImGui::GetMousePos();
		float title_bar_height          = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
		sf::Vector2f window_padding     = ImGui::GetStyle().WindowPadding;

		window_size.y += -23.f;

		RenderContext renderContext;
		renderContext.canvas_position   = sf::Vector2f(window_position.x + window_padding.x, window_position.y + title_bar_height + window_padding.y);
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

	void EditorInterface::prepareRenderTexture()
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

	bool EditorInterface::mouseOnCanvas()
	{
		sf::Rect<float> canvas(m_RenderContext->canvas_position.x, m_RenderContext->canvas_position.y, m_RenderContext->canvas_size.x, m_RenderContext->canvas_size.y);

		sf::Vector2i mousePosition = ImGui::GetMousePos();

		return canvas.contains(mousePosition.x, mousePosition.y);
	}

	void EditorInterface::showGameSettingWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_GAME_SETTING.c_str());

		ImGui::End();
	}

	void EditorInterface::showVisualScriptWindow()
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

    void EditorInterface::showGameProjectWindow()
    {
		ImGui::Begin(EditorConstant.WINDOW_GAME_PROJECT.c_str());

        ImGui::End();
    }



	//1- create workspace
    void EditorInterface::createDockSpace()
	{
		//tranfer viewport state to dockspace window
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

		//create dockspace window style
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
										ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		//add style
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin(EditorConstant.WINDOW_EDITOR_DOCKSPACE.c_str(), nullptr, window_flags);

			//remove style
			ImGui::PopStyleVar(3);

			//save dockspace id
			m_DockspaceID = ImGui::GetID(EditorConstant.ID_DOCKSPACE.c_str());

			//create the dockspace from current window
			ImGui::DockSpace(m_DockspaceID, ImVec2(viewport->Size.x, viewport->Size.y - 20.f), ImGuiDockNodeFlags_None);

			//clean pointer
			viewport = nullptr;

			//build dockspace layout : this is done only once, when the editor is launched the first time
			//if(m_EditorSetting->getSetting("dockspace").getBool("build_layout") && !file::fileExist(file::getPath({"setting", EditorConstant.FILE_IMGUI_SETTING}, StringPool.EXT_INI)))
			if(m_EditorSetting->getSetting("dockspace").getBool("build_layout") && !m_EditorSetting->getSetting("dockspace").getBool("imgui_setting_exist"))
			{
				//split main dockspace in six
				ImGuiID upperLeftDockspaceID	= ImGui::DockBuilderSplitNode(m_DockspaceID,		ImGuiDir_Left,	0.20f, nullptr, &m_DockspaceID);
				ImGui::DockBuilderGetNode(upperLeftDockspaceID)->SizeRef.x = m_EditorSetting->getSetting("dockspace").getSetting("upper_left_dock").getFloat("width");
				ImGuiID lowerLeftDockspaceID	= ImGui::DockBuilderSplitNode(upperLeftDockspaceID,	ImGuiDir_Down,	0.20f, nullptr, &upperLeftDockspaceID);
				ImGuiID rightDockspaceID        = ImGui::DockBuilderSplitNode(m_DockspaceID,		ImGuiDir_Right, 0.20f, nullptr, &m_DockspaceID);
				ImGuiID toolbarDockspaceID		= ImGui::DockBuilderSplitNode(m_DockspaceID,		ImGuiDir_Up,	0.20f, nullptr, &m_DockspaceID);
				ImGuiID bottomDockspaceID       = ImGui::DockBuilderSplitNode(m_DockspaceID,		ImGuiDir_Down,	0.20f, nullptr, &m_DockspaceID);
				ImGuiID centralDockspaceID		= ImGui::DockBuilderGetCentralNode(m_DockspaceID)->ID;

				//lay windows
					//tool bar
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_TOOLBAR.c_str(),				toolbarDockspaceID);
					//upper left
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_UTILITY.c_str(),				upperLeftDockspaceID);
				//ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_SCREEN.c_str(),				upperLeftDockspaceID);
					//lower left
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_CHUNCK.c_str(),				lowerLeftDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LAYER.c_str(),				lowerLeftDockspaceID);
					//right
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_EXPLORER.c_str(),			rightDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_HELP.c_str(),				rightDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_RESOURCE_BROWSER.c_str(),	rightDockspaceID);
					//bottom
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LOGGING.c_str(),				bottomDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_RESOURCE.c_str(),			bottomDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_CONSOLE.c_str(),				bottomDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LEVEL.c_str(),				bottomDockspaceID);
					//center
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_SCENE.c_str(),			centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_PROJECT.c_str(),		centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_SETTING.c_str(),		centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_VISUAL_SCRIPT.c_str(),		centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_FACTORY.c_str(),				centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_IMGUI_DEMO.c_str(),			centralDockspaceID);

				//commit dockspace
				ImGui::DockBuilderFinish(m_DockspaceID);

				//lower left dockspace
				ImGuiDockNode* lowerLeftDockNode	= ImGui::DockBuilderGetNode(lowerLeftDockspaceID);
				lowerLeftDockNode->SizeRef.y = m_EditorSetting->getSetting("dockspace").getSetting("lower_left_dock").getFloat("height");
				//right dockspace
				ImGuiDockNode* rightDockNode		= ImGui::DockBuilderGetNode(rightDockspaceID);
				rightDockNode->SizeRef.x = m_EditorSetting->getSetting("dockspace").getSetting("right_dock").getFloat("width");
				//bottom dockspace
				ImGuiDockNode* bottomDockNode		= ImGui::DockBuilderGetNode(bottomDockspaceID);
				bottomDockNode->SizeRef.y = m_EditorSetting->getSetting("dockspace").getSetting("bottom_dock").getFloat("height");

				//clean pointer
				lowerLeftDockNode	= nullptr;
				rightDockNode		= nullptr;
				bottomDockNode		= nullptr;

				nero_log(nero_sv(toolbarDockspaceID));

				//update and save dockspace setting
				m_EditorSetting->getSetting("dockspace").getSetting("toolbar_dock").setUInt("id", toolbarDockspaceID);
				m_EditorSetting->getSetting("dockspace").setBool("build_layout", false);

				file::saveFile(file::getPath({"setting", "dockspace"}, StringPool.EXT_JSON), m_EditorSetting->getSetting("dockspace").toString(), true);
			}

			if(m_SetupDockspaceLayout)
			{
				//toolbar
				ImGuiID toolbarDockspaceID			= m_EditorSetting->getSetting("dockspace").getSetting("toolbar_dock").getUInt("id");
				ImGuiDockNode* toolbarDockNode		= ImGui::DockBuilderGetNode(toolbarDockspaceID);
				toolbarDockNode->SizeRef.y			= m_EditorSetting->getSetting("dockspace").getSetting("toolbar_dock").getFloat("height");
				toolbarDockNode->LocalFlags			= ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit |
													  ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;
				//clean pointer
				toolbarDockNode		= nullptr;

				m_SetupDockspaceLayout = false;
			}

			if (ImGui::BeginMenuBar())
			{
				showEditorMenuBar();

				ImGui::EndMenuBar();
			}

		ImGui::End(); //end dockspace window
    }

	//2- create menu bar
	void EditorInterface::showEditorMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("New Project", nullptr, false))
			{
				m_MenuBarInput.newProject = true;
			}

			if(ImGui::MenuItem("Open Project", nullptr, false))
			{
				m_MenuBarInput.openProject = true;
			}

			if(ImGui::MenuItem("Save Project", nullptr, false, m_GameProject != nullptr))
			{
				//saveProject();
			}

			if(ImGui::MenuItem("Close Project", nullptr, false, m_GameProject != nullptr))
			{
				closeProject();
			}

			ImGui::Separator();

			if(ImGui::MenuItem("New Workspace", nullptr, false))
			{
				m_MenuBarInput.newWorkspace = true;
			}

			if(ImGui::MenuItem("Import Workspace", nullptr, false))
			{
				m_MenuBarInput.newWorkspace = true;
			}

			ImGui::Separator();

			auto recentProjectTable =  file::loadJson(file::getPath({"setting", "recent_project"}));

			if (ImGui::BeginMenu("Recent Projects",!recentProjectTable.empty()))
			{

				int count_project = recentProjectTable.size() > 10 ? 10 : recentProjectTable.size();

				for (int i = count_project - 1; i >=0 ; i--)
				{
					auto project = recentProjectTable[i];

					if(ImGui::MenuItem(project["project_name"].get<std::string>().c_str(), nullptr, false))
					{
						openProject(project["project_directory"].get<std::string>());
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Exit", nullptr, false))
			{
				closeEditor();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit", m_GameProject != nullptr))
		{
			if(ImGui::MenuItem("Undo", nullptr, nullptr))
			{

			}

			if(ImGui::MenuItem("Redo", nullptr, nullptr))
			{

			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build", m_GameProject != nullptr))
		{
			if(ImGui::MenuItem("Clean", nullptr, nullptr))
			{

			}

			if(ImGui::MenuItem("Run Cmake", nullptr, nullptr))
			{

			}

			if(ImGui::MenuItem("Compile", nullptr, nullptr))
			{

			}

			if(ImGui::MenuItem("Reload", nullptr, nullptr))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Views", false))
		{
			const char* names[] = { "Logging", "Quick Helps", "Mackerel", "Pollock", "Tilefish" };
			static bool toggles[] = { true, false, false, false, false };
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
				ImGui::MenuItem(names[i], "", &toggles[i]);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings", false))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if(ImGui::MenuItem("Profiler", "Ctrl+Alt+P", nullptr))
			{
				AppLauncher::launchProfiler();
			}

			if(ImGui::MenuItem("Texture Packer", "Ctrl+Alt+T", nullptr))
			{
				AppLauncher::launchTexturePacker();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if(ImGui::MenuItem("Learn", "Alt+L", nullptr))
			{
				cmd::launchBrowser("https://nero-game.com/learn");
			}

			if(ImGui::MenuItem("Snippet", "Alt+S", nullptr))
			{
				cmd::launchBrowser("https://nero-game.com/snippet");
			}

			if(ImGui::MenuItem("Forum", "Alt+F", nullptr))
			{
				cmd::launchBrowser("https://nero-game.com/forum/index.php");
			}

			if(ImGui::MenuItem("Engine API", "Alt+A", nullptr))
			{
				cmd::launchBrowser("https://nero-game.com/engine-v2/api");
			}

			if(ImGui::MenuItem("Website", "Alt+W", nullptr))
			{
				cmd::launchBrowser("https://nero-game.com");
			}

			ImGui::Separator();

			if(ImGui::MenuItem("About Nero Game Engine", nullptr, nullptr))
			{
				m_MenuBarInput.aboutEngine = true;
			}

			if(ImGui::MenuItem("Check for Updates", nullptr, nullptr, false))
			{

			}

			ImGui::EndMenu();
		}
	}

	void EditorInterface::showConfigurationWindow()
	{
		//welcome the user
		//configure the code editor
		//configure texture packer
		//configure workspace
		ImGui::OpenPopup(EditorConstant.WINDOW_STARTER_WIZARD.c_str());

		//Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		//Winsow size
		ImVec2 winsow_size              = EditorConstant.WINDOW_STARTER_WIZARD_SIZE;

		//Project manager window
		ImGui::SetNextWindowSize(winsow_size);
		//Begin window
		if(ImGui::BeginPopupModal(EditorConstant.WINDOW_STARTER_WIZARD.c_str(), nullptr, window_flags))
		{
			ImGui::BeginChild("##banner", ImVec2(winsow_size.x * 0.2f, 0.f));
				ImGui::Image(m_EditorTextureHolder->getTexture("editor_configuration"));
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("##configre");

				m_EnvironmentSetup.showView();

				ImGui::SetCursorPosY(EditorConstant.WINDOW_STARTER_WIZARD_SIZE.y - 75.f);

				if(m_EnvironmentSetup.hasPrevious())
				{
					pushToolbarStyle();
					if(ImGui::Button("Back", ImVec2(100.f, 0.f)))
					{
						m_EnvironmentSetup.previousView();
					}
					popToolbarStyle();
				}


				if(m_EnvironmentSetup.readyNext)
				{
					if(m_EnvironmentSetup.hasPrevious())
						ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 115.f);
					else
						ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 115.f);

					pushToolbarStyle();
					if(ImGui::Button(m_EnvironmentSetup.finish() ? "Finish" : "Next", ImVec2(100.f, 0.f)))
					{
						if(m_EnvironmentSetup.finish())
						{
							m_EnvironmentSetup.setupWorkspace = false;
							m_EnvironmentSetup.setupCodeEditor = false;
							m_EnvironmentSetup.setupTexturePacker = false;
							ImGui::CloseCurrentPopup();
						}
						else
						{
							m_EnvironmentSetup.nextView();
						}
					}
					popToolbarStyle();
				}

			ImGui::EndChild();

			ImGui::EndPopup();
		}
	}

	void EditorInterface::showConfigurationWelcome()
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Text("Welcome To The Configuration Wizard");
		ImGui::PopFont();
		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::TextWrapped("If you see this window it's because it's your first time using the Engine or because you are missing some configurations");
		ImGui::TextWrapped("The following things need to done in order for the Engine to function properly");

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::Indent();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		if(m_EnvironmentSetup.setupCodeEditor)
		{
			ImGui::Text(ICON_FA_CIRCLE "	Configure a Code Editor");
			ImGui::Dummy(ImVec2(0.f, 5.f));
		}
		if(m_EnvironmentSetup.setupTexturePacker)
		{
			ImGui::Text(ICON_FA_CIRCLE "	Configure Texture Packer");
			ImGui::Dummy(ImVec2(0.f, 5.f));

		}
		if(m_EnvironmentSetup.setupWorkspace)
		{
			ImGui::Text(ICON_FA_CIRCLE "	Create a Workspace");
			ImGui::Dummy(ImVec2(0.f, 5.f));
		}
		ImGui::PopFont();
		ImGui::Unindent();

		ImGui::Dummy(ImVec2(0.f, 15.f));

		ImGui::AlignTextToFramePadding();
		ImGui::Text("You can learn more on the necessary configurations at "); ImGui::SameLine();
		if(ImGui::Button("Environment Configurations"))
		{
			cmd::launchBrowser("https://nero-game.com/learn/engine-v2/environment-configuration");
		}

		ImGui::Dummy(ImVec2(0.f, 15.f));

		ImGui::Text("Click on Next to continous");

		m_EnvironmentSetup.readyNext = true;
	}

	void EditorInterface::showConfigurationFinish()
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Text("Configuration Completed");
		ImGui::PopFont();
		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::TextWrapped("Congratulation ! You now have a proper environment");

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::Text("Click Finish to close the Wizard");

		m_EnvironmentSetup.readyNext = true;
	}

	void EditorInterface::showConfigurationEditor()
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Text("Configure a Code Editor");
		ImGui::PopFont();
		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::TextWrapped("You can use QT Creator or Visual Studio 2019, We recommand using QT Creator");

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 550.f)/2.f);

		pushToolbarStyle(m_EnvironmentSetup.selectQtCreator);
		if(ImGui::Button("QT Creator", ImVec2(250.f, 50.f)))
		{
			m_EnvironmentSetup.selectQtCreator		= true;
			m_EnvironmentSetup.selectVisualStudio	= false;
		}
		popToolbarStyle();

		ImGui::SameLine(0.f, 50.f);

		pushToolbarStyle(m_EnvironmentSetup.selectVisualStudio);
		if(ImGui::Button("Visual Studio 2019", ImVec2(250.f, 50.f)))
		{
			m_EnvironmentSetup.selectQtCreator		= false;
			m_EnvironmentSetup.selectVisualStudio	= true;
		}
		popToolbarStyle();

		ImGui::Dummy(ImVec2(0.f, 50.f));

		if(m_EnvironmentSetup.selectQtCreator)
		{
			ImGui::TextWrapped("Install and Configure QT Creator");

			ImGui::Dummy(ImVec2(0.f, 10.f));

			ImGui::Text("After installing QT Creator, please provide the path to its excutable");
			ImGui::Text("Here are some examples of path");
			ImGui::BulletText("C:/Qt/qtcreator-4.11.2/bin/qtcreator.exe");
			ImGui::BulletText("C:/Qt/Tools/QtCreator/bin/qtcreator.exe");

			ImGui::Dummy(ImVec2(0.f, 10.f));

			float wording_width = 100.f;
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Download here");
			ImGui::SameLine(wording_width);
			if(ImGui::Button("https://www.qt.io/offline-installers"))
			{
				cmd::launchBrowser("https://www.qt.io/offline-installers");
			}

			ImGui::Dummy(ImVec2(0.f, 10.f));

			float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;
			ImGui::Text("QtCreator");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width - 100.f);
			ImGui::InputText("##qtcreator", m_EnvironmentSetup.qtCreatorPath, sizeof(m_EnvironmentSetup.qtCreatorPath), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine(wording_width + input_width - 80.f);
			if(ImGui::Button("Browse##editor_path", ImVec2(60.f, 0)))
			{
				selectFile([this](std::string outPath)
				{
					if(outPath.find("qtcreator.exe") != std::string::npos)
					{
						string::fillCharArray(m_EnvironmentSetup.qtCreatorPath, sizeof(m_EnvironmentSetup.qtCreatorPath), outPath);

						putenv(std::string("NERO_GAME_QT=" + outPath).c_str());

						BTManager::startTask([outPath](BackgroundTask::Ptr backgroundTask)
						{
							cmd::setEnvironmnentVariable("NERO_GAME_QT", outPath);
							backgroundTask->setCompleted(true);
						});

						m_EditorSetting->getSetting("environment").setString("qt_creator", outPath);
					}
					else
					{
						ImGui::OpenPopup(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str());
					}

				});
			}

		}
		else
		{
			ImGui::TextWrapped("Install and Configure Visual Studio 2019");

			ImGui::Dummy(ImVec2(0.f, 10.f));

			ImGui::Text("After installing Visual Studio, please provide the path to its excutable.");
			ImGui::Text("Here are some examples of path");
			ImGui::BulletText("C:/Program Files (x86)/Microsoft Visual Studio/2019/Preview/Common7/IDE/devenv.exe");

			ImGui::Dummy(ImVec2(0.f, 10.f));

			float wording_width = 100.f;
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Download here");
			ImGui::SameLine(wording_width);
			if(ImGui::Button("https://visualstudio.microsoft.com/downloads/"))
			{
				cmd::launchBrowser("https://visualstudio.microsoft.com/downloads/");
			}

			ImGui::Dummy(ImVec2(0.f, 10.f));

			float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;
			ImGui::Text("Visual Studio");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width - 100.f);
			ImGui::InputText("##visualstudio", m_EnvironmentSetup.visualStudioPath, sizeof(m_EnvironmentSetup.visualStudioPath), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine(wording_width + input_width - 80.f);
			if(ImGui::Button("Browse##editor_path", ImVec2(60.f, 0)))
			{
				selectFile([this](std::string outPath)
				{
					if(outPath.find("devenv.exe") != std::string::npos)
					{
						string::fillCharArray(m_EnvironmentSetup.visualStudioPath, sizeof(m_EnvironmentSetup.visualStudioPath), outPath);

						putenv(std::string("NERO_GAME_VS=" + outPath).c_str());

						BTManager::startTask([outPath](BackgroundTask::Ptr backgroundTask)
						{
							cmd::setEnvironmnentVariable("NERO_GAME_VS", outPath);
							backgroundTask->setCompleted(true);
						});

						m_EditorSetting->getSetting("environment").setString("visual_studio", outPath);
					}
					else
					{
						ImGui::OpenPopup(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str());
					}

				});
			}
		}

		//Error modal
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowSize(ImVec2(250.f, 100.f));
		if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str(), nullptr, window_flags))
		{
			ImGui::TextWrapped("The path provided is not valid");

			ImGui::SetCursorPosY(100.f - 30.f);
			ImGui::SetCursorPosX((250.f - 75.f)/2.f);
			if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		m_EnvironmentSetup.readyNext =	std::string(m_EnvironmentSetup.qtCreatorPath) != StringPool.BLANK
										|| std::string(m_EnvironmentSetup.visualStudioPath) != StringPool.BLANK;
	}

	void EditorInterface::showConfigurationTexturePacker()
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Text("Configure Texture Packer");
		ImGui::PopFont();
		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.f, 10.f));

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 150.f)/2.f);
		ImGui::Image(m_EditorTextureHolder->getTexture("texture_packer_icon"), ImVec2(150.f, 150.f));

		ImGui::Dummy(ImVec2(0.f, 10.f));

		ImGui::TextWrapped("Install and Configure Texture Packer");

		ImGui::Dummy(ImVec2(0.f, 10.f));

		ImGui::Text("After installing Texture Packer, please provide the path to its excutable.");
		ImGui::Text("Here are some examples of path");
		ImGui::BulletText("C:/Program Files/CodeAndWeb/TexturePacker/bin/TexturePackerGUI.exe");

		ImGui::Dummy(ImVec2(0.f, 10.f));

		float wording_width = 100.f;
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Download here");
		ImGui::SameLine(wording_width);
		if(ImGui::Button("https://www.codeandweb.com/texturepacker"))
		{
			cmd::launchBrowser("https://www.codeandweb.com/texturepacker");
		}

		ImGui::Dummy(ImVec2(0.f, 10.f));

		float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;
		ImGui::Text("Texture Packer");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width - 100.f);
		ImGui::InputText("##texturepacker", m_EnvironmentSetup.texturePackerPath, sizeof(m_EnvironmentSetup.texturePackerPath), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine(wording_width + input_width - 80.f);
		if(ImGui::Button("Browse##texturepacker", ImVec2(60.f, 0)))
		{
			selectFile([this](std::string outPath)
			{
				if(outPath.find("TexturePackerGUI.exe") != std::string::npos)
				{
					string::fillCharArray(m_EnvironmentSetup.texturePackerPath, sizeof(m_EnvironmentSetup.texturePackerPath), outPath);

					putenv(std::string("NERO_GAME_TP=" + outPath).c_str());

					BTManager::startTask([outPath](BackgroundTask::Ptr backgroundTask)
					{
						cmd::setEnvironmnentVariable("NERO_GAME_TP", outPath);
						backgroundTask->setCompleted(true);
					});

					m_EditorSetting->getSetting("environment").setString("texture_packer", outPath);
				}
				else
				{
					ImGui::OpenPopup(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str());
				}

			});
		}

		//Error modal
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowSize(ImVec2(250.f, 100.f));
		if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str(), nullptr, window_flags))
		{
			ImGui::TextWrapped("The path provided is not valid");

			ImGui::SetCursorPosY(100.f - 30.f);
			ImGui::SetCursorPosX((250.f - 75.f)/2.f);
			if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		m_EnvironmentSetup.readyNext =	std::string(m_EnvironmentSetup.texturePackerPath) != StringPool.BLANK;
	}

	void EditorInterface::showConfigurationWorksapce()
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Text("Create or Import Workspace");
		ImGui::PopFont();
		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::TextWrapped("Create a new workspace or import an existing one");

		ImGui::Dummy(ImVec2(0.f, 20.f));

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 550.f)/2.f);

		pushToolbarStyle(m_EnvironmentSetup.createWorkspace);
		if(ImGui::Button("Create", ImVec2(250.f, 50.f)))
		{
			m_EnvironmentSetup.createWorkspace		= true;
			m_EnvironmentSetup.importWorkspace		= false;
		}
		popToolbarStyle();

		ImGui::SameLine(0.f, 50.f);

		pushToolbarStyle(m_EnvironmentSetup.importWorkspace);
		if(ImGui::Button("Import", ImVec2(250.f, 50.f)))
		{
			m_EnvironmentSetup.createWorkspace		= false;
			m_EnvironmentSetup.importWorkspace		= true;
		}
		popToolbarStyle();

		ImGui::Dummy(ImVec2(0.f, 50.f));

		if(m_EnvironmentSetup.createWorkspace)
		{
			showCreateWorkspace();
		}
		else
		{
			showImportWorkspace();
		}

		m_EnvironmentSetup.readyNext = !m_ProjectManager->getWorkspaceTable().empty();
	}

	//3-  show toolbar
	void EditorInterface::showToolbarWindow()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
		//ImGui::SetNextWindowSize(ImVec2(), ImGuiCond_Always);
		ImGui::Begin(EditorConstant.WINDOW_TOOLBAR.c_str(), nullptr, window_flags);

		float content_region_width	= ImGui::GetWindowContentRegionWidth();
		bool  scrollToolbar			= content_region_width < EditorConstant.WINDOW_TOOLBAR_MIN_WIDTH;

		if(scrollToolbar)
		{
			showToolbarScrollLeft();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.f, 6.f));
		if(!scrollToolbar)
			ImGui::BeginChild("tool_bar_button", ImVec2(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding);
		else
		{
			ImGui::SetNextWindowContentWidth(EditorConstant.WINDOW_TOOLBAR_MIN_WIDTH);
			ImGui::BeginChild("tool_bar_button", ImVec2(content_region_width-67.f, 0.f), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_ScrollToolbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysUseWindowPadding);
			mToolbarWindow = ImGui::GetCurrentWindow();
		}
			pushToolbarStyle();

			showToolbarLeft(scrollToolbar);

			showToolbarMiddle(scrollToolbar);

			showToolbarRight(scrollToolbar);

			popToolbarStyle();

			//show project manager window
			showProjectManagerWindow();

			//show script creaton window
			showScriptCreationWindow();

			//show about engine
			showAboutEngineWindow();


		ImGui::EndChild();
		ImGui::PopStyleVar();


		if(scrollToolbar)
		{
			showToolbarScrollRight();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorInterface::showToolbarLeft(bool scrollToolbar)
	{
		ImGui::SameLine();

		if(m_EditorMode == EditorMode::WORLD_BUILDER && m_GameProject)
		{
			showToolbarScreenButton();
			showToolbarFactoryButton();
		}
		else if(m_EditorMode == EditorMode::SCREEN_BUILDER && m_GameProject)
		{
			showToolbarWorldButton();
			showToolbarFactoryButton();
		}
		else if(m_EditorMode == EditorMode::OBJECT_BUILDER && m_GameProject)
		{
			showToolbarScreenButton();
			showToolbarWorldButton();
		}
	}

	void EditorInterface::showToolbarWorldButton()
	{
		if(ImGui::Button(ICON_FA_GLOBE_AFRICA " World", ImVec2(105.f, 28.f)))
		{
			m_EditorMode = EditorMode::WORLD_BUILDER;
			ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
		}

		ImGui::SameLine();
	}

	void EditorInterface::showToolbarScreenButton()
	{
		if(ImGui::Button(ICON_FA_SQUARE " Screen", ImVec2(105.f, 28.f)))
		{
			m_EditorMode = EditorMode::SCREEN_BUILDER;
			ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
		}

		ImGui::SameLine();
	}

	void EditorInterface::showToolbarFactoryButton()
	{
		if(ImGui::Button(ICON_FA_WAREHOUSE " Factory", ImVec2(105.f, 28.f)))
		{
			m_EditorMode = EditorMode::OBJECT_BUILDER;
			ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
		}
	}

	void EditorInterface::showToolbarRight(bool scrollToolbar)
	{
		float rightOffset = 95.f;
		if(m_GameProject)
			rightOffset += 45.f * 3.f + 8.f * 3.f + 90.f + 3.f;
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - rightOffset);

		if(m_GameProject)
		{
			if(ImGui::Button(ICON_FA_CODE, ImVec2(90.f, 28.f)))
			{
				ImGui::OpenPopup(EditorConstant.WINDOW_SCRIPT_WIZARD.c_str());
			}

			ImGui::SameLine();

			if(ImGui::Button(ICON_FA_EDIT, ImVec2(45.f, 28.f)))
			{
				editProject();
			}

			ImGui::SameLine();

			if(ImGui::Button(ICON_FA_COGS, ImVec2(45.f, 28.f)))
			{
				compileProject();
			}

			ImGui::SameLine();

			if(ImGui::Button(ICON_FA_SYNC, ImVec2(45.f, 28.f)))
			{
				reloadProject();
			}

			ImGui::SameLine();
		}

		if(ImGui::Button(ICON_FA_ANCHOR " Project", ImVec2(95.f, 28.f)))
		{
			ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_RECENT_PROJECT);
		}
		else
		{
			handleMenuBarFileAction();
		}
	}

	void EditorInterface::showToolbarMiddle(bool scrollToolbar)
	{
		if(m_GameProject)
		{
			float offSet = ImGui::GetWindowContentRegionWidth();
			offSet = (offSet - (45 * 7.f + 8.f * 6.f + 32.f))/2.f;

			ImGui::SameLine(offSet);

			//undo
			if(ImGui::Button(ICON_FA_UNDO_ALT, ImVec2(45.f, 28.f)))
			{

			}

			ImGui::SameLine(0.f, 24.f);

			//play
			if(ImGui::Button(ICON_FA_PLAY, ImVec2(45.f, 28.f)))
			{

			}

			ImGui::SameLine();

			//pause
			if(ImGui::Button(ICON_FA_PAUSE, ImVec2(45.f, 28.f)))
			{

			}

			ImGui::SameLine();

			//step
			if(ImGui::Button(ICON_FA_STEP_FORWARD, ImVec2(45.f, 28.f)))
			{

			}

			ImGui::SameLine();

			//reset
			if(ImGui::Button(ICON_FA_CIRCLE, ImVec2(45.f, 28.f)))
			{

			}

			ImGui::SameLine();

			//render
			if(ImGui::Button(ICON_FA_GAMEPAD, ImVec2(45.f, 28.f)))
			{

			}

			ImGui::SameLine(0.f, 24.f);

			//redo
			if(ImGui::Button(ICON_FA_REDO_ALT, ImVec2(45.f, 28.f)))
			{

			}
		}
	}



	void EditorInterface::showToolbarScrollLeft()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
		ImGui::BeginChild("##toolbarscrollleft", ImVec2(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.590f, 0.980f, 1.000f));
			if(ImGui::Button("<<##scroll_back_button", ImVec2(32.f, 38.f)))
			{
				if(mToolbarWindow)
				{
					ImVec2 max_step;
					max_step.x = mToolbarWindow->InnerRect.GetSize().x * 0.67f;
					max_step.y = mToolbarWindow->InnerRect.GetSize().y * 0.67f;

					float scroll_step = ImFloor(ImMin(2 * mToolbarWindow->CalcFontSize(), max_step.x));
					ImGui::SetWindowScrollX(mToolbarWindow, mToolbarWindow->Scroll.x - 1 * scroll_step);
				}
			}
			ImGui::PopStyleColor();
		ImGui::EndChild();
		ImGui::PopStyleVar();
		ImGui::SameLine(32.f);

	}

	void EditorInterface::showToolbarScrollRight()
	{
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 32.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
		ImGui::BeginChild("##toolbarscrollright", ImVec2(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.590f, 0.980f, 1.000f));
			if(ImGui::Button(">>##scroll_forward_button", ImVec2(32.f, 38.f)))
			{
				if(mToolbarWindow)
				{
					ImVec2 max_step;
					max_step.x = mToolbarWindow->InnerRect.GetSize().x * 0.67f;
					max_step.y = mToolbarWindow->InnerRect.GetSize().y * 0.67f;

					float scroll_step = ImFloor(ImMin(2 * mToolbarWindow->CalcFontSize(), max_step.x));
					ImGui::SetWindowScrollX(mToolbarWindow, mToolbarWindow->Scroll.x + 1 * scroll_step);
				}
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		ImGui::EndChild();
	}

	void EditorInterface::handleMenuBarFileAction()
	{
		if(m_MenuBarInput.newProject)
		{
			m_MenuBarInput.newProject = false;
			ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_CREATE_PROJECT);
		}
		else if(m_MenuBarInput.openProject)
		{
			m_MenuBarInput.openProject = false;
			ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_OPEN_PROJECT);
		}
		else if(m_MenuBarInput.newWorkspace)
		{
			m_MenuBarInput.newWorkspace		= false;
			ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_WORKSPACE);
		}
		else if(m_MenuBarInput.aboutEngine)
		{
			m_MenuBarInput.aboutEngine = false;
			ImGui::OpenPopup(EditorConstant.WINDOW_ABOUT_ENGINE.c_str());
		}
	}

	//4- project manager
    void EditorInterface::showProjectManagerWindow()
    {
        //Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        //Winsow size
		ImVec2 winsow_size              = EditorConstant.WINDOW_PROJECT_MANAGER_SIZE;

        //Project manager window
        ImGui::SetNextWindowSize(winsow_size);
        //Begin window
		if(ImGui::BeginPopupModal(EditorConstant.WINDOW_PROJECT_MANAGER.c_str(), nullptr, window_flags))
        {
            //Save cursor position
            ImVec2 cursor = ImGui::GetCursorPos();

            //Panel 1 : Engine Banner
			ImGui::BeginChild("##project_manager_panel_1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.33f, winsow_size.y - 20.f));
				ImGui::Image(m_EditorTextureHolder->getTexture("editor_project_manager"));
            ImGui::EndChild();

            //Panel 2 : Window tabs
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            ImGui::SetCursorPosY(cursor.y);
            ImGui::BeginChild("##project_manager_panel_2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.67f, winsow_size.y * 0.85f));

				if (ImGui::BeginTabBar("##project_manager_tabbar"))
                {
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					if (ImGui::BeginTabItem(EditorConstant.TAB_RECENT_PROJECT.c_str(), nullptr, m_ProjectManagerTabBarSwitch.getTabStatus(EditorConstant.TAB_RECENT_PROJECT)))
					{
						showRecentProjectWindow();

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem(EditorConstant.TAB_CREATE_PROJECT.c_str(), nullptr, m_ProjectManagerTabBarSwitch.getTabStatus(EditorConstant.TAB_CREATE_PROJECT)))
					{
						showCreateProjectWindow();

                        ImGui::EndTabItem();
                    }

					if (ImGui::BeginTabItem(EditorConstant.TAB_OPEN_PROJECT.c_str(), nullptr, m_ProjectManagerTabBarSwitch.getTabStatus(EditorConstant.TAB_OPEN_PROJECT)))
					{
						showOpenProjectWindow();

                        ImGui::EndTabItem();
                    }

					if (ImGui::BeginTabItem(EditorConstant.TAB_WORKSPACE.c_str(), nullptr, m_ProjectManagerTabBarSwitch.getTabStatus(EditorConstant.TAB_WORKSPACE)))
					{
                        showWorkspaceWindow();

                        ImGui::EndTabItem();
					}

					m_ProjectManagerTabBarSwitch.resetSwith();

                    ImGui::EndTabBar();
                }

				//

            ImGui::EndChild();

			ImGui::SetCursorPosY(winsow_size.y - 38.f);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 4.0f));
			ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
            if (ImGui::Button("Close##close_project_manager_window", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

	void EditorInterface::showScriptCreationWindow()
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
							parameter.setString("level_name", std::string(m_SelectedGameLevel));
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

	void EditorInterface::showAboutEngineWindow()
	{
		//Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		//Winsow size
		ImVec2 winsow_size              = ImVec2(400.f, 250.f);

		//Project manager window
		ImGui::SetNextWindowSize(winsow_size);
		//Begin window
		if(ImGui::BeginPopupModal(EditorConstant.WINDOW_ABOUT_ENGINE.c_str(), nullptr, window_flags))
		{
			std::string aboutEngine = "Nero Game Engine d2.0.1 (Demo version 2.0.1)\n\n"
									  "Licence : BSD 4-Clause \"Original\" or \"Old\" License\n\n"
									  "Built on May 24 2020\n\n"
									  "Copyright 2016-2020 Sanou A. K. Landry. All rights reserved.\n\n"
									  "The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.";

			ImGui::TextWrapped("%s", aboutEngine.c_str());

			ImGui::SetCursorPosY(winsow_size.y - 38.f);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 4.0f));
			ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
			if (ImGui::Button("Close##close_about_engine", ImVec2(100, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void EditorInterface::createScriptObject(const Parameter& parameter)
	{
		if(m_GameProject)
		{
			m_GameProject->createScriptObject(parameter);
		}
	}


	void EditorInterface::showRecentProjectWindow()
	{
		ImGui::Text("Open a recent project and continuous where you left");
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 16.0f));

		//list of recent project

		ImVec2 button_sz(130, 100);

		float spacing = (ImGui::GetWindowContentRegionWidth() - 3 * 150)/2.f;

		ImGuiStyle& style = ImGui::GetStyle();
		int buttons_count = 6;
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		auto recentProjectTable =  file::loadJson(file::getPath({"setting", "recent_project"}));

		for (int i = 0; i < buttons_count; i++)
		{
			if(i < recentProjectTable.size())
			{
				auto project = recentProjectTable[recentProjectTable.size()-i-1];

				ImGui::Image(m_EditorTextureHolder->getTexture("recent_project_" + toString(i+1)));
				ImGui::SameLine(0.f, 5.f);
				std::string project_name =  string::wrapString(project["project_name"].get<std::string>(), 17);
				ImGui::PushID(i);

				if(ImGui::Button(project_name.c_str(), button_sz))
				{
					ImGui::CloseCurrentPopup();
					openProject(project["project_directory"].get<std::string>());
				}
				ImGui::PopID();
			}

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x + 20.f; // Expected position if next button was on same line
			if (i + 1 < buttons_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine(0.f, spacing);
			else {
				ImGui::Dummy(ImVec2(0.0f, spacing));
			}

		}

		ImGui::SetCursorPosY(EditorConstant.WINDOW_PROJECT_MANAGER_SIZE.y * 0.74f);

		//ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 450.f)/2.f);

		pushToolbarStyle();
		if(ImGui::Button("Create New Project", ImVec2(200.f, 40.f)))
		{
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_CREATE_PROJECT);
		}

		ImGui::SameLine(0.f, 50.f);

		if(ImGui::Button("Open Other Projects", ImVec2(200.f, 40.f)))
		{
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_OPEN_PROJECT);
		}
		popToolbarStyle();
	}

    void EditorInterface::showCreateProjectWindow()
	{
        //Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
										  ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse |
										  ImGuiWindowFlags_NoScrollbar;
        //Winsow size
		ImVec2 winsow_size              = EditorConstant.WINDOW_PROJECT_MANAGER_SIZE;

        float wording_width = 130.f;
        float input_width = ImGui::GetWindowContentRegionWidth() - 150.f;

		ImGui::Text("Create a new Project and start a new Adventure");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::BeginChild("project form", ImVec2(0.f, 0.f), true);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Project Name");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			ImGui::InputText("##project_name", m_ProjectInput.name, sizeof(m_ProjectInput.name));
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::Text("Startup Pack");
			ImGui::SameLine(wording_width);
			ImGui::Checkbox("##starter_pack", &m_ProjectInput.startupPack);
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::Text("Workspace");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);

			//load workpsace
			std::vector<std::string> workspaceNameTable = m_ProjectManager->getWorkspaceNameTable();

			std::size_t worskpaceCount = workspaceNameTable.size();

			const char* workspaceComboTable[worskpaceCount];
			nero_fill_char_array(workspaceComboTable, workspaceNameTable);

			m_ProjectInput.workspace = workspaceComboTable[m_ProjectInput.workspaceIndex];
			if (ImGui::BeginCombo("##workspace_combo", m_ProjectInput.workspace, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
			{
				for (std::size_t n = 0; n < worskpaceCount; n++)
				{
					bool is_selected = (m_ProjectInput.workspace == workspaceComboTable[n]);


					if (ImGui::Selectable(workspaceComboTable[n], is_selected))
					{
						m_ProjectInput.workspace = workspaceComboTable[n];
						m_ProjectInput.workspaceIndex = n;

						auto workspace = m_ProjectManager->findWorkspace(workspaceNameTable[n]);

						string::fillCharArray(m_ProjectInput.company,			sizeof(m_ProjectInput.company),				workspace["company_name"].get<std::string>());
						string::fillCharArray(m_ProjectInput.projectLead,		sizeof(m_ProjectInput.projectLead),			workspace["project_lead"].get<std::string>());
						string::fillCharArray(m_ProjectInput.projectNamespace,	sizeof(m_ProjectInput.projectNamespace),	workspace["project_namespace"].get<std::string>());
					}

					if (is_selected)
					{
						 ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Project Type");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			const char* projectTypeComboTable[] = {"CPP Project"}; //Demo //{"CPP Project", "Lua Project", "CPP and Lua Project"};
			m_ProjectInput.projectType = projectTypeComboTable[m_ProjectInput.projectTypeIndex];            // Here our selection is a single pointer stored outside the object.
			if (ImGui::BeginCombo("##project_type_combo", m_ProjectInput.projectType, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < IM_ARRAYSIZE(projectTypeComboTable); n++)
				{
					bool is_selected = (m_ProjectInput.projectType == projectTypeComboTable[n]);

					if (ImGui::Selectable(projectTypeComboTable[n], is_selected))
					{
						m_ProjectInput.projectType = projectTypeComboTable[n];
						m_ProjectInput.projectTypeIndex = n;
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Code Editor");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);

			std::vector<std::string> codeEditorTable;
			if(m_EditorSetting->getSetting("environment").getString("qt_creator") != StringPool.BLANK)
			{
				codeEditorTable.push_back("Qt Creator");
			}

			if(m_EditorSetting->getSetting("environment").getString("visual_studio") != StringPool.BLANK)
			{
				codeEditorTable.push_back("Visual Studio");
			}

			std::size_t editorCount = codeEditorTable.size();

			const char* codeEditorComboTable[editorCount];
			nero_fill_char_array(codeEditorComboTable, codeEditorTable);

			m_ProjectInput.codeEditor = codeEditorComboTable[m_ProjectInput.codeEditorIndex];
			if (ImGui::BeginCombo("##code-editor-combo", m_ProjectInput.codeEditor, ImGuiComboFlags()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(codeEditorComboTable); n++)
				{
					bool is_selected = (m_ProjectInput.codeEditor == codeEditorComboTable[n]);

					if (ImGui::Selectable(codeEditorComboTable[n], is_selected))
					{
						m_ProjectInput.codeEditor = codeEditorComboTable[n];
						m_ProjectInput.codeEditorIndex = n;
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Project Lead");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			ImGui::InputText("##project_lead", m_ProjectInput.projectLead, IM_ARRAYSIZE(m_ProjectInput.projectLead));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Company Name");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			ImGui::InputText("##project_company", m_ProjectInput.company, IM_ARRAYSIZE(m_ProjectInput.company));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Namesapce");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			ImGui::InputText("##project_namespace", m_ProjectInput.projectNamespace, IM_ARRAYSIZE(m_ProjectInput.projectNamespace));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));


			ImGui::Text("Description");
			ImGui::SameLine(wording_width);
			static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
			ImGui::InputTextMultiline("##project_description", m_ProjectInput.description, IM_ARRAYSIZE(m_ProjectInput.description), ImVec2(input_width, ImGui::GetTextLineHeight() * 5), flags);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
			ImGui::SetCursorPosY(winsow_size.y * 0.85f - 95.f);
			bool onCreate = ImGui::Button("Create", ImVec2(100, 0));

			if(onCreate)
			{
				m_ProjectInput.errorMessage = StringPool.BLANK;
				m_ProjectInput.redirectLink = StringPool.BLANK;
				m_ProjectInput.error = true;

				//Project name blank
				if(std::string(m_ProjectInput.name) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage = "Please enter a Project Name";
				}
				//Project name does not match regex
				else if(!string::matchPattern(std::string(m_ProjectInput.name), StringPool.REGEX_NAME_02))
				{
					m_ProjectInput.errorMessage = "Invalid Project Name";
					m_ProjectInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-project#project_name";
				}
				else if(m_ProjectManager->projectExist(std::string(m_ProjectInput.name), std::string(m_ProjectInput.workspace)))
				{
					m_ProjectInput.errorMessage = "A project with the same Id already exist, please choose another Project Name";
					m_ProjectInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-project#project_name";
				}
				//Worksapce blank
				else if(std::string(m_ProjectInput.workspace) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage = "Please select a Workspace";
				}
				//Project type blank
				else if(std::string(m_ProjectInput.projectType) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage = "Please select a Project Type";
				}
				//Code Editor blank
				else if(std::string(m_ProjectInput.codeEditor) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage = "Please select a Code Editor";
				}
				//workpspace company name blank
				else if(std::string(m_ProjectInput.company) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage	= "Please enter a Company Name";
					m_ProjectInput.redirectLink	= StringPool.BLANK;
				}
				//workpspace company not matching regex
				else if(!string::matchPattern(std::string(m_ProjectInput.company), StringPool.REGEX_NAME_01))
				{
					m_ProjectInput.errorMessage = "Invalid Company Name";
					m_ProjectInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-project#company_name";
				}
				//workpspace project lead name blank
				else if(std::string(m_ProjectInput.projectLead) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage	= "Please enter a Project Lead";
					m_ProjectInput.redirectLink	= StringPool.BLANK;
				}
				//workpspace project lead not matching regex
				else if(!string::matchPattern(std::string(m_ProjectInput.projectLead), StringPool.REGEX_NAME_01))
				{
					m_ProjectInput.errorMessage	= "Invalid Project Lead";
					m_ProjectInput.redirectLink	= "https://nero-game.com/learn/engine-v2/create-project#project_lead";
				}
				//workpspace namespace blank
				else if(std::string(m_ProjectInput.projectNamespace) == StringPool.BLANK)
				{
					m_ProjectInput.errorMessage	= "Please enter a Project Namesapce";
					m_ProjectInput.redirectLink	= StringPool.BLANK;
				}
				//workpspace namespace not matching regex
				else if(!string::matchPattern(std::string(m_ProjectInput.projectNamespace), StringPool.REGEX_NAMESPACE))
				{
					m_ProjectInput.errorMessage = "Invalid Project Namespace";
					m_ProjectInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-project#namespace";
				}
				else
				{
					m_ProjectInput.error = false;
				}
			}

			if (onCreate && m_ProjectInput.error)
			{
				ImGui::OpenPopup(EditorConstant.MODAL_ERROR_CREATING_PROJECT.c_str());
			}
			else if(onCreate)
			{
				Parameter parameter;
				parameter.setString("project_name",			std::string(m_ProjectInput.name));
				parameter.setString("workspace_name",		std::string(m_ProjectInput.workspace));
				parameter.setString("project_type",			std::string(m_ProjectInput.projectType));
				parameter.setString("project_namespace",	std::string(m_ProjectInput.projectNamespace));
				parameter.setString("project_lead",			std::string(m_ProjectInput.projectLead));
				parameter.setString("company_name",			std::string(m_ProjectInput.company));
				parameter.setString("description",			std::string(m_ProjectInput.description));
				parameter.setString("code_editor",			std::string(m_ProjectInput.codeEditor));
				parameter.setBool("startup_pack",			m_ProjectInput.startupPack);

				m_ProjectInput.lastCreatedProject = m_ProjectManager->getProjectDirectory(parameter);

				m_ProjectInput.clear();
				m_ProjectInput.update(m_ProjectManager->getWorkspaceTable());

				//start new thread
				createProject(parameter);

				ImGui::OpenPopup(EditorConstant.MODAL_WAITING_PROJECT_CREATION.c_str());
			}

			//Error modal
			ImGuiWindowFlags modal_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
											  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
			ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
			if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_CREATING_PROJECT.c_str(), nullptr, modal_flags))
			{
				ImGui::TextWrapped("%s", m_ProjectInput.errorMessage.c_str());

				if(m_ProjectInput.redirectLink != StringPool.BLANK)
				{
					ImGui::SetCursorPosY(130.f - 60.f);
					ImGui::SetCursorPosX((300.f - 150.f)/2.f);
					if(ImGui::Button("Learn more here", ImVec2(150.f, 0.f)))
					{
						cmd::launchBrowser(m_ProjectInput.redirectLink);
					}
				}

				ImGui::Dummy(ImVec2(0.0f, 35.0f));

				ImGui::SetCursorPosY(130.f - 30.f);
				ImGui::SetCursorPosX((300.f - 75.f)/2.f);
				if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::SetNextWindowSize(ImVec2(400.f, 200.f));
			if(ImGui::BeginPopupModal(EditorConstant.MODAL_WAITING_PROJECT_CREATION.c_str(), nullptr, window_flags))
			{
				std::string taskName = EditorConstant.TASK_CREATE_PROJECT + toString(m_CountCreateProject-1);
				BackgroundTask::Ptr createProjectTask = BTManager::findTaskByName(taskName);

				if(createProjectTask)
				{
					BTManager::pauseTask(taskName, 1);
					std::string message = createProjectTask->printMessage();

					ImGui::TextWrapped("%s", message.c_str());

					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					if(createProjectTask->getStatus() == 4)
					{
						ImGui::SetCursorPosY(200.f - 40.f);
						ImGui::SetCursorPosX(50.f);
						if (ImGui::Button("Open project", ImVec2(100, 0)))
						{
							ImGui::CloseCurrentPopup();
							ImGui::ClosePopupToLevel(0, true);

							openProject(m_ProjectInput.lastCreatedProject);
						}

						ImGui::SetCursorPosY(200.f - 40.f);
						ImGui::SetCursorPosX(250.f);
						if (ImGui::Button("Close", ImVec2(100, 0)))
						{
							ImGui::CloseCurrentPopup();
						}
					}
				}

				ImGui::EndPopup();
			 }

		ImGui::EndChild();
    }

	void EditorInterface::showOpenProjectWindow()
    {
		float wording_width = 200.f;
		float input_width	= ImGui::GetWindowContentRegionWidth() - 150.f;

		ImGui::Text("Open a Project and continous where you left");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::BeginChild("project list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

            ImGui::Dummy(ImVec2(0.f, 10.f));

            for(const std::string workspace : m_ProjectManager->getWorkspaceNameTable())
            {
				if (ImGui::CollapsingHeader(workspace.c_str()))
                {
                    ImGui::SetCursorPosX(20.f);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    for(const nlohmann::json& project : m_ProjectManager->getWorkspaceProjectTable(workspace))
                    {
                        std::string project_name = project["project_name"].get<std::string>();

                        ImGui::SetCursorPosX(20.f);
                        if (ImGui::CollapsingHeader(project_name.c_str()))
                        {
                            std::string project_lead        = ": " + project["project_lead"].get<std::string>();
							std::string project_company     = ": " + project["company_name"].get<std::string>();
							std::string project_description = project["description"].get<std::string>();
							std::string project_type		= ": " + project["project_type"].get<std::string>();
							std::string code_editor			= ": " + project["code_editor"].get<std::string>();
							std::string project_namespace	= ": " + project["project_namespace"].get<std::string>();
							std::string creation_date       = ": " + project["creation_date"].get<std::string>();
							std::string modification_date   = ": " + project["modification_date"].get<std::string>();

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Company");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(project_company.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

							ImGui::SetCursorPosX(40.f);
							ImGui::Text("Project Lead");
							ImGui::SameLine(wording_width);
							ImGui::Text(project_lead.c_str());
							ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Create Date");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(creation_date.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Modification Date");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(modification_date.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Description");
							ImGui::SetCursorPosX(40.f);
							ImGui::BeginChild(project_name.c_str(), ImVec2(0.f, 100.f), true, ImGuiWindowFlags_ScrollToolbar);
								ImGui::TextWrapped("%s", project_description.c_str());
							ImGui::EndChild();
                            ImGui::Dummy(ImVec2(0.0f, 5.0f));


							ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 130.f);
							pushToolbarStyle();
							if (ImGui::Button("Open Project", ImVec2(125, 30.f)))
                            {
								openProject(file::getPath({m_ProjectManager->findWorkspace(workspace)["workspace_directory"].get<std::string>(), "Project", project_name}));

                                ImGui::CloseCurrentPopup();
                            }
							popToolbarStyle();

                            ImGui::Dummy(ImVec2(0.0f, 10.0f));

                        }
                    }
                 }

                ImGui::Dummy(ImVec2(0.f, 15.f));
            }

         ImGui::EndChild();
    }

	void EditorInterface::selectFile(std::function<void(std::string)> callback)
	{
		nfdchar_t *outPath = nullptr;
		nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);

		if (result == NFD_OKAY)
		{
			callback(std::string(outPath));

			free(outPath);
		}
		else if (result == NFD_CANCEL)
		{
			nero_log("select file canceled");
		}
		else
		{
		   nero_log("failed to select file : " + nero_s(NFD_GetError()));
		}
	}

	void EditorInterface::selectFile(std::function<void(std::vector<std::string>)> callback)
	{
		nfdpathset_t pathSet;
		nfdresult_t result = NFD_OpenDialogMultiple( nullptr, nullptr, &pathSet);

		if (result == NFD_OKAY)
		{
			std::vector<std::string> fileTable;

			size_t i;
			for ( i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i )
			{
				nfdchar_t *path = NFD_PathSet_GetPath(&pathSet, i);

				fileTable.push_back(toString(path));
			}

			callback(fileTable);

			NFD_PathSet_Free(&pathSet);
		}
		else if (result == NFD_CANCEL)
		{
			nero_log("select files canceled");
		}
		else
		{
			nero_log("failed to select files : " + nero_s(NFD_GetError()));
		}
	}

	void EditorInterface::selectDirectory(std::function<void(std::string)> callback)
	{
		nfdchar_t *outPath = nullptr;
		nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

		if (result == NFD_OKAY)
		{
			callback(std::string(outPath));

			free(outPath);
		}
		else if (result == NFD_CANCEL)
		{
			nero_log("select directory canceled");
		}
		else
		{
		   nero_log("failed to select directory : " + nero_s(NFD_GetError()));
		}
	}

	//////////////Workspace
	void EditorInterface::createWorkspace(const Parameter& parameter)
	{
		m_ProjectManager->createWorkspace(parameter);
		m_ProjectInput.update(m_ProjectManager->getWorkspaceTable());
	}

	void EditorInterface::importWorkspace(const std::string& directory)
	{
		m_ProjectManager->importWorkspace(directory);
		m_ProjectInput.update(m_ProjectManager->getWorkspaceTable());
	}

	void EditorInterface::showCreateWorkspace()
	{
		float wording_width = 150.f;
		float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

		//create workspace
		ImGui::Text("Location");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width - 65.f);
		ImGui::InputText("##workspace_location", m_WorkspaceInput.location, sizeof(m_WorkspaceInput.location), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine(wording_width + input_width - 60.f);
		if(ImGui::Button("Browse##choose_workspace_location", ImVec2(60, 0)))
		{
			selectDirectory([this](std::string outPath)
			{
				string::fillCharArray(m_WorkspaceInput.location, sizeof(m_WorkspaceInput.location), outPath);
			});
		}
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Workspace Name");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_name", m_WorkspaceInput.name, sizeof( m_WorkspaceInput.name));
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Company Name");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_company",  m_WorkspaceInput.company, sizeof( m_WorkspaceInput.company));
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Project Lead");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_lead",  m_WorkspaceInput.projectLead, sizeof( m_WorkspaceInput.projectLead));
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Project Namespace");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_namespace",  m_WorkspaceInput.projectNamespace, sizeof( m_WorkspaceInput.projectNamespace));
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::SameLine(wording_width + input_width - 130.f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);

		bool onCreate = ImGui::Button("Create Workspace", ImVec2(130.f, 0));

		if(onCreate)
		{
			m_WorkspaceInput.errorMessage = StringPool.BLANK;
			m_WorkspaceInput.redirectLink = StringPool.BLANK;
			m_WorkspaceInput.error = true;

			//workpspace location blank
			if(std::string(m_WorkspaceInput.location) == StringPool.BLANK)
			{
				m_WorkspaceInput.errorMessage   = "Please select a Location Directory";
				m_WorkspaceInput.redirectLink   = StringPool.BLANK;
			}
			//workpspace location not valid
			else if(!file::directoryExist(std::string(m_WorkspaceInput.location)))
			{
				m_WorkspaceInput.errorMessage   = "The selected location is not a valid directory";
				m_WorkspaceInput.redirectLink   = StringPool.BLANK;
			}
			//workpspace name blank
			else if(std::string(m_WorkspaceInput.name) == StringPool.BLANK)
			{
				m_WorkspaceInput.errorMessage   = "Please enter a Workspace Name";
				m_WorkspaceInput.redirectLink   = StringPool.BLANK;
			}
			//workpspace name not matching regex
			else if(!string::matchPattern(std::string(m_WorkspaceInput.name), StringPool.REGEX_NAME_01))
			{
				m_WorkspaceInput.errorMessage = "Invalid Workspace Name";
				m_WorkspaceInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-workspace#workspace_name";
			}
			//workpspace already exist
			else if(m_ProjectManager->workspaceExist(m_WorkspaceInput.name))
			{
				m_WorkspaceInput.errorMessage   = "A Workspace with the same Id already exist, Please choose another Name";
				m_WorkspaceInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-workspace#workspace_name";
			}
			//workpspace company name blank
			else if(std::string(m_WorkspaceInput.company) == StringPool.BLANK)
			{
				m_WorkspaceInput.errorMessage   = "Please enter a Company Name";
				m_WorkspaceInput.redirectLink   = StringPool.BLANK;
			}
			//workpspace company not matching regex
			else if(!string::matchPattern(std::string(m_WorkspaceInput.company), StringPool.REGEX_NAME_01))
			{
				m_WorkspaceInput.errorMessage = "Invalid Company Name";
				m_WorkspaceInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-workspace#company_name";
			}
			//workpspace project lead name blank
			else if(std::string(m_WorkspaceInput.projectLead) == StringPool.BLANK)
			{
				m_WorkspaceInput.errorMessage   = "Please enter a Project Lead";
				m_WorkspaceInput.redirectLink   = StringPool.BLANK;
			}
			//workpspace project lead not matching regex
			else if(!string::matchPattern(std::string(m_WorkspaceInput.projectLead), StringPool.REGEX_NAME_01))
			{
				m_WorkspaceInput.errorMessage   = "Invalid Project Lead";
				m_WorkspaceInput.redirectLink   = "https://nero-game.com/learn/engine-v2/create-workspace#project_lead";
			}
			//workpspace namespace blank
			else if(std::string(m_WorkspaceInput.projectNamespace) == StringPool.BLANK)
			{
				m_WorkspaceInput.errorMessage   = "Please enter a Project Namesapce";
				m_WorkspaceInput.redirectLink   = StringPool.BLANK;
			}
			//workpspace namespace not matching regex
			else if(!string::matchPattern(std::string(m_WorkspaceInput.projectNamespace), StringPool.REGEX_NAMESPACE))
			{
				m_WorkspaceInput.errorMessage = "Invalid Project Namespace";
				m_WorkspaceInput.redirectLink = "https://nero-game.com/learn/engine-v2/create-workspace#namespace";
			}
			else
			{
				m_WorkspaceInput.error = false;
			}
		}

		if (onCreate && m_WorkspaceInput.error)
		{
			ImGui::OpenPopup(EditorConstant.MODAL_ERROR_CREATING_WORKSPACE.c_str());
		}
		else if(onCreate)
		{
			Parameter parameter;

			parameter.setString("workspace_location",	std::string(m_WorkspaceInput.location));
			parameter.setString("workspace_name",		std::string(m_WorkspaceInput.name));
			parameter.setString("company_name",			std::string(m_WorkspaceInput.company));
			parameter.setString("project_lead",			std::string(m_WorkspaceInput.projectLead));
			parameter.setString("project_namespace",	std::string(m_WorkspaceInput.projectNamespace));

			m_WorkspaceInput.clear();

			createWorkspace(parameter);
		}

		ImGuiWindowFlags modal_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
		if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_CREATING_WORKSPACE.c_str(), nullptr, modal_flags))
		{
			ImGui::TextWrapped("%s", m_WorkspaceInput.errorMessage.c_str());

			if(m_WorkspaceInput.redirectLink != StringPool.BLANK)
			{
				ImGui::Dummy(ImVec2(0.f, 5.f));

				ImGui::SetCursorPosX((300.f - 150.f)/2.f);
				if(ImGui::Button("Learn more here", ImVec2(150.f, 0.f)))
				{
					cmd::launchBrowser(m_WorkspaceInput.redirectLink);
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 35.0f));

			ImGui::SetCursorPosY(130.f - 30.f);
			ImGui::SetCursorPosX((300.f - 75.f)/2.f);
			if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void EditorInterface::showImportWorkspace()
	{
		float wording_width = 150.f;
		float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

		//import workspace
		ImGui::Text("Workspace Directory");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width - 130.f);
		ImGui::InputText("##workspace_import", m_WorkspaceInput.locationImport, sizeof(m_WorkspaceInput.locationImport), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine(wording_width + input_width - 124.f);

		if(ImGui::Button("Browse##choose_workspace_import", ImVec2(60.f, 0)))
		{
			selectDirectory([this](std::string outPath)
			{
				string::fillCharArray(m_WorkspaceInput.locationImport, sizeof(m_WorkspaceInput.locationImport), outPath);
			});
		}

		ImGui::SameLine(wording_width + input_width - 60.f);
		bool onImport				= ImGui::Button("Import##import_workspace", ImVec2(60.f, 0));

		if(onImport)
		{
			m_WorkspaceInput.errorMessage = StringPool.BLANK;
			m_WorkspaceInput.redirectLink = StringPool.BLANK;
			m_WorkspaceInput.error = true;

			//workpspace location blank
			if(std::string(m_WorkspaceInput.locationImport) == StringPool.BLANK)
			{
				m_WorkspaceInput.errorMessage = "Please select a Workspace Location";
			}
			//workpspace location not valid
			else if(!file::directoryExist(std::string(m_WorkspaceInput.locationImport)))
			{
				m_WorkspaceInput.errorMessage	= "The selected location is not a valid directory";
			}
			//workpspace already exist
			else if(m_ProjectManager->workspaceExist(file::getFileName(std::string(m_WorkspaceInput.locationImport))))
			{
				m_WorkspaceInput.errorMessage	= "A Workspace with the same Id already exist, Please choose another Name";
			}
			//workpspace location not valid
			else if(!file::fileExist(file::getPath({std::string(m_WorkspaceInput.locationImport), ".workspace"})))
			{
				m_WorkspaceInput.errorMessage = "The selected directory is not a valid Workspace";
			}
			else
			{
				m_WorkspaceInput.error = false;
			}
		}

		if (onImport && m_WorkspaceInput.error)
		{
			ImGui::OpenPopup(EditorConstant.MODAL_ERROR_IMPORTING_WORKSPACE.c_str());
		}
		else if(onImport)
		{
			//import workspace
			importWorkspace(std::string(m_WorkspaceInput.locationImport));
			//clear input
			string::fillCharArray(m_WorkspaceInput.locationImport, sizeof(m_WorkspaceInput.locationImport), StringPool.BLANK);
		}

		ImGuiWindowFlags modal_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
		if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_IMPORTING_WORKSPACE.c_str(), nullptr, modal_flags))
		{
			ImGui::TextWrapped("%s", m_WorkspaceInput.errorMessage.c_str());

			if(m_WorkspaceInput.redirectLink != StringPool.BLANK)
			{
				ImGui::Dummy(ImVec2(0.f, 5.f));

				ImGui::SetCursorPosX((300.f - 150.f)/2.f);
				if(ImGui::Button("Learn more here", ImVec2(150.f, 0.f)))
				{
					cmd::launchBrowser(m_WorkspaceInput.redirectLink);
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 35.0f));

			ImGui::SetCursorPosY(130.f - 30.f);
			ImGui::SetCursorPosX((300.f - 75.f)/2.f);
			if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void EditorInterface::showWorkspaceList()
	{
		float wording_width = 150.f;
		float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

		ImGuiWindowFlags window_flag = ImGuiWindowFlags_None;
		window_flag |= ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		ImGui::BeginChild("##workspace_list", ImVec2(0.f, 0.f), true, window_flag);
		ImGui::Dummy(ImVec2(0.0f, 4.0f));


			for(const nlohmann::json& worksapce : m_ProjectManager->getWorkspaceTable())
			{
				if (ImGui::CollapsingHeader(worksapce["workspace_name"].get<std::string>().c_str()))
				{
					std::string project_lead		= ": " + worksapce["project_lead"].get<std::string>();
					std::string company_name		= ": " + worksapce["company_name"].get<std::string>();
					std::string project_namespace	= ": " + worksapce["project_namespace"].get<std::string>();
					std::string workspace_directory	= ": " + worksapce["workspace_directory"].get<std::string>();

					ImGui::Text("Workspace Directory");
					ImGui::SameLine(wording_width);
					ImGui::Text(workspace_directory.c_str());
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::Text("Company Name");
					ImGui::SameLine(wording_width);
					ImGui::Text(company_name.c_str());
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::Text("Project Lead");
					ImGui::SameLine(wording_width);
					ImGui::Text(project_lead.c_str());
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::Text("Project Namespace");
					ImGui::SameLine(wording_width);
					ImGui::Text(project_namespace.c_str());
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

				}

			}

		ImGui::EndChild();
	}

    void EditorInterface::showWorkspaceWindow()
    {

		showCreateWorkspace();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

		showImportWorkspace();

		ImGui::Dummy(ImVec2(0.0f, 10.0f));


		showWorkspaceList();

	}


	void EditorInterface::createProject(const Parameter& parameter)
	{
		nero_log("on create project in background");
		std::string taskName = EditorConstant.TASK_CREATE_PROJECT + toString(m_CountCreateProject++);
		BTManager::startTask(&ProjectManager::createProject, m_ProjectManager.get(), parameter, taskName);
	}

	void EditorInterface::openProject(const std::string& projectDirectory)
    {
		//close currently open project
		if(m_GameProject)
		{
			closeProject();
		}

		//open new project
		m_GameProject		= m_ProjectManager->openProject(projectDirectory);
		m_AdvancedScene		= m_GameProject->getAdvancedScene();

		//update editor window title
		updateWindowTitle(m_GameProject->getProjectName());
    }

	void EditorInterface::closeProject()
	{
		m_ResourceManager	= nullptr;
		m_AdvancedScene		= nullptr;
		m_GameProject		= nullptr;
		m_ProjectManager->closeProject();


		//update editor window title
		updateWindowTitle(EngineConstant.ENGINE_WINDOW_TITLE);
	}

	void EditorInterface::playScene()
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

	void EditorInterface::pauseScene()
	{
		/*if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

			gameLevel->levelSetting->setBool("pause_level", !gameLevel->levelSetting->getBool("pause_level"));
		}*/
	}

	void EditorInterface::stepScene()
	{

		/*if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

			gameLevel->levelSetting->setBool("pause_scene", true);
			gameLevel->levelSetting->setBool("single_step", true);
		}*/
	}

	void EditorInterface::resetScene()
	{
		/*if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			m_AdvancedScene->playScene();
		}*/
	}

	void EditorInterface::renderScene()
	{

	}

	void EditorInterface::compileProject()
	{
		if(m_GameProject)
		{
			BTManager::startTask(&GameProject::compileProject, m_GameProject.get());
		}
	}

	void EditorInterface::editProject()
	{
		if(m_GameProject)
		{
			m_GameProject->openEditor();

		}
	}

	void EditorInterface::reloadProject()
	{

		if(m_GameProject)
		{
			nero_log("reloading project ...");

			//m_GameProject->loadLibrary();

		}
	}



	sf::Sprite EditorInterface::flipTexture(const sf::Texture& texture)
	{
		sf::Vector2u size = texture.getSize();
		sf::Sprite sprite(texture, sf::IntRect(0, size.y, size.x, -size.y));

		return sprite;
	}

	void EditorInterface::showLoggingWindow()
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

	void EditorInterface::showConsoleWindow()
	{
		m_ConsoleApplication.Draw(EditorConstant.WINDOW_CONSOLE.c_str(), nullptr);
	}

	ImVec4 EditorInterface::getLoggingColor(logging::LEVEL level)
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

	void EditorInterface::showResourceWindow()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin("Resource", nullptr, flags);

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


		if(ImGui::Button("Sprite##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Texture;
		}

		printSameLine();

		if(ImGui::Button("Animation##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Animation;
		}

		printSameLine();


		if(ImGui::Button("Mesh##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Mesh;
		}

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

		if(ImGui::Button("Font##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Font;
		}

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

	void EditorInterface::showResourceBrowserWindow()
	{
		if(!m_ResourceManager) { return;}

		//project manager window
		if(m_ResourceBrowserType != ResourceType::None)
		{
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
	}

	void EditorInterface::saveResourceFile(ResourceType type, const std::vector<std::string> loadedFileTable)
	{
		switch (m_ResourceBrowserType)
		{
			case ResourceType::Texture:
			{
				//copy texture file and json helper
				for(std::string file : loadedFileTable)
				{
					file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "texture", file::getFileName(file, true)}));

					std::string jsonHelper  = file::replaceExtension(file, "json");
					if(file::fileExist(jsonHelper))
					{
						file::copyFile(file, file::getPath({m_GameProject->getResourceFoler(), "texture", file::getFileName(jsonHelper, true)}));
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

	void EditorInterface::showMeshResource()
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
			/*if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				m_AdvancedScene->addObject(Object::Mesh_Object, "Polygon", getAddObjectPosition(), m_EditorMode);*/
		}

		printSameLine();

		if(ImGui::Button("Circle", ImVec2(100.f, 100.f)))
		{
			/*if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				m_AdvancedScene->addObject(Object::Mesh_Object, "Circle", getAddObjectPosition(), m_EditorMode);*/
		}

		printSameLine();

		if(ImGui::Button("Line", ImVec2(100.f, 100.f)))
		{
			/*if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				m_AdvancedScene->addObject(Object::Mesh_Object, "Line", getAddObjectPosition(), m_EditorMode);*/
		}
	}

	sf::Vector2f EditorInterface::getAddObjectPosition()
	{
		//sf::Vector2f screen_pos    = sf::Vector2f(mouse.x - m_RenderContext->canvas_position.x, mouse.y - m_RenderContext->canvas_position.y);
		//sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(screen_pos.x, screen_pos.y), m_RenderTexture->getView());

		sf::Vector2f screen_pos     = sf::Vector2f(m_RenderContext->canvas_size.x / 2.f, 150.f);
		sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(sf::Vector2i(screen_pos.x, screen_pos.y), m_RenderTexture->getView());

		return world_pos;
	}

	void EditorInterface::showSpriteResource()
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
				/*if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
					m_AdvancedScene->addObject(Object::Sprite_Object, spriteTable[n], getAddObjectPosition(), m_EditorMode);*/
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

	void EditorInterface::showLightmapResource()
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

	void EditorInterface::showAnimationResource()
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

	void EditorInterface::showFontResource()
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

	sf::Texture& EditorInterface::getFontTexture(const std::string& fontName)
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

	void EditorInterface::showUtilityWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_UTILITY.c_str());

			ImVec2 size = ImGui::GetWindowSize();

			ImGui::BeginChild("scene_mode", ImVec2(0.f, 105.f), true);
				ImGui::Text("Choose Scene Mode");
				ImGui::Separator();

				static int e = 0;
				ImGui::RadioButton("Object", &e, 0);
				ImGui::RadioButton("Mesh", &e, 1);
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
					cmd::launchBrowser("https://nero-game.com/learn/engine-v2");
				 }

				ImGui::SameLine();

				 if(ImGui::Button("Forum", button_size))
				 {
					 cmd::launchBrowser("https://nero-game.com/forum");
				 }


				 if(ImGui::Button("Snippet", button_size))
				 {
					 cmd::launchBrowser("https://nero-game.com/snippet/engine-v2");
				 }

				 ImGui::SameLine();

				 if(ImGui::Button("API", button_size))
				 {
					 cmd::launchBrowser("https://nero-game.com/learn/engine-v2/api");
				 }

			ImGui::EndChild();

		ImGui::End();
	}

	void EditorInterface::onSaveProject()
	{
		if(m_GameProject)
		{
			switch (m_EditorMode)
			{
				case EditorMode::WORLD_BUILDER:
				{
					//m_GameProject->saveGameLevel();
				}break;

				case EditorMode::SCREEN_BUILDER:
				{
					//m_GameProject->saveGameScreen();
				}break;
			}
		}
	}

	void EditorInterface::onLoadProject()
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

	void EditorInterface::autoSaveProject()
	{
		/*if(m_GameProject && m_AutoSaveClock.getElapsedTime() > sf::seconds(m_EditorSetting->getSetting("editor").getUInt("auto_save_interval")))
		{
			m_GameProject->saveProject();
			m_AutoSaveClock.restart();
		}*/
	}

	void EditorInterface::showGameScreenWindow()
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

	void EditorInterface::removeGameScreen()
	{

	}



	void EditorInterface::showObjectLayerWindow()
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

				/*if(m_AdvancedScene)
				{
					auto sceneBuilder = m_AdvancedScene->getSelectedSceneBuilder(m_EditorMode);

					m_InputSelectedObjectLayerId = sceneBuilder->getSelectedLayer()->getObjectId();

					for(const auto& objectLayer : sceneBuilder->getLayerTable())
					{
						std::string itemId = "##select_layer" + toString(objectLayer->getObjectId());
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedObjectLayerId, objectLayer->getObjectId());

						if(ImGui::IsItemClicked())
						{
							sceneBuilder->setSelectedLayer(objectLayer);
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
				}*/

			ImGui::EndChild();

		ImGui::End();
	}

	std::tuple<ImVec4, ImVec4> EditorInterface::getLayerColor(Object::Type type)
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

	void EditorInterface::addObjectLayer()
	{
		/*if(m_AdvancedScene)
		{
			m_AdvancedScene->getSelectedSceneBuilder(m_EditorMode)->addLayer();
		}*/
	}

	void EditorInterface::removeObjectLayer()
	{
		if(m_AdvancedScene)
		{

		}
	}

	void EditorInterface::showGameLevelWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_LEVEL.c_str());

			ImVec2 button_size = ImVec2(100.f, 0.f);
			if(ImGui::Button("Open##open_game_level", button_size))
			{

			}

			ImGui::SameLine();

			if(ImGui::Button("Close##close_game_level", button_size))
			{

			}

			ImGui::SameLine();

			if(ImGui::Button("Close##close_game_level", button_size))
			{

			}

			ImGui::SameLine();


			if(ImGui::Button("Copy##close_game_level", button_size))
			{

			}


			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 115.f);

			if(ImGui::Button("Delete##delete_game_level", button_size))
			{

			}

			ImGui::Dummy(ImVec2(0.f, 5.f));

			ImGui::BeginChild("##show_game_level", ImVec2(), true);

				auto levelNameTable = m_AdvancedScene->getGameLevelNameTable();
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

					pushToolbarStyle(m_GameLevelBuilder->getLevelName() == name);
					if(ImGui::Button(name.c_str(), button_size))
					{
						m_GameLevelBuilder = m_AdvancedScene->selectLevelBuilder(name);
					}

					popToolbarStyle();

					printSameLine();
				}

			ImGui::EndChild();

			//showNewGameLevelPopup();

		ImGui::End();
	};

	void EditorInterface::showNewGameLevelPopup()
	{
		//Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		//Winsow size
		ImVec2 winsow_size = ImVec2(400.f, 200.f);
		ImGui::SetNextWindowSize(winsow_size);

		//Begin window
		if(ImGui::BeginPopupModal("Create Game Level", nullptr, window_flags))
		{
			float wording_width = 130.f;
			float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;

			ImGui::Text("Level Name");
			ImGui::SameLine(wording_width);
			//ImGui::SetNextItemWidth(input_width - 30.f);
			ImGui::InputText("##new_level_name", m_NewGameLevelInput.name, sizeof(m_NewGameLevelInput.name));
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::Text("Enable Physics");
			ImGui::SameLine(wording_width);
			ImGui::Checkbox("##new_level_physics", &m_NewGameLevelInput.enablePhysics);
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::Text("Enable Light");
			ImGui::SameLine(wording_width);
			ImGui::Checkbox("##new_level_light", &m_NewGameLevelInput.enableLight);
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::Text("Template");
			ImGui::SameLine(wording_width);
			const char* items[] = { "None"};
			static int item_current = 0;
			ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::Dummy(ImVec2(0.0f, 1.0f));

			ImGui::SetCursorPosY(winsow_size.y - 50.f);

			if (ImGui::Button("Close##clow_new_level", ImVec2(100, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 95.f);

			//ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
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
					parameter.setString("level_name", std::string(m_NewGameLevelInput.name));
					parameter.setBool("enable_physics", m_NewGameLevelInput.enablePhysics);
					parameter.setBool("enable_light", m_NewGameLevelInput.enableLight);
					parameter.setString("template", "None");

					createGameLevel(parameter);

					ImGui::CloseCurrentPopup();
				}
			}

			/*ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 4.0f));
			ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
			if (ImGui::Button("Close##clow_new_level", ImVec2(100, 0)))
			{
				ImGui::CloseCurrentPopup();
			}*/

			ImGui::EndPopup();
		}
	}

	void EditorInterface::createGameLevel(const Parameter& parameter)
	{
		if(m_AdvancedScene)
		{
			m_GameLevelBuilder =  m_AdvancedScene->addGameLevel(parameter);
		}
	}

	void EditorInterface::createGameScreen(const Parameter& parameter)
	{
		if(m_AdvancedScene)
		{
			m_AdvancedScene->addGameScreen(parameter);
		}
	}

	void EditorInterface::showNewGameScreenPopup()
	{
		//Window flags
		ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
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
		}
	}

	void EditorInterface::removeGameLevel()
	{
		if(m_AdvancedScene)
		{

		}
	}

	void EditorInterface::editGameLevel()
	{
		if(m_AdvancedScene)
		{

		}
	}

	void EditorInterface::showWorldChunckWindow()
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

				/*if(m_AdvancedScene)
				{
					auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

					m_InputSelectedWorldChunkId = m_AdvancedScene->getSelectedWorldChunk()->chunkId;

					for(const auto& worldChunk : gameLevel->chunkTable)
					{
						std::string itemId = "##select_chunk" + toString(worldChunk->chunkId);
						ImGui::RadioButton(itemId.c_str(), &m_InputSelectedWorldChunkId, worldChunk->chunkId);

						if(ImGui::IsItemClicked())
						{
							m_AdvancedScene->setSelectedWorldChunk(worldChunk);
						}

						ImGui::SameLine();

						itemId = "##visible_chunk" + toString(worldChunk->chunkId);
						ImGui::Checkbox(itemId.c_str(), &worldChunk->visible);

						ImGui::SameLine();

						char chunk_name[100];
						string::fillCharArray(chunk_name, sizeof(chunk_name), worldChunk->name);
						ImGui::SetNextItemWidth(118.f);
						itemId = "##chunk_name" + toString(worldChunk->chunkId);
						ImGui::InputText(itemId.c_str(), chunk_name, sizeof(chunk_name));

						if(ImGui::IsItemEdited())
						{
							worldChunk->name = std::string(chunk_name);
						}
					}
				}*/

			ImGui::EndChild();

		ImGui::End();
	}

	void EditorInterface::addWorldChunk()
	{
		/*if(m_AdvancedScene && m_EditorMode == EditorMode::WORLD_BUILDER && m_BuilderMode == BuilderMode::OBJECT)
		{
			m_AdvancedScene->addWorldChunk();
		}*/
	}

	void EditorInterface::removeWorldChunk()
	{
		if(m_AdvancedScene)
		{
			//m_AdvancedScene->removeWorldChunk();
		}
	}

	void EditorInterface::showToggleButton(bool toggle, const std::string& label, std::function<void()> callback)
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

	void EditorInterface::interfaceFirstDraw()
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

	void EditorInterface::showHelpWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_HELP.c_str());

		ImGui::End();
	}


	void EditorInterface::showExplorerWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_EXPLORER.c_str());

		if (ImGui::CollapsingHeader("Game World", m_AdvancedScene ? ImGuiTreeNodeFlags_DefaultOpen :ImGuiTreeNodeFlags_None))
		{
			/*if(m_AdvancedScene)
			{
				auto gameLevel = m_AdvancedScene->getSelectedGameLevel();


				ImGuiViewport* viewport = ImGui::GetMainViewport();
				float game_world_window_height = viewport->Size.y * 0.25f;
				viewport = nullptr;

				ImGui::BeginChild("game_world", ImVec2(0.f, game_world_window_height), true);

				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()*1.5);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if(ImGui::TreeNode(std::string("[Level] " + gameLevel->name).c_str()))
				{
					int			chunk_node_clicked		= -1;
					static int	chunk_selection_mask	= (1 << gameLevel->chunkTable.size());
					int			selectedWorldChunkId	= m_AdvancedScene->getSelectedWorldChunk()->chunkId;

					int loop_chunk = 0;
					for(const auto& worldChunk : gameLevel->chunkTable)
					{
						ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

						if (chunk_selection_mask & (1 << loop_chunk))
						{
							node_flags |= ImGuiTreeNodeFlags_Selected;
						}

						bool chunk_node_open = ImGui::TreeNodeEx((void*)(intptr_t)loop_chunk, node_flags, std::string("[Chunk] " + worldChunk->name).c_str(), loop_chunk);

						if(worldChunk->chunkId == selectedWorldChunkId)
						{
							chunk_node_clicked = loop_chunk;
						}
						if (ImGui::IsItemClicked())
						{
							chunk_node_clicked = loop_chunk;
							m_AdvancedScene->setSelectedWorldChunk(worldChunk);
							selectedWorldChunkId = worldChunk->chunkId;
						}

						if (chunk_node_open)
						{
							//display chunk layer here
							int			layer_node_clicked		= -1;
							static int	layer_selection_mask	= (1 << worldChunk->sceneBuilder->getLayerTable().size());
							int			selectedObjectLayerId	= m_AdvancedScene->getSelectedSceneBuilder(m_EditorMode)->getSelectedLayer()->getObjectId();

							int loop_layer = 0;
							for(const auto& objectLayer : worldChunk->sceneBuilder->getLayerTable())
							{
								ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

								if ((layer_selection_mask & (1 << loop_layer)) && (chunk_selection_mask & (1 << loop_chunk)))
								{
									node_flags |= ImGuiTreeNodeFlags_Selected;
								}


								bool layer_node_open = ImGui::TreeNodeEx((void*)(intptr_t)loop_layer, node_flags, std::string("[Layer] " + objectLayer->getName()).c_str(), loop_layer);

								if(objectLayer->getObjectId() == selectedObjectLayerId && worldChunk->chunkId == selectedWorldChunkId)
								{
									layer_node_clicked = loop_layer;
									chunk_node_clicked = loop_chunk;
								}
								else if(ImGui::IsItemClicked())
								{
									layer_node_clicked = loop_layer;
									chunk_node_clicked = loop_chunk;
									m_AdvancedScene->setSelectedWorldChunk(worldChunk);
									worldChunk->sceneBuilder->setSelectedLayer(objectLayer);
									selectedWorldChunkId = worldChunk->chunkId;
									selectedObjectLayerId = objectLayer->getObjectId();
								}

								if (layer_node_open)
								{
									int			object_node_clicked		= -1;
									static int	object_selection_mask	= (1 << objectLayer->getChildCount());
									int			selectedGameObjectId	= -1;

									if(worldChunk->sceneBuilder->getSelectedObject())
									{
										selectedGameObjectId = worldChunk->sceneBuilder->getSelectedObject()->getObjectId();
									}

									int loop_object = 0;
									for(const auto& gameObject : *objectLayer->getAllChild())
									{
										ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
										node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

										if ((object_selection_mask & (1 << loop_object)) && (layer_selection_mask & (1 << loop_layer)) && chunk_selection_mask & (1 << loop_chunk))
										{
											node_flags |= ImGuiTreeNodeFlags_Selected;
										}

										std::string object_name = std::string(gameObject->getName());

										ImGui::TreeNodeEx((void*)(intptr_t)loop_object, node_flags, object_name.c_str(), loop_object);

										if(gameObject->getObjectId() == selectedGameObjectId && objectLayer->getObjectId() == selectedObjectLayerId && worldChunk->chunkId == selectedWorldChunkId)
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

											m_AdvancedScene->setSelectedWorldChunk(worldChunk);
											worldChunk->sceneBuilder->setSelectedLayer(objectLayer);
											worldChunk->sceneBuilder->setSelectedObject(gameObject);

											selectedWorldChunkId	= worldChunk->chunkId;
											selectedObjectLayerId	= objectLayer->getObjectId();
											selectedGameObjectId	= gameObject->getObjectId();
										}

										loop_object++;
									}

									if (object_node_clicked != -1)
									{
										object_selection_mask = (1 << object_node_clicked);

										//f (ImGui::GetIO().KeyCtrl)
										//{
											//object_selection_mask ^= (1 << object_node_clicked);
										//}
										//else
										//{
											//object_selection_mask = (1 << object_node_clicked);
										//}//
									}

									ImGui::TreePop();
								}

								loop_layer++;
							}

							if (layer_node_clicked != -1)
							{
								layer_selection_mask = (1 << layer_node_clicked);

								//if (ImGui::GetIO().KeyCtrl)
								//{
									//layer_selection_mask ^= (1 << layer_node_clicked);
								//}
								//else
								//{
									//layer_selection_mask = (1 << layer_node_clicked);
								//}
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

				ImGui::PopStyleVar();
				ImGui::EndChild();
			}*/
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

		if (ImGui::CollapsingHeader("Game Object"))
		{

		}

		ImGui::End();
	}

	void EditorInterface::showBackgroundTaskWindow()
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


	void EditorInterface::clearScriptWizardInput()
	{
		string::fillCharArray(m_InputClassName,			sizeof(m_InputClassName),		StringPool.BLANK);
	}

	void EditorInterface::registerSignalHandler()
	{
		signal(SIGABRT, handleSignalAbnormalTermination);
		signal(SIGFPE, handleSignalArithmeticError);
		signal(SIGILL, handleSignalIllegalInstruction);
		signal(SIGINT, handleSignalInteractiveAttention);
		signal(SIGSEGV, handleSignalInvalidStorageAccess);
		signal(SIGTERM, handleSignalTerminsationRequest);
	}

	void EditorInterface::handleSignalAbnormalTermination(int signal)
	{

	}

	void EditorInterface::handleSignalArithmeticError(int signal)
	{

	}

	void EditorInterface::handleSignalIllegalInstruction(int signal)
	{

	}

	void EditorInterface::handleSignalInteractiveAttention(int signal)
	{

	}

	void EditorInterface::handleSignalInvalidStorageAccess(int signal)
	{

	}

	void EditorInterface::handleSignalTerminsationRequest(int signal)
	{

	}
}
