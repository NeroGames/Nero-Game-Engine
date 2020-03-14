#include <Nero/editor/EditorInterface.h>
#include <SFML/Window/Event.hpp>
#include <Nero/core/cpp/engine/EngineConstant.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Nero/core/cpp/utility/FileUtil.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Nero/editor/EditorConstant.h>
#include <functional>
#include <vector>
#include <IconFontCppHeaders/IconsFontAwesome5.h>
#include <easy/profiler.h>
namespace  nero
{
    EditorInterface::EditorInterface(sf::RenderWindow& window):
		 m_RenderWindow(window)
		,m_InterfaceFirstDraw(true)
		,m_EditorTextureHolder(nullptr)
		////////////////////////Project and Workspace////////////////////////
		,m_WorksapceStatus(0)
		,m_SelectedWorkpsapce(nullptr)
		,m_SelectedWorkpsapceIdex(0)
		,m_SelectedProjectTypeIdex(0)
		,m_SelectedCodeEditorIdex(0)
		,m_ProjectCreationStatus(0)
		,m_AdvancedScene(nullptr)
		,g_Context(nullptr)
		,m_BuildDockspaceLayout(true)
		,m_SetupDockspaceLayout(true)
		,m_ProjectManagerTabBarSwitch()
		,m_BottomDockspaceTabBarSwitch()
		,m_Setting(nullptr)
		,m_ResourceBrowserType(ResourceType::None)
		,m_EditorMode(EditorMode::WORLD_BUILDER)
		,m_BuilderMode(BuilderMode::OBJECT)
		,m_EditorCamera(nullptr)
		,m_SelectedChunkNode(StringPool.BLANK)
		,InputSelectedWorldChunkId(-1)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		io.Fonts->AddFontDefault();

		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-regular-400.ttf", 13.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("resource/editor/font/forkawesome-webfont.ttf", 13.0f, &config, icon_ranges);

		m_RenderTexture = std::make_shared<sf::RenderTexture>();
		m_ProjectManager = std::make_unique<ProjectManager>();
		m_RenderContext = std::make_shared<RenderContext>();

        //old = std::cout.rdbuf(buffer.rdbuf());

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
	}

    EditorInterface::~EditorInterface()
    {
       //Empty
		ax::NodeEditor::DestroyEditor(g_Context);
    }

	void EditorInterface::init()
	{
		//setup game mode information text
		m_GameModeInfo.setFont(m_EditorFontHolder->getDefaultFont());
		m_GameModeInfo.setCharacterSize(18.f);
		m_GameModeInfo.setFillColor(sf::Color::White);

		//clear workspace input
		clearWorkspaceInput();
		clearProjectInput();
		updateProjectInput();

		//
		m_ProjectManager->setSetting(m_Setting);
		m_ProjectManager->setRenderTexture(m_RenderTexture);
		m_ProjectManager->setResourceManager(m_ResourceManager);
		m_ProjectManager->setRenderContext(m_RenderContext);
		m_ProjectManager->setCamera(m_EditorCamera);
	}

    void EditorInterface::handleEvent(const sf::Event& event)
    {
		if(isMouseOnCanvas())
		{
			m_EditorCamera->handleEvent(event);
		}

		if(m_AdvancedScene)
		{
			m_AdvancedScene->handleEvent(event, m_EditorMode, m_BuilderMode);
		}

		switch(event.type)
		{
			case sf::Event::Closed:
				quitEditor();
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

		if(m_AdvancedScene)
		{
			m_AdvancedScene->update(timeStep, m_EditorMode, m_BuilderMode);
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
			//game setting
		showGameSettingWindow();
			//game project
		showGameProjectWindow();
			//imgui demo
		ImGui::ShowDemoWindow();


		//left dockspace
			//upper left
		showUtilityWindow();
		if(m_EditorMode == EditorMode::WORLD_BUILDER)
			showSceneLevelWindow();
		if(m_EditorMode == EditorMode::SCREEN_BUILDER)
			showSceneScreenWindow();

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
		showResourceWindow();

		//init
		interfaceFirstDraw();

		//background task
		showBackgroundTaskWindow();


		//commit
		ImGui::SFML::Render(m_RenderWindow);
	}

	void EditorInterface::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
	{
		if(isPressed)
		{
			if(key == sf::Keyboard::Space && !CTRL_SHIFT_ALT() && isMouseOnCanvas())
				switchBuilderMode();
		}
	}

	void EditorInterface::switchBuilderMode()
	{
		if(m_EditorMode == EditorMode::WORLD_BUILDER)
		{
			if(m_BuilderMode == BuilderMode::OBJECT && !CTRL_SHIFT_ALT())
			{
				m_BuilderMode = BuilderMode::MESH;
			}
			else if(m_BuilderMode != BuilderMode::OBJECT && !CTRL_SHIFT_ALT())
			{
				m_BuilderMode = BuilderMode::OBJECT;
			}
		}
		else if(m_EditorMode == EditorMode::PLAY_GAME && !CTRL_SHIFT_ALT())
		{
			m_EditorMode = EditorMode::WORLD_BUILDER;
			m_BuilderMode = BuilderMode::OBJECT;
		}
	}

    void EditorInterface::showSceneWindow()
    {
		ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

			buildRenderContext();

			prepareRenderTexture();

			if(m_AdvancedScene)
			{
				m_AdvancedScene->render(m_EditorMode, m_BuilderMode);
			}

			//Render on Front Screen
			m_RenderTexture->setView(m_RenderTexture->getDefaultView());
				if(m_AdvancedScene)
				{
					m_AdvancedScene->renderFrontScreen(m_EditorMode, m_BuilderMode);
				}
				renderCamera();
				renderGameModeInfo();
			m_RenderTexture->setView(m_EditorCamera->getView());


			ImGui::Image(flipTexture(m_RenderTexture->getTexture()));

			showCanvasMenu();

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

	void EditorInterface::setCamera(const AdvancedCamera::Ptr& camera)
	{
		m_EditorCamera = camera;
	}

	void EditorInterface::buildRenderContext()
	{
		sf::Vector2f window_position    = ImGui::GetWindowPos();
		sf::Vector2f window_size        = ImGui::GetWindowSize();
		sf::Vector2f mouse_position     = ImGui::GetMousePos();
		float title_bar_height          = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
		sf::Vector2f window_padding     = ImGui::GetStyle().WindowPadding;


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
			if(m_AdvancedScene && m_AdvancedScene->getSelectedGameLevel()->levelSetting->getBool("enable_lighting"))
			{
				clearColor = sf::Color::White;
			}
		}


		m_RenderTexture->clear(clearColor);
		m_RenderTexture->setView(m_EditorCamera->getView());
	}

	bool EditorInterface::isMouseOnCanvas()
	{
		sf::Rect<float> canvas(m_RenderContext->canvas_position.x, m_RenderContext->canvas_position.y, m_RenderContext->canvas_size.x, m_RenderContext->canvas_size.y);

		sf::Vector2i mousePosition = ImGui::GetMousePos();

		return canvas.contains(mousePosition.x, mousePosition.y);
	}

    void EditorInterface::showGameSettingWindow()
    {
		ImGui::Begin(EditorConstant.WINDOW_GAME_SETTING.c_str());

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

    void EditorInterface::updateFrameRate(const float& frameRate, const float& frameTime)
    {
		m_FrameRate = frameRate;
		m_FrameTime = frameTime;
    }

    void EditorInterface::quitEditor()
    {
		//save size and position
		std::string file = getPath({"setting", "window"}, StringPool.EXTENSION_JSON);

		if(fileExist(file))
		{
			sf::Vector2u windowSize		= m_RenderWindow.getSize();
			sf::Vector2i windowPosition = m_RenderWindow.getPosition();

			if(windowPosition.y < 0)
			{
				windowPosition.y = 0;
			}

			if(windowPosition.x < -8)
			{
				windowPosition.x = -8;
			}

			auto loaded = loadJson(file, true);
			loaded["width"]		= windowSize.x;
			loaded["height"]		= windowSize.y;
			loaded["position_x"] = windowPosition.x;
			loaded["position_y"] = windowPosition.y;

			saveFile(file, loaded.dump(3), true);
		}

		m_ProjectManager->close();

        m_RenderWindow.close();
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
			//if(m_Setting->getSetting("dockspace").getBool("build_layout") && !fileExist(getPath({"setting", EditorConstant.FILE_IMGUI_SETTING}, StringPool.EXTENSION_INI)))
			if(m_Setting->getSetting("dockspace").getBool("build_layout") && !m_Setting->getSetting("dockspace").getBool("imgui_setting_exist"))
			{
				//split main dockspace in six
				ImGuiID upperLeftDockspaceID	= ImGui::DockBuilderSplitNode(m_DockspaceID,		ImGuiDir_Left,	0.20f, nullptr, &m_DockspaceID);
				ImGui::DockBuilderGetNode(upperLeftDockspaceID)->SizeRef.x = m_Setting->getSetting("dockspace").getSetting("upper_left_dock").getFloat("width");
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
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LEVEL.c_str(),				upperLeftDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_SCREEN.c_str(),				upperLeftDockspaceID);
					//lower left
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LAYER.c_str(),				lowerLeftDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_CHUNCK.c_str(),				lowerLeftDockspaceID);
					//right
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_EXPLORER.c_str(),			rightDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_HELP.c_str(),				rightDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_RESOURCE_BROWSER.c_str(),	rightDockspaceID);
					//bottom
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LOGGING.c_str(),				bottomDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_RESOURCE.c_str(),			bottomDockspaceID);
					//center
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_SCENE.c_str(),			centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_SETTING.c_str(),		centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_PROJECT.c_str(),		centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_FACTORY.c_str(),				centralDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_IMGUI_DEMO.c_str(),			centralDockspaceID);

				//commit dockspace
				ImGui::DockBuilderFinish(m_DockspaceID);

				//lower left dockspace
				ImGuiDockNode* lowerLeftDockNode	= ImGui::DockBuilderGetNode(lowerLeftDockspaceID);
				lowerLeftDockNode->SizeRef.y = m_Setting->getSetting("dockspace").getSetting("lower_left_dock").getFloat("height");
				//right dockspace
				ImGuiDockNode* rightDockNode		= ImGui::DockBuilderGetNode(rightDockspaceID);
				rightDockNode->SizeRef.x = m_Setting->getSetting("dockspace").getSetting("right_dock").getFloat("width");
				//bottom dockspace
				ImGuiDockNode* bottomDockNode		= ImGui::DockBuilderGetNode(bottomDockspaceID);
				bottomDockNode->SizeRef.y = m_Setting->getSetting("dockspace").getSetting("bottom_dock").getFloat("height");

				//clean pointer
				lowerLeftDockNode	= nullptr;
				rightDockNode		= nullptr;
				bottomDockNode		= nullptr;

				//update and save dockspace setting
				m_Setting->getSetting("dockspace").getSetting("toolbar_dock").setUInt("id", toolbarDockspaceID);
				m_Setting->getSetting("dockspace").setBool("build_layout", false);

				saveFile(getPath({"setting", "dockspace"}, StringPool.EXTENSION_JSON), m_Setting->getSetting("dockspace").toString(), true);
			}

			if(m_SetupDockspaceLayout)
			{
				//toolbar
				ImGuiID toolbarDockspaceID			= m_Setting->getSetting("dockspace").getSetting("toolbar_dock").getUInt("id");
				ImGuiDockNode* toolbarDockNode		= ImGui::DockBuilderGetNode(toolbarDockspaceID);
				toolbarDockNode->SizeRef.y			= m_Setting->getSetting("dockspace").getSetting("toolbar_dock").getFloat("height");
				toolbarDockNode->LocalFlags			= ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;

				//clean pointer
				toolbarDockNode		= nullptr;

				m_SetupDockspaceLayout = false;
			}

			if (ImGui::BeginMenuBar())
			{
				createEditorMenuBar();

				ImGui::EndMenuBar();
			}

		ImGui::End(); //end dockspace window
    }

	//2- create menu bar
	void EditorInterface::createEditorMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("action_1", nullptr, nullptr);
			ImGui::MenuItem("action_2", nullptr, nullptr);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Views"))
		{
			ImGui::MenuItem("action_3", nullptr, nullptr);
			ImGui::MenuItem("action_4", nullptr, nullptr);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Download"))
		{
			ImGui::MenuItem("action_3", nullptr, nullptr);
			ImGui::MenuItem("action_4", nullptr, nullptr);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Helps"))
		{
			ImGui::MenuItem("action_3", nullptr, nullptr);
			ImGui::MenuItem("action_4", nullptr, nullptr);

			ImGui::EndMenu();
		}
	}

	//3-  show toolbar
    void EditorInterface::showToolbarWindow()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 2.f));
		ImGui::Begin(EditorConstant.WINDOW_TOOLBAR.c_str(), nullptr, window_flags);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.5f, 2.5f));

			float width = ImGui::GetWindowContentRegionWidth();
			float start = (width - (60.f * 5.f + 10.f * 4.f))/2.f;
			float buttonSpace = 60.f + 14.f;
			int i = 0;

			if(m_EditorMode != EditorMode::WORLD_BUILDER && m_GameProject)
			{
				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("world_button")))
				{
				   m_EditorMode = EditorMode::WORLD_BUILDER;
				   ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
				}

				ImGui::SameLine(buttonSpace + 6.f);
			}

			if(m_EditorMode != EditorMode::SCREEN_BUILDER && m_GameProject)
			{
				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("screen_button")))
				{
				   m_EditorMode = EditorMode::SCREEN_BUILDER;
				   ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
				}

				ImGui::SameLine(buttonSpace + 6.f);
			}


			if(m_EditorMode != EditorMode::OBJECT_BUILDER && m_GameProject)
			{
				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("factory_button")))
				{
				   m_EditorMode = EditorMode::OBJECT_BUILDER;
				   ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
				}
			}

			if(m_GameProject)
			{
				ImGui::SameLine(start + buttonSpace*i++);

				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("play_button")))
				{
				   playScene();
				}

				ImGui::SameLine(start + buttonSpace*i++);

				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("pause_button")))
				{
				   pauseScene();
				}

				ImGui::SameLine(start + buttonSpace*i++);

				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("step_button")))
				{
				   stepScene();
				}

				ImGui::SameLine(start + buttonSpace*i++);

				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("reset_button")))
				{
				   resetScene();
				}

				ImGui::SameLine(start + buttonSpace*i++);

				if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("render_button")))
				{
				   renderScene();
				}
			}

			ImGui::SameLine(width - 72.f);

			 if(ImGui::ImageButton(m_EditorTextureHolder->getTexture(EditorConstant.TEXTURE_PROJECT_BUTTON)))
             {
				ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
             }

             ImGui::SameLine(width - 72.f - 24.f - 3.f - 10.f);

			 if(ImGui::ImageButton(m_EditorTextureHolder->getTexture(EditorConstant.TEXTURE_RELOAD_BUTTON)))
             {
                reloadProject();
             }

             ImGui::SameLine(width - 72.f - 48.f - 6.f - 20.f);

			 if(ImGui::ImageButton(m_EditorTextureHolder->getTexture(EditorConstant.TEXTURE_EDIT_BUTTON)))
             {
                editProject();
             }

             ImGui::SameLine(width - 72.f - 72.f - 9.f - 30.f);

			 if(ImGui::ImageButton(m_EditorTextureHolder->getTexture(EditorConstant.TEXTURE_COMPILE_BUTTON)))
             {
                compileProject();
             }

             ImGui::PopStyleVar();

			 //show project manager window
			 showProjectManagerWindow();

        ImGui::End();
		ImGui::PopStyleVar();
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

                    ImGui::EndTabBar();
                }

				m_ProjectManagerTabBarSwitch.resetSwith();

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

		auto recentProjectTable =  loadJson(getPath({"setting", "recent_project"}));

		for (int i = 0; i < buttons_count; i++)
		{
			if(i < recentProjectTable.size())
			{
				auto project = recentProjectTable[recentProjectTable.size()-i-1];

				ImGui::Image(m_EditorTextureHolder->getTexture("recent_project_" + toString(i+1)));
				ImGui::SameLine(0.f, 5.f);
				std::string project_name =  wrapString(project["project_name"].get<std::string>(), 17);
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


		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
		if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("recent_project_create_project")))
		{
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_CREATE_PROJECT);
		}

		ImGui::SameLine(0.f, 50.f);

		if(ImGui::ImageButton(m_EditorTextureHolder->getTexture("recent_project_open_project")))
		{
			m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_OPEN_PROJECT);
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);

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
			ImGui::InputText("##project_name", m_InputProjectName, sizeof(m_InputProjectName));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Workspace");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);

			//load workpsace
			std::vector<std::string> workspaceNameTable = m_ProjectManager->getWorkspaceNameTable();

			if(workspaceNameTable.empty())
			{
				workspaceNameTable.push_back("There is no Workspace availabled");
			}

			std::size_t worskpaceCount = workspaceNameTable.size();
			const char** workspaceComboTable = new const char* [worskpaceCount];

			fillCharTable(workspaceComboTable, workspaceNameTable);

			m_SelectedWorkpsapce = workspaceComboTable[m_SelectedWorkpsapceIdex];
			if (ImGui::BeginCombo("##combo", m_SelectedWorkpsapce, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < worskpaceCount; n++)
				{
					bool is_selected = (m_SelectedWorkpsapce == workspaceComboTable[n]);


					if (ImGui::Selectable(workspaceComboTable[n], is_selected))
					{
						m_SelectedWorkpsapce = workspaceComboTable[n];
						m_SelectedWorkpsapceIdex = n;

						auto workspace = m_ProjectManager->findWorkspace(workspaceNameTable[n]);

						fillCharArray(m_InputProjectCompany,		sizeof(m_InputProjectCompany),		workspace["company_name"].get<std::string>());
						fillCharArray(m_InputProjectLead,			sizeof(m_InputProjectLead),			workspace["project_lead"].get<std::string>());
						fillCharArray(m_InputProjectNamespace,		sizeof(m_InputProjectNamespace),	workspace["project_namespace"].get<std::string>());
					}

					if (is_selected)
					{
						 ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			//delete array
			for (std::size_t i = 0 ; i < worskpaceCount; i++)
			{
				delete[] workspaceComboTable[i] ;
			}
			delete[] workspaceComboTable ;
			workspaceComboTable = nullptr;

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Project Type");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			const char* projectTypeComboTable[] = {"CPP Project", "Lua Project", "CPP and Lua Project"};
			m_SelectedProjectType = projectTypeComboTable[m_SelectedProjectTypeIdex];            // Here our selection is a single pointer stored outside the object.
			if (ImGui::BeginCombo("##project_type_combo", m_SelectedProjectType, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < IM_ARRAYSIZE(projectTypeComboTable); n++)
				{
					bool is_selected = (m_SelectedProjectType == projectTypeComboTable[n]);

					if (ImGui::Selectable(projectTypeComboTable[n], is_selected))
					{
						m_SelectedProjectType = projectTypeComboTable[n];
						m_SelectedProjectTypeIdex = n;
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
			const char* codeEditorComboTable[] = {"Qt Creator", "Visual Studio"};
			m_SelectedCodeEditor = codeEditorComboTable[m_SelectedCodeEditorIdex];
			if (ImGui::BeginCombo("##code-editor-combo", m_SelectedCodeEditor, ImGuiComboFlags()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(codeEditorComboTable); n++)
				{
					bool is_selected = (m_SelectedCodeEditor == codeEditorComboTable[n]);

					if (ImGui::Selectable(codeEditorComboTable[n], is_selected))
					{
						m_SelectedCodeEditor = codeEditorComboTable[n];
						m_SelectedCodeEditorIdex = n;
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
			ImGui::InputText("##project_lead", m_InputProjectLead, IM_ARRAYSIZE(m_InputProjectLead));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Company Name");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			ImGui::InputText("##project_company", m_InputProjectCompany, IM_ARRAYSIZE(m_InputProjectCompany));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Namesapce");
			ImGui::SameLine(wording_width);
			ImGui::SetNextItemWidth(input_width);
			ImGui::InputText("##project_namespace", m_InputProjectNamespace, IM_ARRAYSIZE(m_InputProjectNamespace));
			ImGui::Dummy(ImVec2(0.0f, 2.0f));


			ImGui::Text("Description");
			ImGui::SameLine(wording_width);
			static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
			ImGui::InputTextMultiline("##project_description", m_InputProjectDescription, IM_ARRAYSIZE(m_InputProjectDescription), ImVec2(input_width, ImGui::GetTextLineHeight() * 5), flags);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
			ImGui::SetCursorPosY(winsow_size.y * 0.85f - 100.f);
			bool onCreate = ImGui::Button("Create", ImVec2(100, 0));

			std::string error_message = StringPool.BLANK;
			bool error = true;

			if(std::string(m_InputProjectName) == StringPool.BLANK)
			{
				error_message = "Please enter a Project Name";
			}
			/*else if(m_ProjectManager->isProjectExist(std::string(m_InputProjectName)))
			{
				error_message = "A project with the same signature already exist,\n"
								"please choose another Project Name";
			}*/
			else if(std::string(m_InputProjectLead) == StringPool.BLANK)
			{
				error_message = "Please enter a Project Lead";
			}
			else if (std::string(m_InputProjectCompany) == StringPool.BLANK)
			{
				error_message = "Please enter a Company Name";
			}
			else
			{
				error = false;
			}


			if(onCreate)
			{
				if (error)
				{
					ImGui::OpenPopup("Error Creating Project");
				}
				else
				{
					Setting parameter;
					parameter.setString("project_name", std::string(m_InputProjectName));
					parameter.setString("workspace_name", std::string(m_SelectedWorkpsapce));
					parameter.setString("project_type", std::string(m_SelectedProjectType));
					parameter.setString("project_namespace", std::string(m_InputProjectNamespace));
					parameter.setString("project_lead", std::string(m_InputProjectLead));
					parameter.setString("company_name", std::string(m_InputProjectCompany));
					parameter.setString("description", std::string(m_InputProjectDescription));
					parameter.setString("code_editor", std::string(m_SelectedCodeEditor));

					m_LastCreatedProject = m_ProjectManager->getProjectDirectory(parameter);

					clearProjectInput();
					updateProjectInput();

					//start new thread
					m_CreateProjectFuture = std::async(std::launch::async, &EditorInterface::createProject, this, parameter, std::ref(m_ProjectCreationStatus));


					ImGui::OpenPopup("Wait Project Creation");
				}
			}


			ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
			if(ImGui::BeginPopupModal("Error Creating Project", nullptr, window_flags))
			{
				ImGui::Text("%s", error_message.c_str());
				ImGui::Dummy(ImVec2(0.0f, 45.0f));
				ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 95.f);
				if (ImGui::Button("Close", ImVec2(100, 0)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			 }

			ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
			if(ImGui::BeginPopupModal("Wait Project Creation", nullptr, window_flags))
			{
				std::string message = StringPool.BLANK;

				if(m_ProjectCreationStatus == 1)
				{
					message = "Creating project ... ";
				}
				else if(m_ProjectCreationStatus == 2)
				{
					 message = "Creating project ... ";
					 message += "\nStep 1/2 : Generating project files";
				}
				else if(m_ProjectCreationStatus == 3)
				{
					 message = "Creating project ... ";
					 message += "\nStep 1/2 : Generating project files ..";
					 message += "\nStep 2/2 : Compiling project ...";
				}
				else if(m_ProjectCreationStatus == 4)
				{
					 message = "Creating project ... ";
					 message += "\nStep 1/2 : Generating project files ..";
					 message += "\nStep 2/2 : Compiling project ...";
					  message += "\nCreation Complet !";
				}

				ImGui::TextWrapped("%s", message.c_str());

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				if(m_ProjectCreationStatus == 4)
				{
					if (ImGui::Button("Open project", ImVec2(100, 0)))
					{

						ImGui::CloseCurrentPopup();
						ImGui::ClosePopupToLevel(0, true);

						 openProject(m_LastCreatedProject);
					}

					ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 95.f);

					if (ImGui::Button("Close", ImVec2(100, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			 }

        //check if a workspace exit
		if(false)//m_WorksapceStatus == 0)
        {
            ImGui::OpenPopup("Creat a Worksapce");
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 100.f));
        if(ImGui::BeginPopupModal("Creat a Worksapce", nullptr, window_flags))
        {

            if (ImGui::Button("Create Workspace##close_workspace", ImVec2(100, 0)))
            {
                m_WorksapceStatus = 1;

                ImGui::CloseCurrentPopup();
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
                    std::string message = "List of Projects in workspace : " + workspace;
                    ImGui::Text(message.c_str());
                    ImGui::Separator();

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    for(const nlohmann::json& project : m_ProjectManager->getWorkspaceProjectTable(workspace))
                    {
                        std::string project_name = project["project_name"].get<std::string>();

                        ImGui::SetCursorPosX(20.f);
                        if (ImGui::CollapsingHeader(project_name.c_str()))
                        {
                            std::string project_lead        = ": " + project["project_lead"].get<std::string>();
							std::string project_company     = ": " + project["company_name"].get<std::string>();
							std::string project_description = ": " + project["description"].get<std::string>();
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
                            ImGui::SameLine(wording_width);
                            ImGui::Text(project_description.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 5.0f));


                            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 100.f);
                            if (ImGui::Button("Open Project", ImVec2(100, 0)))
                            {
								openProject(getPath({m_ProjectManager->findWorkspace(workspace)["workspace_directory"].get<std::string>(), "Project", project_name}));

                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::Dummy(ImVec2(0.0f, 10.0f));

                        }
                    }
                 }

                ImGui::Dummy(ImVec2(0.f, 15.f));
            }

         ImGui::EndChild();
    }

	void EditorInterface::selectDirectory(std::function<void(nfdchar_t *outPath)> callback)
	{
		nfdchar_t *outPath = nullptr;
		nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

		if (result == NFD_OKAY)
		{
			callback(outPath);

			free(outPath);
		}
		else if (result == NFD_CANCEL)
		{
			nero_log("select directory canceled");
		}
		else
		{
		   nero_log("failed to select directory : " + nero_ss(NFD_GetError()));
		}
	}

    void EditorInterface::showWorkspaceWindow()
    {
        float wording_width = 150.f;
        float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

		//create workspace
		ImGui::Text("Location");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width - 65.f);
		ImGui::InputText("##workspace_location", m_InputWorksapceLocation, sizeof(m_InputWorksapceLocation));
        ImGui::SameLine(wording_width + input_width - 60.f);
		if(ImGui::Button("Browse##choose_workspace_location", ImVec2(60, 0)))
        {
			selectDirectory([this](nfdchar_t *outPath)
			{
				fillCharArray(m_InputWorksapceLocation, sizeof(m_InputWorksapceLocation), std::string(outPath));
			});
        }
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Workspace Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_name", m_InputWorkspaceName, sizeof(m_InputWorkspaceName));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Company Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_company", m_InputWorkspaceCompany, sizeof(m_InputWorkspaceCompany));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Project Lead");
		ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_lead", m_InputWorkspaceLead, sizeof(m_InputWorkspaceLead));
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::Text("Project Namespace");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
		ImGui::InputText("##workspace_namespace", m_InputWorkspaceNamespace, sizeof(m_InputWorkspaceNamespace));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::SameLine(wording_width + input_width - 130.f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);

        bool onCreate               = ImGui::Button("Create Workspace", ImVec2(130.f, 0));
        std::string error_message   = StringPool.BLANK;
		bool error = true;

		//workpspace location blank
		if(std::string(m_InputWorksapceLocation) == StringPool.BLANK)
        {
			error_message = "Please select a location directory";
        }
		//workpspace location not valid
		else if(!directoryExist(std::string(m_InputWorksapceLocation)))
        {
			error_message = "The selected location is not a valid directory";
        }
		//workpspace name blank
		else if(std::string(m_InputWorkspaceName) == StringPool.BLANK)
		{
			error_message = "Please enter a workspace name";
		}
		//workpspace company name blank
		else if(std::string(m_InputWorkspaceCompany) == StringPool.BLANK)
		{
			error_message = "Please enter a company name";
		}
		//workpspace project lead name blank
		else if(std::string(m_InputWorkspaceLead) == StringPool.BLANK)
		{
			error_message = "Please enter a project lead name";
		}
		//workpspace namespace blank
		else if(std::string(m_InputWorkspaceNamespace) == StringPool.BLANK)
		{
			error_message = "Please enter a project namesapce";
		}
		else
        {
			error = false;
		}

        if (onCreate && error)
		{
			ImGui::OpenPopup(EditorConstant.ERROR_CREATING_WORKSPACE.c_str());
		}
        else if(onCreate)
        {
			Setting parameter;

			parameter.setString("workspace_location", std::string(m_InputWorksapceLocation));
			parameter.setString("workspace_name", std::string(m_InputWorkspaceName));
			parameter.setString("company_name", std::string(m_InputWorkspaceCompany));
			parameter.setString("project_lead", std::string(m_InputWorkspaceLead));
			parameter.setString("project_namespace", std::string(m_InputWorkspaceNamespace));

			clearWorkspaceInput();

			createWorkspace(parameter);
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

		//import workspace
		ImGui::Text("Workspace Directory");
        ImGui::SameLine(wording_width);
		ImGui::SetNextItemWidth(input_width - 130.f);
		ImGui::InputText("##workspace_import", m_InputWorksapceLocationImport, sizeof(m_InputWorksapceLocationImport));
		 ImGui::SameLine(wording_width + input_width - 124.f);
		if(ImGui::Button("Browse##choose_workspace_import", ImVec2(60.f, 0)))
        {
			selectDirectory([this](nfdchar_t *outPath)
			{
				//get selected directory
				fillCharArray(m_InputWorksapceLocationImport, sizeof(m_InputWorksapceLocationImport), std::string(outPath));
			});
        }

		ImGui::SameLine(wording_width + input_width - 60.f);
		bool onImport						= ImGui::Button("Import##import_workspace", ImVec2(60.f, 0));
		std::string import_error_message	= StringPool.BLANK;
		bool import_error					= true;

		//workpspace location blank
		if(std::string(m_InputWorksapceLocationImport) == StringPool.BLANK)
		{
			import_error_message = "Please select a workspace";
		}
		//workpspace location not valid
		else if(!directoryExist(std::string(m_InputWorksapceLocationImport)))
		{
			import_error_message = "The selected location is not a valid directory";
		}
		//workpspace location not valid
		else if(!fileExist(getPath({std::string(m_InputWorksapceLocationImport), ".workspace"})))
		{
			import_error_message = "The selected directory is not a valid workspace";
		}
		else
		{
			import_error = false;
		}


		if (onImport && import_error)
		{
			ImGui::OpenPopup(EditorConstant.ERROR_CREATING_WORKSPACE.c_str());
		}
		else if(onImport)
		{
			//import workspace
			importWorkspace(std::string(m_InputWorksapceLocationImport));
			//clear input
			fillCharArray(m_InputWorksapceLocationImport, sizeof(m_InputWorksapceLocationImport), StringPool.BLANK);
		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));



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


		ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
		if(ImGui::BeginPopupModal(EditorConstant.ERROR_CREATING_WORKSPACE.c_str()))
		{
			ImGui::BeginChild("##error_message", ImVec2(0.f, 70.f));

			//if(onCreate)
			//{
				ImGui::TextWrapped("%s", error_message.c_str());
			//}
			//else if(onImport)
			//{
				//ImGui::TextWrapped("%s", import_error_message.c_str());
			//}

			ImGui::Dummy(ImVec2(0.0f, 45.0f));

			 ImGui::EndChild();

			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth()/2.f - 50.f);

			if (ImGui::Button("Close##workspace_error_close_button", ImVec2(100, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		 }
	}

	void EditorInterface::createWorkspace(const Setting& parameter)
    {
		m_ProjectManager->createWorkspace(parameter);
		updateProjectInput();
    }

	void EditorInterface::importWorkspace(const std::string& directory)
	{
		m_ProjectManager->importWorkspace(directory);
		updateProjectInput();
	}

	int EditorInterface::createProject(const Setting& parameter, int& status)
    {
		m_ProjectManager->createProject(parameter, status);

        return status;
    }

	void EditorInterface::openProject(const std::string& projectDirectory)
    {
		//close currently open project
		if(m_GameProject)
		{
			//closeProject();
		}

		//open new project
		m_GameProject	= m_ProjectManager->openProject(projectDirectory);
		m_AdvancedScene = m_GameProject->getAdvancedScene();

		//update editor window title
		updateWindowTitle(m_GameProject->getProjectName());
    }

	void EditorInterface::updateWindowTitle(const std::string& title)
	{
		 m_UpdateWindowTile(title);
	}

	void EditorInterface::setCallbackWindowTitle(std::function<void (const std::string&)> fn)
    {
        m_UpdateWindowTile = fn;
    }

	void EditorInterface::playScene()
	{
		if(m_AdvancedScene && m_EditorMode != EditorMode::PLAY_GAME)
		{
			try
			{
				m_AdvancedScene->playScene();
				m_EditorMode = EditorMode::PLAY_GAME;
			}
			catch(std::exception const& e)
			{

			}
		}
	}

	void EditorInterface::pauseScene()
	{
		if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

			gameLevel->levelSetting->setBool("pause_level", !gameLevel->levelSetting->getBool("pause_level"));
		}
	}

	void EditorInterface::stepScene()
	{

		if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

			gameLevel->levelSetting->setBool("pause_scene", true);
			gameLevel->levelSetting->setBool("single_step", true);
		}
	}

	void EditorInterface::resetScene()
	{
		if(m_AdvancedScene && m_EditorMode == EditorMode::PLAY_GAME)
		{
			m_AdvancedScene->playScene();
		}
	}

	void EditorInterface::renderScene()
	{

	}

    void EditorInterface::compileProject()
    {
		m_CompileProjectFuture = std::async(std::launch::async, [this]()
		{
			if(m_GameProject)
			{
				m_GameProject->compileProject();

			}

			return 0;
		});

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

            m_GameProject->loadLibrary();

        }
    }

    void EditorInterface::setEditorSetting(const nlohmann::json& setting)
    {
        m_EditorSetting = setting;


    }

    void EditorInterface::loadAllProject()
    {
        m_ProjectManager->loadAllProject();
    }

    sf::Sprite EditorInterface::flipTexture(const sf::Texture& texture)
    {
        sf::Vector2u size = texture.getSize();
        sf::Sprite sprite(texture, sf::IntRect(0, size.y, size.x, -size.y));

        return sprite;
    }

	void EditorInterface::showLoggingWindow()
    {

        ImGui::Begin("Logging");

        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        m_LoggerApplication.Draw("Logging");
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

		if(ImGui::Button("Spawer##spawn_object", ImVec2(100.f, 100.f)))
		{

		}

		printSameLine();

		if(ImGui::Button("Factory##open_factory_resource", ImVec2(100.f, 100.f)))
		{

		}

		ImGui::End();
    }

	void EditorInterface::showResourceBrowserWindow()
    {
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

							m_ResourceManager->loadFile(m_ResourceBrowserType, fileTable);
							saveResourceFile(m_ResourceBrowserType, fileTable);

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

	void EditorInterface::saveResourceFile(ResourceType type, const std::vector<std::string> fileTable)
	{
		switch (m_ResourceBrowserType)
		{
			case ResourceType::Texture:
			{
				//copy texture file and json helper
				for(std::string file : fileTable)
				{
					//copyFile(file, m_GameProject->getWorkspaceResourceFoler() + "Texture" + "filename")
				}

			}break;

			case ResourceType::Animation:
			{
				//copy texture file and json helper

			}break;

			case ResourceType::Font:
			{
				//copy font file and json helper

			}break;

			case ResourceType::Sound:
			{
				//copy sound file and json helper

			}break;

			case ResourceType::Music:
			{
				//copy music file and json helper

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
			if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				m_AdvancedScene->addObject(Object::Mesh_Object, "Polygon", getAddObjectPosition(), m_EditorMode);
		}

		printSameLine();

		if(ImGui::Button("Circle", ImVec2(100.f, 100.f)))
		{
			if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				m_AdvancedScene->addObject(Object::Mesh_Object, "Circle", getAddObjectPosition(), m_EditorMode);
		}

		printSameLine();

		if(ImGui::Button("Line", ImVec2(100.f, 100.f)))
		{
			if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
				m_AdvancedScene->addObject(Object::Mesh_Object, "Line", getAddObjectPosition(), m_EditorMode);
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
				if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
					m_AdvancedScene->addObject(Object::Sprite_Object, spriteTable[n], getAddObjectPosition(), m_EditorMode);
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
					m_AdvancedScene->addObject(Object::Light_Object, spriteTable[n], getAddObjectPosition(), m_EditorMode);
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
				if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
					m_AdvancedScene->addObject(Object::Animation_Object, animationTable[n], getAddObjectPosition(), m_EditorMode);
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
		std::string file_name = getPath({"resource/editor/texture", fontName}, StringPool.EXTENSION_PNG);

		if(!fileExist(file_name))
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

                ImVec2 button_size = ImVec2(70.f, 0.f);

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

                 }

                ImGui::SameLine();

                 if(ImGui::Button("Forum", button_size))
                 {

                 }


                 if(ImGui::Button("Snippet", button_size))
                 {

                 }

                 ImGui::SameLine();

                 if(ImGui::Button("API", button_size))
                 {

                 }

            ImGui::EndChild();

        ImGui::End();
    }

	void EditorInterface::onSaveProject()
	{
		if(m_GameProject)
		{
			m_GameProject->saveProject();
		}
	}

	void EditorInterface::onLoadProject()
	{
		if(m_GameProject)
		{
			m_GameProject->loadProject();
		}
	}

	void EditorInterface::autoSaveProject()
	{
		/*if(m_GameProject && m_AutoSaveClock.getElapsedTime() > sf::seconds(m_Setting->getSetting("editor").getUInt("auto_save_interval")))
		{
			m_GameProject->saveProject();
			m_AutoSaveClock.restart();
		}*/
	}

    void EditorInterface::showSceneScreenWindow()
    {
		ImGui::Begin(EditorConstant.WINDOW_SCREEN.c_str());

            ImGui::Text("Manage Scene Screens");
            ImGui::Separator();
			ImGui::Dummy(ImVec2(0.f, 2.f));

			float width = 82.f;
			static char new_screen_name[100] = "";
			ImGui::SetNextItemWidth(width*2.f + 8.f);
			ImGui::InputText("##screen_did", new_screen_name, IM_ARRAYSIZE(new_screen_name));

			ImGui::Dummy(ImVec2(0.f, 2.f));

			ImVec2 button_size = ImVec2(width, 0.f);
            //addcreen
			if(ImGui::Button("Add##add_screen", button_size))
            {
                //
            }

			ImGui::SameLine();

			if(ImGui::Button("Rename##add_screen", button_size))
			{
				//
			}


			if(ImGui::Button("Duplicate##add_screen", button_size))
			{
				//
			}

			ImGui::SameLine();

			if(ImGui::Button("Remove##add_screen", button_size))
			{
				//
			}




            //rename screen
            //copy

            ImGui::Dummy(ImVec2(0.f, 5.f));

            ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("scene_screen_list", ImVec2(), true);

                static bool check = false;
				ImGui::Checkbox("", &check);

                ImGui::SameLine();

                ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_ReadOnly;
                static char screen_name[100] = "screen one";
				ImGui::SetNextItemWidth(130.f);
                ImGui::InputText("##screen_id", screen_name, IM_ARRAYSIZE(screen_name), flags2);               

           ImGui::EndChild();

        ImGui::End();
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

			if(ImGui::Button("Remove##remove_world_chunk", button_size))
			{
				removeObjectLayer();
			}

			ImGui::Dummy(ImVec2(0.f, 5.f));

			ImGui::BeginChild("##manage_object_layer", ImVec2(), true);

				if(m_AdvancedScene)
				{
					auto sceneBuilder = m_AdvancedScene->getSelectedSceneBuilder(m_EditorMode);

					int inputSelectedObjectLayerId = sceneBuilder->getSelectedLayer()->getObjectId();

					for(const auto& objectLayer : sceneBuilder->getLayerTable())
					{
						std::string itemId = "##select_layer" + toString(objectLayer->getObjectId());
						ImGui::RadioButton(itemId.c_str(), &inputSelectedObjectLayerId, objectLayer->getObjectId());

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
						fillCharArray(layer_name, sizeof(layer_name), objectLayer->getName());
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
		m_AdvancedScene->getSelectedSceneBuilder(m_EditorMode)->addLayer();
	}

	void EditorInterface::removeObjectLayer()
	{

	}

	void EditorInterface::showSceneLevelWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_LEVEL.c_str());

		ImGui::Text("Manage Scene Level");
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.f, 2.f));

		float width = 82.f;
		static char input_level_name[100] = "";
		ImGui::SetNextItemWidth(width*2.f + 8.f);
		ImGui::InputText("##level_id", input_level_name, IM_ARRAYSIZE(input_level_name));

		ImGui::Dummy(ImVec2(0.f, 2.f));

		ImVec2 button_size = ImVec2(width, 0.f);
		//addcreen
		if(ImGui::Button("Add##add_level", button_size))
		{
			//
		}

		ImGui::SameLine();

		if(ImGui::Button("Rename##rename_level", button_size))
		{
			//
		}


		if(ImGui::Button("Duplicate##duplicate_level", button_size))
		{
			//
		}

		ImGui::SameLine();

		if(ImGui::Button("Remove##remove_level", button_size))
		{
			//
		}

		//rename screen
		//copy

		ImGui::Dummy(ImVec2(0.f, 5.f));

		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.f, 5.f));

		//ImGui::BeginChild("scene_level_list", ImVec2());

			static int e = 0;

			ImGui::RadioButton("##bsdf", &e, 0);

			ImGui::SameLine();

			static bool sdfsfs = false;
			ImGui::Checkbox("##bsdf2", &sdfsfs);

			ImGui::SameLine();

			ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_ReadOnly;
			static char screen_name[100] = "screen one";
			ImGui::SetNextItemWidth(118.f);
			ImGui::InputText("##level_id", screen_name, IM_ARRAYSIZE(screen_name), flags2);

	   //ImGui::EndChild();


		ImGui::End();
	};

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

				if(m_AdvancedScene)
				{
					auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

					InputSelectedWorldChunkId = m_AdvancedScene->getSelectedWorldChunk()->chunkId;

					for(const auto& worldChunk : gameLevel->chunkTable)
					{
						std::string itemId = "##select_chunk" + toString(worldChunk->chunkId);
						ImGui::RadioButton(itemId.c_str(), &InputSelectedWorldChunkId, worldChunk->chunkId);

						if(ImGui::IsItemClicked())
						{
							m_AdvancedScene->setSelectedWorldChunk(worldChunk);
						}

						ImGui::SameLine();

						itemId = "##visible_chunk" + toString(worldChunk->chunkId);
						ImGui::Checkbox(itemId.c_str(), &worldChunk->visible);

						ImGui::SameLine();

						char chunk_name[100];
						fillCharArray(chunk_name, sizeof(chunk_name), worldChunk->name);
						ImGui::SetNextItemWidth(118.f);
						itemId = "##chunk_name" + toString(worldChunk->chunkId);
						ImGui::InputText(itemId.c_str(), chunk_name, sizeof(chunk_name));

						if(ImGui::IsItemEdited())
						{
							worldChunk->name = std::string(chunk_name);
						}
					}
				}

			ImGui::EndChild();

        ImGui::End();
    }

	void EditorInterface::addWorldChunk()
	{
		if(m_AdvancedScene && m_EditorMode == EditorMode::WORLD_BUILDER && m_BuilderMode == BuilderMode::OBJECT)
		{
			m_AdvancedScene->addWorldChunk(StringPool.BLANK);
		}
	}

	void EditorInterface::removeWorldChunk()
	{
		//m_AdvancedScene->removeWorldChunk();
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
			if(m_AdvancedScene)
			{
				auto gameLevel = m_AdvancedScene->getSelectedGameLevel();


				ImGuiViewport* viewport = ImGui::GetMainViewport();
				float game_world_window_height = viewport->Size.y * 0.25f;
				viewport = nullptr;
				//ImGui::Text( ICON_FA_PASTE "  Paint" );

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

										/*if (ImGui::GetIO().KeyCtrl)
										{
											object_selection_mask ^= (1 << object_node_clicked);
										}
										else
										{
											object_selection_mask = (1 << object_node_clicked);
										}*/
									}

									ImGui::TreePop();
								}

								loop_layer++;
							}

							if (layer_node_clicked != -1)
							{
								layer_selection_mask = (1 << layer_node_clicked);

								/*if (ImGui::GetIO().KeyCtrl)
								{
									layer_selection_mask ^= (1 << layer_node_clicked);
								}
								else
								{
									layer_selection_mask = (1 << layer_node_clicked);
								}*/
							}

							ImGui::TreePop();
						}

						loop_chunk++;
					}

					if (chunk_node_clicked != -1)
					{
						chunk_selection_mask = (1 << chunk_node_clicked);

						/*if (ImGui::GetIO().KeyCtrl)
						{
							chunk_selection_mask ^= (1 << chunk_node_clicked);
						}
						else
						{
							chunk_selection_mask = (1 << chunk_node_clicked);
						}*/
					}

					ImGui::TreePop();
				}

				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
		}

		if (ImGui::CollapsingHeader("Game Level"))
		{
			if(m_AdvancedScene)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
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
					/*levelSetting->setBool("enable_warm_starting", true);
					levelSetting->setBool("enable_continous", true);
					levelSetting->setBool("enable_sub_stepping", false);
					levelSetting->setBool("enable_sleep", true);
					//
					levelSetting->setBool("pause_level", false);
					levelSetting->setBool("single_step", false);*/

				ImGui::EndChild();
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
			auto taskTable = m_GameProject->getBackgroundTaskTable();

			for(BackgroundTask& task : taskTable)
			{
				if(!task.isCompleted())
				{
					ImGui::Text(task.printMessage().c_str());
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
	}

	void EditorInterface::setSetting(Setting::Ptr setting)
	{
		m_Setting = setting;
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

	void EditorInterface::setResourceManager(ResourceManager::Ptr resourceManager)
	{
		m_ResourceManager = resourceManager;
	}

	void EditorInterface::clearWorkspaceInput()
	{
		fillCharArray(m_InputWorksapceLocation,			sizeof(m_InputWorksapceLocation),		StringPool.BLANK);
		fillCharArray(m_InputWorkspaceName,				sizeof(m_InputWorkspaceName),			StringPool.BLANK);
		fillCharArray(m_InputWorkspaceLead,				sizeof(m_InputWorkspaceLead),			StringPool.BLANK);
		fillCharArray(m_InputWorkspaceCompany,			sizeof(m_InputWorkspaceCompany),		StringPool.BLANK);
		fillCharArray(m_InputWorkspaceNamespace,		sizeof(m_InputWorkspaceNamespace),		StringPool.BLANK);
		fillCharArray(m_InputWorksapceLocationImport,	sizeof(m_InputWorksapceLocationImport), StringPool.BLANK);
	}

	void EditorInterface::clearProjectInput()
	{
		fillCharArray(m_InputProjectName,			sizeof(m_InputProjectName),			StringPool.BLANK);
		fillCharArray(m_InputProjectLead,			sizeof(m_InputProjectLead),			StringPool.BLANK);
		fillCharArray(m_InputProjectCompany,		sizeof(m_InputProjectCompany),		StringPool.BLANK);
		fillCharArray(m_InputProjectNamespace,		sizeof(m_InputProjectNamespace),	StringPool.BLANK);
		fillCharArray(m_InputProjectDescription,	sizeof(m_InputProjectDescription),	StringPool.BLANK);
	}

	void EditorInterface::updateProjectInput()
	{
		auto workspaceNameTable = m_ProjectManager->getWorkspaceTable();

		if(!workspaceNameTable.empty())
		{
			auto workspace = workspaceNameTable.front();

			fillCharArray(m_InputProjectCompany,		sizeof(m_InputProjectCompany),		workspace["company_name"].get<std::string>());
			fillCharArray(m_InputProjectLead,			sizeof(m_InputProjectLead),			workspace["project_lead"].get<std::string>());
			fillCharArray(m_InputProjectNamespace,		sizeof(m_InputProjectNamespace),	workspace["project_namespace"].get<std::string>());

			m_SelectedWorkpsapceIdex = 0;
		}
		else
		{
			clearProjectInput();
		}
	}

}
