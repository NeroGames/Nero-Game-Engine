#include <Nero/editor/EditorInterface.h>
#include <SFML/Window/Event.hpp>
#include <Nero/core/engine/EngineConstant.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Nero/core/utility/FileUtil.h>
#include <nativefiledialog/include/nfd.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Nero/editor/EditorConstant.h>
#include <functional>
#include <vector>

namespace  nero
{
    EditorInterface::EditorInterface(sf::RenderWindow& window):
		 m_RenderWindow(window)
		,m_InterfaceFirstDraw(true)
		,m_EditorTextureHolder(nullptr)
		////////////////////////Project and Workspace////////////////////////
		,m_WorksapceStatus(0)
		,m_InputWorksapceFolder("")
		,m_InputWorkspaceName("")
		,m_InputWorkspaceCompany("")
		,m_InputWorkspaceLead("")
		,m_SelectedWorkpsapce(nullptr)
		,m_SelectedWorkpsapceIdex(0)
		,m_SelectedProjectTypeIdex(0)
		,m_SelectedCodeEditorIdex(0)
		,m_ProjectCreationStatus(0)
		,m_AdvancedScene(new AdvancedScene())
		,g_Context(nullptr)
		,m_BuildDockspaceLayout(true)
		,m_SetupDockspaceLayout(true)
		,m_ProjectManagerTabBarSwitch()
		,m_BottomDockspaceTabBarSwitch()
		,m_Setting(nullptr)
		,m_ResourceBrowserType(ResourceType::None)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        m_ProjectManager = std::make_unique<ProjectManager>();
        m_SceneManager = std::make_unique<SceneManager>();

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
	}

    EditorInterface::~EditorInterface()
    {
       //Empty
		ax::NodeEditor::DestroyEditor(g_Context);
    }

    void EditorInterface::handleEvent(const sf::Event& event)
    {
		switch(event.type)
		{
			case sf::Event::Closed:
			{
				quitEditor();
			}break;
		}

        m_AdvancedScene->handleEvent(event);

        ImGui::SFML::ProcessEvent(event);
    }

    void EditorInterface::update(const sf::Time& timeStep)
	{
        m_AdvancedScene->update(timeStep);
    }

    void EditorInterface::render()
    {
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
		showMusicWindow();
			//bottom left
		showSceneLevelWindow();
		showSceneChunckWindow();
		showSceneScreenWindow();
		showSceneLayerWindow();

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

    void EditorInterface::showSceneWindow()
    {
		ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

            //if(ImGui::IsWindowFocused())
            {

                sf::Vector2f window_position    = ImGui::GetWindowPos();
                sf::Vector2f window_size        = ImGui::GetWindowSize();
                sf::Vector2f mouse_position     = ImGui::GetMousePos();
                float title_bar_height          = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
                sf::Vector2f window_padding     = ImGui::GetStyle().WindowPadding;

                AdvancedScene::RenderContext renderContext;
                renderContext.canvas_position   = sf::Vector2f(window_position.x + window_padding.x, window_position.y + title_bar_height + window_padding.y);
                renderContext.canvas_size       = sf::Vector2f(window_size.x - window_padding.x * 2, window_size.y - title_bar_height - window_padding.y * 2);
                renderContext.mouse_position    = sf::Vector2f(mouse_position.x - renderContext.canvas_position.x, mouse_position.y - renderContext.canvas_position.y);
                renderContext.focus             = ImGui::IsWindowFocused();

                sf::RenderTexture& renderTexture = m_AdvancedScene->render(renderContext);

                ImGui::Image(flipTexture(renderTexture.getTexture()));
            }

        ImGui::End();
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

    }

    void EditorInterface::quitEditor()
    {
		//save size and position
		std::string file = getPath({"setting", "window"}, StringPool.EXTENSION_JSON);

		if(fileExist(file))
		{
			sf::Vector2u windowSize		= m_RenderWindow.getSize();
			sf::Vector2i windowPosition = m_RenderWindow.getPosition();

			if(windowPosition.y < 0 || windowPosition.y < 0)
			{
				return;
			}

			auto loaded = loadJson(file, true);
			loaded["width"]		= windowSize.x;
			loaded["height"]		= windowSize.y;
			loaded["position_x"] = windowPosition.x;
			loaded["position_y"] = windowPosition.y;

			saveFile(file, loaded.dump(3), true);
		}

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

			//create the dockspace
			ImGui::DockSpace(m_DockspaceID, ImVec2(viewport->Size.x, viewport->Size.y - 20.f), ImGuiDockNodeFlags_None);

			//clean pointer
			viewport = nullptr;

			//build dockspace layout : this is done only once, when the editor is launched the first time
			if(m_BuildDockspaceLayout && !fileExist(getPath({"setting", EditorConstant.FILE_INTERFACE_LAYOUT}, StringPool.EXTENSION_INI)))
			{
				//split main dockspace in six
				ImGuiID upperLeftDockspaceID	= ImGui::DockBuilderSplitNode(m_DockspaceID,		ImGuiDir_Left,	0.20f, nullptr, &m_DockspaceID);
					ImGui::DockBuilderGetNode(upperLeftDockspaceID)->SizeRef.x = 180.f;
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
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_MUSIC.c_str(),				upperLeftDockspaceID);
					//lower left
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LEVEL.c_str(),				lowerLeftDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_CHUNCK.c_str(),				lowerLeftDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_SCREEN.c_str(),				lowerLeftDockspaceID);
				ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LAYER.c_str(),				lowerLeftDockspaceID);
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

				//save dockspace ids
				nlohmann::json dockspaceTable;
				dockspaceTable["upper-left-dockspace"]	= upperLeftDockspaceID;
				dockspaceTable["lower-left-dockspace"]	= lowerLeftDockspaceID;
				dockspaceTable["right-dockspace"]		= rightDockspaceID;
				dockspaceTable["toolbar-dockspace"]		= toolbarDockspaceID;
				dockspaceTable["bottom-dockspace"]		= bottomDockspaceID;
				dockspaceTable["central-dockspace"]		= centralDockspaceID;

				saveFile(getPath({"setting", "dockspace"}, StringPool.EXTENSION_JSON), dockspaceTable.dump(3));

				m_BuildDockspaceLayout = !m_BuildDockspaceLayout;
			}

			if(m_SetupDockspaceLayout)
			{
				nlohmann::json dockspaceTable = loadJson(getPath({"setting", "dockspace"}));

				//toolbar
				ImGuiID toolbarDockspaceID			= dockspaceTable["toolbar-dockspace"];
				ImGuiDockNode* toobalDockNode		= ImGui::DockBuilderGetNode(toolbarDockspaceID);
				toobalDockNode->SizeRef.y			= 16.8f;
				toobalDockNode->LocalFlags			= ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;
				//lower left dockspace
				ImGuiID lowerLeftDockspaceID		= dockspaceTable["lower-left-dockspace"];
				ImGuiDockNode* lowerLeftDockNode	= ImGui::DockBuilderGetNode(lowerLeftDockspaceID);
				lowerLeftDockNode->SizeRef.y = 400;
				//right dockspace
				ImGuiID rightDockspaceID			= dockspaceTable["right-dockspace"];
				ImGuiDockNode* rightDockNode		= ImGui::DockBuilderGetNode(rightDockspaceID);
				rightDockNode->SizeRef.x = 300;
				//bottom dockspace
				ImGuiID bottomDockspaceID			= dockspaceTable["bottom-dockspace"];
				ImGuiDockNode* bottomDockNode		= ImGui::DockBuilderGetNode(bottomDockspaceID);
				bottomDockNode->SizeRef.y = 150;

				//clean pointer
				toobalDockNode		= nullptr;
				lowerLeftDockNode	= nullptr;
				rightDockNode		= nullptr;
				bottomDockNode		= nullptr;

				m_SetupDockspaceLayout = !m_SetupDockspaceLayout;
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

            int i = 0;
            float baseWidth = 24.f + 6.f + 20.f;

            /*ImGui::SameLine(baseWidth * i++);

            if(ImGui::ImageButton(m_BlankButtonTexture))
            {

            }

            ImGui::SameLine(baseWidth * i++);

            if(ImGui::ImageButton(m_BlankButtonTexture))
            {

            }

            ImGui::SameLine(baseWidth * i++);

            if(ImGui::ImageButton(m_BlankButtonTexture))
            {

            }

            ImGui::SameLine(baseWidth * i++);

            if(ImGui::ImageButton(m_BlankButtonTexture))
            {

            }*/

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
                        showOpenPorjectWindow();

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
		for (int n = 0; n < buttons_count; n++)
		{
			ImGui::Image(m_EditorTextureHolder->getTexture("recent_project_" + toString(n+1)));
			ImGui::SameLine(0.f, 5.f);
			std::string project_name =  wrapString("Oblivion the Great Journey", 17);
			ImGui::PushID(n);
			ImGui::Button(project_name.c_str(), button_sz);
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x + 20.f; // Expected position if next button was on same line
			if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine(0.f, spacing);
			else {
				ImGui::Dummy(ImVec2(0.0f, spacing));
			}
			ImGui::PopID();
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
		ImGui::InputText("##project_name", m_InputProjectName, IM_ARRAYSIZE(m_InputProjectName));
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

                    fillCharArray(m_InputProjectCompany, IM_ARRAYSIZE(m_InputProjectCompany), workspace["default_company_name"].get<std::string>());
                    fillCharArray(m_InputProjectLead, IM_ARRAYSIZE(m_InputProjectLead), workspace["default_project_lead"].get<std::string>());
                    fillCharArray(m_InputProjectNamespace, IM_ARRAYSIZE(m_InputProjectNamespace), workspace["default_namespace"].get<std::string>());
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
		const char* codeEditorComboTable[] = {"Qt Creator", "Visual Studio 2019"};
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
        std::string project_name = StringPool.BLANK;

        std::string error_message = StringPool.BLANK;
        bool error = true;

        if(std::string(m_InputProjectName) == StringPool.BLANK)
        {
            error_message = "Please enter a Project Name";
        }
        else if(m_ProjectManager->isProjectExist(std::string(m_InputProjectName)))
        {
            error_message = "A project with the same signature already exist,\n"
                            "please choose another Project Name";
        }
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

        if (onCreate && error)
        {
            ImGui::OpenPopup("Error Creating Project");
        }
        else if(onCreate)
        {
            nlohmann::json projectJson;

            m_LastCreatedProject = std::string(m_InputProjectName);


            projectJson["workspace_name"]       = std::string(m_SelectedWorkpsapce);;
            projectJson["project_name"]         = std::string(m_InputProjectName);
            projectJson["project_type"]       = std::string(m_SelectedProjectType);;
            projectJson["project_namspace"]         = std::string(m_InputProjectNamespace);
            projectJson["project_lead"]         = std::string(m_InputProjectLead);
            projectJson["project_company"]      = std::string(m_InputProjectCompany);
            projectJson["project_description"]  = std::string(m_InputProjectDescription);

            memset(m_InputProjectName, 0, sizeof m_InputProjectName);
            //memset(m_InputProjectLead, 0, sizeof m_InputProjectLead);
            //memset(m_InputProjectNamespace, 0, sizeof m_InputProjectNamespace);
            //memset(m_InputProjectCompany, 0, sizeof m_InputProjectCompany);
            //memset(m_InputProjectDescription, 0, sizeof m_InputProjectDescription);

            auto workspace = m_ProjectManager->findWorkspace(workspaceNameTable.front());

            fillCharArray(m_InputProjectCompany, IM_ARRAYSIZE(m_InputProjectCompany), workspace["default_company_name"].get<std::string>());
            fillCharArray(m_InputProjectLead, IM_ARRAYSIZE(m_InputProjectLead), workspace["default_project_lead"].get<std::string>());
            fillCharArray(m_InputProjectNamespace, IM_ARRAYSIZE(m_InputProjectNamespace), workspace["default_namespace"].get<std::string>());
            fillCharArray(m_InputProjectDescription, IM_ARRAYSIZE(m_InputProjectDescription), "My awsome Game Project !");

            //start new thread
            m_CreateProjectFuture = std::async(std::launch::async, &EditorInterface::createProject, this, projectJson, std::ref(m_ProjectCreationStatus));

            ImGui::OpenPopup("Wait Project Creation");
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

    void EditorInterface::showOpenPorjectWindow()
    {
        float wording_width = 130.f;
        float input_width = ImGui::GetWindowContentRegionWidth() - 150.f;

        ImGui::Text("Create a new Project and start a new Adventure");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));



        ImGui::BeginChild("project list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

            ImGui::Dummy(ImVec2(0.f, 10.f));

            for(const std::string workspace : m_ProjectManager->getWorkspaceNameTable())
            {
                std::string header = workspace + " - workspace";
                if (ImGui::CollapsingHeader(header.c_str()))
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
                                        project_name        = ": " + project_name;
                            std::string project_id          = ": " + project["project_id"].get<std::string>();
                            std::string project_lead        = ": " + project["project_lead"].get<std::string>();
                            std::string project_company     = ": " + project["project_company"].get<std::string>();
                            std::string project_description = ": " + project["project_description"].get<std::string>();
                            std::string creation_date       = ": " + project["creation_date"].get<std::string>();
                            std::string modification_date   = ": " + project["modification_date"].get<std::string>();

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Name");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(project_name.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Id");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(project_id.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Lead");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(project_lead.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Company");
                            ImGui::SameLine(wording_width);
                            ImGui::Text(project_company.c_str());
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
                                openProject(project["project_name"].get<std::string>());

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

    void EditorInterface::showWorkspaceWindow()
    {
        float wording_width = 150.f;
        float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

        ImGui::Text("Workspace Folder");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width - 65.f);
        ImGui::InputText("##workspace_folder", m_InputWorksapceFolder, IM_ARRAYSIZE(m_InputWorksapceFolder));
        ImGui::SameLine(wording_width + input_width - 60.f);

        if(ImGui::Button("Browse##choose_workspace_folder", ImVec2(60, 0)))
        {
            nfdchar_t *outPath = nullptr;
            nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

            if (result == NFD_OKAY)
            {
                free(outPath);

                //Copy outPath to m_InputWorksapceFolder
                strncpy(m_InputWorksapceFolder, outPath, sizeof(m_InputWorksapceFolder) - 1);
                m_InputWorksapceFolder[sizeof(m_InputWorksapceFolder) - 1] = 0;

            }
            else if (result == NFD_CANCEL)
            {

            }
            else
            {
               //printf("Error: %s\n", NFD_GetError() );
            }
        }

        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Workspace Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_name", m_InputWorkspaceName, IM_ARRAYSIZE(m_InputWorkspaceName));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Default Project Lead");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_lead", m_InputWorkspaceLead, IM_ARRAYSIZE(m_InputWorkspaceLead));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Default Company Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_company", m_InputWorkspaceCompany, IM_ARRAYSIZE(m_InputWorkspaceCompany));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Default Namespace");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_namespace", m_InputWorkspaceNamespace, IM_ARRAYSIZE(m_InputWorkspaceNamespace));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::SameLine(wording_width + input_width - 130.f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);

        bool onCreate               = ImGui::Button("Create Workspace", ImVec2(130.f, 0));
        std::string error_message   = StringPool.BLANK;
        bool error = false;

        /*if(false)
        {
            error_message = "Please enter a Project Name";
        }
        else if(false)
        {
            error_message = "A project with the same signature already exist,\n"
                            "please choose another Project Name";
        }
        else
        {
            error = false;
        }*/

        if (onCreate && error)
        {
			ImGui::OpenPopup(EditorConstant.ERROR_CREATING_WORKSPACE.c_str());
        }
        else if(onCreate)
        {
            nlohmann::json workspaceJson;

            workspaceJson["workspace_folder"]       = std::string(m_InputWorksapceFolder);
            workspaceJson["workspace_name"]         = std::string(m_InputWorkspaceName);
            workspaceJson["default_project_lead"]   = std::string(m_InputWorkspaceLead);
            workspaceJson["default_company_name"]   = std::string(m_InputWorkspaceCompany);
            workspaceJson["default_namespace"]      = std::string(m_InputWorkspaceNamespace);

            memset(m_InputWorksapceFolder,      0, sizeof m_InputWorksapceFolder);
            memset(m_InputWorkspaceName,        0, sizeof m_InputWorkspaceName);
            memset(m_InputWorkspaceLead,        0, sizeof m_InputWorkspaceLead);
            memset(m_InputWorkspaceCompany,     0, sizeof m_InputWorkspaceCompany);
            memset(m_InputWorkspaceNamespace,   0, sizeof m_InputWorkspaceNamespace);

            createWorkspace(workspaceJson);
        }



        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));


        ImGui::Text("Workspace Folder");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width - 130.f);
        ImGui::InputText("##workspace_folder_import", m_InputWorksapceImportFolder, IM_ARRAYSIZE(m_InputWorksapceImportFolder));
        ImGui::SameLine(wording_width + input_width - 124.f);

        if(ImGui::Button("Browse##choose_workspace_folder_import", ImVec2(60, 0)))
        {
            nfdchar_t *outPath = nullptr;
            nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

            if (result == NFD_OKAY)
            {
                free(outPath);

                //Copy outPath to m_InputWorksapceFolder
                strncpy(m_InputWorksapceImportFolder, outPath, sizeof(m_InputWorksapceImportFolder) - 1);
                m_InputWorksapceImportFolder[sizeof(m_InputWorksapceImportFolder) - 1] = 0;

            }
            else if (result == NFD_CANCEL)
            {

            }
            else
            {
               //printf("Error: %s\n", NFD_GetError() );
            }
        }

        ImGui::SameLine(wording_width + input_width - 60.f);
        if(ImGui::Button("Import##import_workspace", ImVec2(60, 0)))
        {

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
                    std::string workspace_id            = ": " + worksapce["workspace_id"].get<std::string>();
                    std::string workspace_name          = ": " + worksapce["workspace_name"].get<std::string>();
                    std::string default_project_lead    = ": " + worksapce["default_project_lead"].get<std::string>();
                    std::string default_company_name    = ": " + worksapce["default_company_name"].get<std::string>();
                    std::string default_namespace       = ": " + worksapce["default_namespace"].get<std::string>();
                    std::string workspace_directory     = ": " + worksapce["workspace_directory"].get<std::string>();

                    ImGui::Text("Workspace Id");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(workspace_id.c_str());
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Workspace Name");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(workspace_name.c_str());
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Workspace Directory");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(workspace_directory.c_str());
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Default Project Lead");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(default_project_lead.c_str());
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Default Company Name");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(default_company_name.c_str());
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Default Namespace");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(default_namespace.c_str());
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                }

            }

        ImGui::EndChild();




        ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
		if(ImGui::BeginPopupModal(EditorConstant.ERROR_CREATING_WORKSPACE.c_str()))
        {
            ImGui::Text("%s", error_message.c_str());

            ImGui::Dummy(ImVec2(0.0f, 45.0f));

            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 95.f);

            if (ImGui::Button("Close##workspace_error_close_button", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
         }
    }

    void EditorInterface::createWorkspace(const nlohmann::json& workspaceJson)
    {
        m_ProjectManager->createWorkspace(workspaceJson);
    }

    int EditorInterface::createProject(const nlohmann::json& projectJson, int& status)
    {
        m_ProjectManager->createProject(projectJson, status);

        return status;
    }

    void EditorInterface::openProject(const std::string& project_name)
    {
         m_GameProject =  m_ProjectManager->openProject(project_name);
         m_AdvancedScene = m_GameProject->getAdvancedScene();
         m_UpdateWindowTile(project_name);
    }

	void EditorInterface::setCallbackWindowTitle(std::function<void (const std::string&)> fn)
    {
        m_UpdateWindowTile = fn;
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

    void EditorInterface:: reloadProject()
    {

        if(m_GameProject)
        {
            nero_log("reloading project ...");

            m_GameProject->loadProjectLibrary();

        }
    }




    ////////////////////////Project and Workspace End////////////////////////

    /*void EditorInterface::addScene(const std::string& projectName, std::function<Scene::Ptr(Scene::Context)> factory)
    {
        //Engine SDK create and open a C++ project [Project_C++]

        if(m_ProjectManager->isProjectExist(projectName))
        {
            m_SceneManager->addSceneFacotry(formatString(projectName), factory);
        }
        else
        {
            m_ProjectManager->createProject(projectName, ProjectManager::CPP_PROJECT);
            m_SceneManager->addSceneFacotry(formatString(projectName), factory);

            //save project
            //load project
        }
    }

    void EditorInterface::addLuaScene(const std::string& projectName, std::function<LuaScene::Ptr(Scene::Context)> factory)
    {
        //Engine SDK create or open a C++/Lua project [Project_C++_LUA]

        if(m_ProjectManager->isProjectExist(projectName))
        {
            m_SceneManager->addLuaSceneFacotry(formatString(projectName), factory);
        }
        else
        {
            m_ProjectManager->createProject(projectName, ProjectManager::LUA_CPP_PROJECT);
            m_SceneManager->addLuaSceneFacotry(formatString(projectName), factory);
        }
    }*/

    void EditorInterface::setEditorSetting(const nlohmann::json& setting)
    {
        m_EditorSetting = setting;

        m_ProjectManager->setEditorSetting(m_EditorSetting);


        auto workspaceNameTable = m_ProjectManager->getWorkspaceNameTable();

        if(!workspaceNameTable.empty())
        {
            auto workspace = m_ProjectManager->findWorkspace(workspaceNameTable.front());

            fillCharArray(m_InputProjectCompany, IM_ARRAYSIZE(m_InputProjectCompany), workspace["default_company_name"].get<std::string>());
            fillCharArray(m_InputProjectLead, IM_ARRAYSIZE(m_InputProjectLead), workspace["default_project_lead"].get<std::string>());
            fillCharArray(m_InputProjectNamespace, IM_ARRAYSIZE(m_InputProjectNamespace), workspace["default_namespace"].get<std::string>());
            fillCharArray(m_InputProjectDescription, IM_ARRAYSIZE(m_InputProjectDescription), "My awsome Game Project !");
        }

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

		if(ImGui::Button("Sprite##open_sprite_resource", ImVec2(100.f, 100.f)))
        {
			m_ResourceBrowserType = ResourceType::Texture;
        }

		ImGui::SameLine();


		if(ImGui::Button("Animation##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Animation;
		}

		ImGui::SameLine();


		if(ImGui::Button("Mesh##open_sprite_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Mesh;
		}

		ImGui::SameLine();

		if(ImGui::Button("Shape##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Shape;
		}

		ImGui::SameLine();

		if(ImGui::Button("Particle##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Particle;
		}

		ImGui::SameLine();

		if(ImGui::Button("Light##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Light;
		}


		ImGui::SameLine();

		if(ImGui::Button("Font##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Font;
		}

		ImGui::SameLine();

		if(ImGui::Button("Composite##open_shape_resource", ImVec2(100.f, 100.f)))
		{
			m_ResourceBrowserType = ResourceType::Composite;
		}


		ImGui::End();
    }

	void EditorInterface::showResourceBrowserWindow()
    {
        //project manager window
		if(m_ResourceBrowserType != ResourceType::None)
        {
            ImGui::Begin("Resource Browser", nullptr, ImGuiWindowFlags());

                if(ImGui::Button("Import File##import_sprite_resource", ImVec2(100.f, 0.f)))
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

						NFD_PathSet_Free(&pathSet);
                    }
                    else if ( result == NFD_CANCEL ) {
						//puts("User pressed cancel.");
                    }
                    else {
                        //printf("Error: %s\n", NFD_GetError() );
                    }

                }

                ImGui::SameLine();


                if(ImGui::Button("Close##close_sprite_resource", ImVec2(60.f, 0.f)))
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

					}

				ImGui::EndChild();

			ImGui::End();
        }
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
			  // ImGui::OpenPopup(EditorConstant.PROJECT_MANAGER.c_str());
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
			  // ImGui::OpenPopup(EditorConstant.PROJECT_MANAGER.c_str());
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

                 }

                ImGui::SameLine();

                 if(ImGui::Button("Load", button_size))
                 {

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

	void EditorInterface::showMusicWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_MUSIC.c_str());

		ImGui::End();
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

	void EditorInterface::showSceneLayerWindow()
	{
		ImGui::Begin(EditorConstant.WINDOW_LAYER.c_str());

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

		ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("scene_level_list", ImVec2(), true);

			static bool check = false;
			ImGui::Checkbox("", &check);

			ImGui::SameLine();

			ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_ReadOnly;
			static char screen_name[100] = "screen one";
			ImGui::SetNextItemWidth(130.f);
			ImGui::InputText("##level_id", screen_name, IM_ARRAYSIZE(screen_name), flags2);

	   ImGui::EndChild();


		ImGui::End();
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

    void EditorInterface::showSceneChunckWindow()
    {
		ImGui::Begin(EditorConstant.WINDOW_CHUNCK.c_str());

        ImGui::Text("Manage Scene Chuncks");
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

		ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("scene_level_list", ImVec2(), true);

			static bool check = false;
			ImGui::Checkbox("", &check);

			ImGui::SameLine();

			ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_ReadOnly;
			static char screen_name[100] = "screen one";
			ImGui::SetNextItemWidth(130.f);
			ImGui::InputText("##level_id", screen_name, IM_ARRAYSIZE(screen_name), flags2);

	   ImGui::EndChild();

        ImGui::End();
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
		if(m_InterfaceFirstDraw && !m_SetupDockspaceLayout)
        {

			//toolbar
			ImGuiID rightDockspaceID		= m_Setting->getSetting("dockspace").getUInt("right-dockspace");
			ImGuiID upperLeftDockspaceID	= m_Setting->getSetting("dockspace").getUInt("upper-left-dockspace");
			ImGuiID lowerLeftDockspaceID	= m_Setting->getSetting("dockspace").getUInt("lower-left-dockspace");

			auto right_node = ImGui::DockBuilderGetNode(rightDockspaceID);
			right_node->TabBar->NextSelectedTabId = right_node->TabBar->Tabs.front().ID;

			auto upper_left_node = ImGui::DockBuilderGetNode(upperLeftDockspaceID);
			upper_left_node->TabBar->NextSelectedTabId = right_node->TabBar->Tabs.front().ID;

			auto left_bottom_node = ImGui::DockBuilderGetNode(lowerLeftDockspaceID);
			left_bottom_node->TabBar->NextSelectedTabId = right_node->TabBar->Tabs.front().ID;

			ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());

			//commit
			m_InterfaceFirstDraw = !m_InterfaceFirstDraw;
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

		if (ImGui::CollapsingHeader("Scene Graph"))
		{

		}

		if (ImGui::CollapsingHeader("Current Level"))
		{

		}

		if (ImGui::CollapsingHeader("Current Chunk"))
		{

		}

		if (ImGui::CollapsingHeader("Current Layer"))
		{

		}

		if (ImGui::CollapsingHeader("Selected Object"))
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

	void EditorInterface::setResourceManager(ResourceManager::Ptr resourceManager)
	{
		m_ResourceManager = resourceManager;
	}


}
