#include <Nero/editor/Interface.h>
#include <SFML/Window/Event.hpp>
#include <Nero/core/engine/EngineConstant.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Nero/core/utility/FileUtil.h>
#include <nativefiledialog/include/nfd.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Nero/editor/EditorViewPool.h>
#include <functional>
#include <vector>

namespace  nero
{
    Interface::Interface(sf::RenderWindow& window):
        m_RenderWindow(window)
       ,m_InterfaceFirstDraw(true)
       ////////////////////////Project and Workspace////////////////////////
       ,m_WorksapceStatus(0)
       ,m_InputWorksapceFolder("")
       ,m_InputWorkspaceName("")
       ,m_InputWorkspaceCompany("")
       ,m_InputWorkspaceLead("")
       ,m_ProjectManagerWindowSize(800.f, 500.f)
       ,m_SelectedWorkpsapce(nullptr)
       ,m_SelectedWorkpsapceIdex(0)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_ProjectManagerBannerTexture.loadFromFile("editor_project_manager.png");
        m_ProjectButtonTexture.loadFromFile("project_button.png");
        m_CompileButtonTexture.loadFromFile("compile_button.png");
        m_ReloadButtonTexture.loadFromFile("reload_button.png");
        m_EditButtonTexture.loadFromFile("edit_button.png");
        m_BlankButtonTexture.loadFromFile("blank_button.png");


        m_ProjectManager = std::make_unique<ProjectManager>();
        m_SceneManager = std::make_unique<SceneManager>();

    }

    Interface::~Interface()
    {
       //Empty
    }

    void Interface::handleEvent(const sf::Event& event)
    {
        if(event.type == sf::Event::Closed)
        {
            quitEditor();
        }

        if(event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f position;
            position.x = event.mouseButton.x;
            position.y = event.mouseButton.y;

            if(event.mouseButton.button == sf::Mouse::Left)
            {
                m_SceneManager->addCircleShape(position);
                //m_LoggerApplication.AddLog("adding a circle\n");

            }
            else
            {
                m_SceneManager->removeCircleShape(position);
                //m_LoggerApplication.AddLog("removing a circle\n");

            }

        }

        if(event.type == sf::Event::Resized)
        {
            //ImGuiDockNode* node = ImGui::DockBuilderGetNode(actionBarId);
            //node->AuthorityForSize = ImGuiDataAuthority_Window;

        }

        ImGui::SFML::ProcessEvent(event);
    }

    void Interface::update(const sf::Time& timeStep)
    {

    }

    void Interface::render()
    {

        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);


        createDockSpace();

        ImGui::Begin(ViewPool.SCENE.c_str());

            //if(ImGui::IsWindowFocused())
            {

                sf::Vector2f window_position    = ImGui::GetWindowPos();
                sf::Vector2f window_size        = ImGui::GetWindowSize();
                sf::Vector2f mouse_position     = ImGui::GetMousePos();
                float title_bar_height          = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
                sf::Vector2f window_padding     = ImGui::GetStyle().WindowPadding;

                SceneManager::RenderContext renderContext;
                renderContext.canvas_position   = sf::Vector2f(window_position.x + window_padding.x, window_position.y + title_bar_height + window_padding.y);
                renderContext.canvas_size       = sf::Vector2f(window_size.x - window_padding.x * 2, window_size.y - title_bar_height - window_padding.y * 2);
                renderContext.mouse_position    = sf::Vector2f(mouse_position.x - renderContext.canvas_position.x, mouse_position.y - renderContext.canvas_position.y);
                renderContext.focus             = ImGui::IsWindowFocused();

                sf::RenderTexture& renderTexture = m_SceneManager->render(renderContext);

                ImGui::Image(flipTexture(renderTexture.getTexture()));
            }

        ImGui::End();

        ImGui::Begin("Game     ");

        ImGui::End();

        ImGui::Begin("Project     ");

        ImGui::End();



        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        ImGui::Begin("Mesh    ");

        ImGui::End();

        showCurrentSceneWindow();

        ImGui::Begin("Resource");

        ImGui::End();

        ImGui::Begin("Engine Help");

        ImGui::End();


        showToolbarWindow();

        showLogWindow();

        showResourceCategoryWindow();

        showResourceWindow();

        ImGui::ShowDemoWindow();

        //upper left
        showUtilityWindow();
        showSceneChunckWindow();
        showSceneScreenWindow();



        interfaceFirstDraw();

        ImGui::SFML::Render(m_RenderWindow);
    }

    void Interface::updateFrameRate(const float& frameRate, const float& frameTime)
    {

    }

    void Interface::quitEditor()
    {
        m_RenderWindow.close();
    }

    void Interface::createDockSpace()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);



        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        if(!setup_dock)
        {
            dockspace_id = ImGui::GetID("MyDockSpace");

        }

        ImGui::DockSpace(dockspace_id, viewport->Size, dockspace_flags);



        if (ImGui::BeginMenuBar())
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

            ImGui::EndMenuBar();
        }

        ImGui::End();

        if(!setup_dock && !fileExist(getPath({"imgui"}, ".ini")))
        {
            ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.

            //split main dock in five
            //ImGuiID dock_id_bottom        = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);
            dock_id_upper_left          = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_left_bottom = ImGui::DockBuilderSplitNode(dock_id_upper_left, ImGuiDir_Down, 0.20f, nullptr, &dock_id_upper_left);
            dock_id_right               = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
            actionBarId                 = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_down        = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);

            ImGui::DockBuilderDockWindow("Scene     ", ImGui::DockBuilderGetCentralNode(dockspace_id)->ID);
            ImGui::DockBuilderDockWindow("Game     ", ImGui::DockBuilderGetCentralNode(dockspace_id)->ID);
            ImGui::DockBuilderDockWindow("Project     ", ImGui::DockBuilderGetCentralNode(dockspace_id)->ID);
            //upper left
            ImGui::DockBuilderDockWindow(ViewPool.UTILITY.c_str(),      dock_id_upper_left);
            ImGui::DockBuilderDockWindow(ViewPool.SCENE_CHUNCK.c_str(), dock_id_upper_left);
            ImGui::DockBuilderDockWindow(ViewPool.SCENE_SCREEN.c_str(), dock_id_upper_left);

            ImGui::DockBuilderDockWindow("Mesh    ", dock_id_left_bottom);
            ImGui::DockBuilderDockWindow("Scene Tools", dock_id_right);
            ImGui::DockBuilderDockWindow("Resource", dock_id_right);
            ImGui::DockBuilderDockWindow("Engine Help", dock_id_right);
            ImGui::DockBuilderDockWindow("Resource Browser", dock_id_right);
            ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_right);
            ImGui::DockBuilderDockWindow(ViewPool.Toolbar.c_str(), actionBarId);
            ImGui::DockBuilderDockWindow("Logging", dock_id_down);
            ImGui::DockBuilderDockWindow("Resource", dock_id_down);
            //ImGui::DockBuilderDockWindow("Bottom", dock_id_bottom);

            ImGui::DockBuilderFinish(dockspace_id);

            auto node = ImGui::DockBuilderGetNode(actionBarId);
            node->SizeRef.y = 16.8f;
            node->LocalFlags = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;

            auto right_node = ImGui::DockBuilderGetNode(dock_id_right);
            right_node->SizeRef.x = 400;

            auto bottom_node = ImGui::DockBuilderGetNode(dock_id_down);
            bottom_node->SizeRef.y = 150;

            auto left_bottom_node = ImGui::DockBuilderGetNode(dock_id_left_bottom);
            left_bottom_node->SizeRef.y = 400;

            setup_dock = true;

        }
    }

    void Interface::showToolbarWindow()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 2.f));
        ImGui::Begin(ViewPool.Toolbar.c_str(), nullptr, window_flags);

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


             if(ImGui::ImageButton(m_ProjectButtonTexture))
             {
                ImGui::OpenPopup(ViewPool.PROJECT_MANAGER.c_str());
             }

             ImGui::SameLine(width - 72.f - 24.f - 3.f - 10.f);

             if(ImGui::ImageButton(m_ReloadButtonTexture))
             {
                reloadProject();
             }

             ImGui::SameLine(width - 72.f - 48.f - 6.f - 20.f);

             if(ImGui::ImageButton(m_EditButtonTexture))
             {
                editProject();
             }

             ImGui::SameLine(width - 72.f - 72.f - 9.f - 30.f);

             if(ImGui::ImageButton(m_CompileButtonTexture))
             {
                compileProject();
             }


             ImGui::PopStyleVar();
             showProjectManagerWindow();

        ImGui::End();
        //ImGui::PopStyleColor();
         ImGui::PopStyleVar();
    }


    ////////////////////////Project and Workspace////////////////////////
    void Interface::showProjectManagerWindow()
    {
        //Window flags
        ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        //Winsow size
        ImVec2 winsow_size              = m_ProjectManagerWindowSize;

        //Project manager window
        ImGui::SetNextWindowSize(winsow_size);
        //Begin window
        if(ImGui::BeginPopupModal(ViewPool.PROJECT_MANAGER.c_str(), nullptr, window_flags))
        {
            //Save cursor position
            ImVec2 cursor = ImGui::GetCursorPos();

            //Panel 1 : Engine Banner
            ImGui::BeginChild("##project_manager_panel_1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.33f, winsow_size.y - 20.f));
                ImGui::Image(m_ProjectManagerBannerTexture);
            ImGui::EndChild();

            //Panel 2 : Window tabs
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            ImGui::SetCursorPosY(cursor.y);
            ImGui::BeginChild("##project_manager_panel_2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.67f, winsow_size.y * 0.85f));

                if (ImGui::BeginTabBar("##project_manager_tabbar"))
                {
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if (ImGui::BeginTabItem(ViewPool.CREATE_PROJECT.c_str()))
                    {
                        showCreateProjectWindow();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(ViewPool.OPEN_PROJECT.c_str()))
                    {
                        showOpenPorjectWindow();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(ViewPool.RECENT_PROJECT.c_str()))
                    {
                        showRecentProjectWindow();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(ViewPool.WORKSPACE.c_str()))
                    {
                        showWorkspaceWindow();

                        ImGui::EndTabItem();
                    }

                    ImGui::EndTabBar();
                }

            ImGui::EndChild();

            ImGui::SetCursorPosX(winsow_size.x/2.f);
            ImGui::SetCursorPosY(winsow_size.y - 30.f);
            if (ImGui::Button("Close##close_project_manager_window", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
            /*float wording_width = 130.f;

                float input_width = ImGui::GetWindowContentRegionWidth() - 150.f;

                ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs;
                if (ImGui::BeginTabBar("ProjectManagerTab", tab_bar_flags))
                {
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if (ImGui::BeginTabItem("Create Project"))
                    {

                        ImGui::EndTabItem();

                    }

                    if (ImGui::BeginTabItem("Open Project"))
                    {
                        ImGui::Text("Create a new Project and start a new Adventure");
                        ImGui::Dummy(ImVec2(0.0f, 5.0f));



                        ImGui::BeginChild("project list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                            for(const nlohmann::json& project : m_ProjectManager->getProjectTable())
                            {
                                std::string project_name = project["project_name"].get<std::string>();

                                if (ImGui::CollapsingHeader(project_name.c_str()))
                                {
                                                project_name        = ": " + project_name;
                                    std::string project_id          = ": " + project["project_id"].get<std::string>();
                                    std::string project_lead        = ": " + project["project_lead"].get<std::string>();
                                    std::string project_company     = ": " + project["project_company"].get<std::string>();
                                    std::string project_description = ": " + project["project_description"].get<std::string>();
                                    std::string creation_date       = ": " + project["creation_date"].get<std::string>();
                                    std::string modification_date   = ": " + project["modification_date"].get<std::string>();

                                    ImGui::Text("Project Name");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(project_name.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                                    ImGui::Text("Project Id");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(project_id.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                                    ImGui::Text("Project Lead");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(project_lead.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                                    ImGui::Text("Project Company");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(project_company.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                                    ImGui::Text("Create Date");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(creation_date.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                                    ImGui::Text("Modification Date");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(modification_date.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                                    ImGui::Text("Project Description");
                                    ImGui::SameLine(wording_width);
                                    ImGui::Text(project_description.c_str());
                                    ImGui::Dummy(ImVec2(0.0f, 5.0f));


                                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 100.f);
                                    if (ImGui::Button("Open Project", ImVec2(100, 0)))
                                    {
                                        openProject(project);

                                        ImGui::CloseCurrentPopup();
                                    }

                                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                                }
                            }
                         ImGui::EndChild();



                        ImGui::EndTabItem();

                    }

                    if (ImGui::BeginTabItem("Recent Project"))
                    {
                        ImGui::EndTabItem();

                    }

                    ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
                    if(m_WorksapceStatus == 1)
                    {
                       flags = ImGuiTabItemFlags_SetSelected;
                       m_WorksapceStatus = 0;
                    }

                    if (ImGui::BeginTabItem("Workspace", nullptr, flags))
                    {
                        wording_width = 150.f;
                        input_width = ImGui::GetWindowContentRegionWidth() - wording_width;

                        ImGui::Text("Workspace Folder");
                        ImGui::SameLine(wording_width);
                        ImGui::SetNextItemWidth(input_width - 65.f);
                        ImGui::InputText("##workspace_folder", m_InputWorksapceFolder, IM_ARRAYSIZE(m_InputWorksapceFolder));
                        ImGui::SameLine(wording_width + input_width - 60.f);

                        if (ImGui::Button("Browse##choose_folder", ImVec2(60, 0)))
                        {
                            nfdchar_t *outPath = nullptr;
                            nfdresult_t result = NFD_PickFolder(nullptr, &outPath);


                            if ( result == NFD_OKAY ) {
                                puts("Success!");
                                puts(outPath);
                                free(outPath);

                                strncpy(m_InputWorksapceFolder, outPath, sizeof(m_InputWorksapceFolder) - 1);
                                m_InputWorksapceFolder[sizeof(m_InputWorksapceFolder) - 1] = 0;

                            }
                            else if ( result == NFD_CANCEL ) {
                                puts("User pressed cancel.");
                            }
                            else {
                                printf("Error: %s\n", NFD_GetError() );
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

                        ImGui::SameLine(wording_width + input_width - 130.f);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
                        if (ImGui::Button("Create Workspace##create_workspace_button", ImVec2(130.f, 0)))
                        {

                        }

                        ImGui::Dummy(ImVec2(0.0f, 10.0f));

                        ImGui::BeginChild("workspace list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                        ImGui::EndChild(),


                        ImGui::EndTabItem();

                    }

                    ImGui::EndTabBar();
                }
            ImGui::EndChild();*/

            /*//Panel 3 : existing project list
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            ImGui::SetCursorPosY(cursor.y + size.y * 0.5f);
            ImGui::BeginChild("Open Project", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.67f, size.y * 0.5f - 75.f));

            ImGui::Text("Open Project");
            ImGui::Separator();

            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::BeginChild("project list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                for(const nlohmann::json& project : m_ProjectManager->getProjectTable())
                {
                    std::string project_name = project["project_name"].get<std::string>();

                    if (ImGui::CollapsingHeader(project_name.c_str()))
                    {
                                    project_name        = ": " + project_name;
                        std::string project_id          = ": " + project["project_id"].get<std::string>();
                        std::string project_lead        = ": " + project["project_lead"].get<std::string>();
                        std::string project_company     = ": " + project["project_company"].get<std::string>();
                        std::string project_description = ": " + project["project_description"].get<std::string>();
                        std::string creation_date       = ": " + project["creation_date"].get<std::string>();
                        std::string modification_date   = ": " + project["modification_date"].get<std::string>();

                        ImGui::Text("Project Name");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_name.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Id");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_id.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Lead");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_lead.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Company");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_company.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Create Date");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(creation_date.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Modification Date");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(modification_date.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 2.0f));

                        ImGui::Text("Project Description");
                        ImGui::SameLine(wording_width);
                        ImGui::Text(project_description.c_str());
                        ImGui::Dummy(ImVec2(0.0f, 5.0f));


                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 100.f);
                        if (ImGui::Button("Open Project", ImVec2(100, 0)))
                        {
                            openProject(project);

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    }
                }
             ImGui::EndChild();

            ImGui::EndChild();*/

            /*ImGui::Dummy(ImVec2(0.0f, 7.0f));

            cursor = ImGui::GetCursorPos();*/
            /*ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            if (ImGui::Button("Open Folder", ImVec2(100, 0)))
            {
                nfdchar_t *outPath = nullptr;
                nfdresult_t result = NFD_PickFolder(nullptr, &outPath); //NFD_OpenDialog( nullptr, nullptr, &outPath );


                if ( result == NFD_OKAY ) {
                    puts("Success!");
                    puts(outPath);
                    free(outPath);

                    nero_log(outPath);

                    openProject(std::string(outPath));
                }
                else if ( result == NFD_CANCEL ) {
                    puts("User pressed cancel.");
                }
                else {
                    printf("Error: %s\n", NFD_GetError() );
                }

                //ImGui::CloseCurrentPopup();
            }*/



    void Interface::showCreateProjectWindow()
    {
        //Window flags
        ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        //Winsow size
        ImVec2 winsow_size              = m_ProjectManagerWindowSize;

        float wording_width = 130.f;
        float input_width = ImGui::GetWindowContentRegionWidth() - 150.f;

        ImGui::Text("Create a new Project and start a new Adventure");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::BeginChild("project form", ImVec2(0.f, 0.f), true);
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::Text("Workspace");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);

        //load workpsace
        const auto worksapceJson = loadJson(getPath({"setting", "workspace_setting"}));

        std::vector<std::string> workspaceTab;

        for(auto workspace : worksapceJson)
        {
            workspaceTab.push_back(workspace["workspace_name"].get<std::string>());
        }

        if(workspaceTab.empty())
        {
            workspaceTab.push_back("There is no Workspace availabled");
        }


        //std::vector<std::string> workspaceTab = {"Learn Nero 01", "Workspace 01", "Workspace 02"};

        std::function<void (const char**, std::vector<std::string>)> fillArray = [](const char** charTab, std::vector<std::string> stringTab)
        {
            for(std::size_t i = 0; i < stringTab.size(); i++)
            {
                charTab[i] = const_cast<const char*>(stringTab[i].c_str());
            }
        };


        std::size_t worskpaceCount = workspaceTab.size();
        const char** items = new const char* [worskpaceCount];

        fillArray(items, workspaceTab);
        //items[workspaceTab.size()] = "";


        m_SelectedWorkpsapce = items[m_SelectedWorkpsapceIdex];
        if (ImGui::BeginCombo("##combo", m_SelectedWorkpsapce, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < worskpaceCount; n++)
            {
                bool is_selected = (m_SelectedWorkpsapce == items[n]);


                if (ImGui::Selectable(items[n], is_selected))
                {
                    m_SelectedWorkpsapce = items[n];
                    m_SelectedWorkpsapceIdex = n;
                }

                if (is_selected)
                {
                     ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        /*for (std::size_t i = 0 ; i < worskpaceCount; i++)
          delete[] items[i] ;
        delete[] items ;
        items = nullptr;*/

        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Project Type");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);



        const char* project_type[] = {"CPP Project", "Lua Project", "CPP and Lua Project"};
        static const char* selected_project_type = project_type[0];            // Here our selection is a single pointer stored outside the object.
        if (ImGui::BeginCombo("##project_type_combo", selected_project_type, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(project_type); n++)
            {
                bool is_selected = (selected_project_type == project_type[n]);
                if (ImGui::Selectable(project_type[n], is_selected))
                    selected_project_type = project_type[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Project Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##project_name", m_InputProjectName, IM_ARRAYSIZE(m_InputProjectName));
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

            std::string workspace =  std::string(m_SelectedWorkpsapce);
             //toString(items[m_SelectedWorkpsapceIdex]);

            projectJson["project_name"]         = std::string(m_InputProjectName);
            projectJson["project_lead"]         = std::string(m_InputProjectLead);
            projectJson["project_company"]      = std::string(m_InputProjectCompany);
            projectJson["project_description"]  = std::string(m_InputProjectDescription);
            projectJson["workspace_name"]    = workspace;


            memset(m_InputProjectName, 0, sizeof m_InputProjectName);
            memset(m_InputProjectLead, 0, sizeof m_InputProjectLead);
            memset(m_InputProjectCompany, 0, sizeof m_InputProjectCompany);
            memset(m_InputProjectDescription, 0, sizeof m_InputProjectDescription);

            createProject(projectJson);

            ImGui::CloseCurrentPopup();
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

    void Interface::showOpenPorjectWindow()
    {

    }

    void Interface::showRecentProjectWindow()
    {

    }

    void Interface::showWorkspaceWindow()
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
                printf("Error: %s\n", NFD_GetError() );
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
            ImGui::OpenPopup(ViewPool.ERROR_CREATING_WORKSPACE.c_str());
        }
        else if(onCreate)
        {
            nlohmann::json workspaceJson;

            workspaceJson["workspace_folder"]       = std::string(m_InputWorksapceFolder);
            workspaceJson["workspace_name"]         = std::string(m_InputWorkspaceName);
            workspaceJson["default_project_lead"]         = std::string(m_InputWorkspaceLead);
            workspaceJson["default_company_name"]      = std::string(m_InputWorkspaceCompany);

            memset(m_InputWorksapceFolder, 0, sizeof m_InputWorksapceFolder);
            memset(m_InputWorkspaceName, 0, sizeof m_InputWorkspaceName);
            memset(m_InputWorkspaceLead, 0, sizeof m_InputWorkspaceLead);
            memset(m_InputWorkspaceCompany, 0, sizeof m_InputWorkspaceCompany);

            createWorkspace(workspaceJson);

            //ImGui::CloseCurrentPopup();
        }

        ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
        if(ImGui::BeginPopupModal(ViewPool.ERROR_CREATING_WORKSPACE.c_str()))
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

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::BeginChild("workspace list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGui::EndChild();
    }

    void Interface::createWorkspace(const nlohmann::json& workspaceJson)
    {
        nero_log(workspaceJson.dump(3));
        m_ProjectManager->createWorkspace(workspaceJson);
    }

    void Interface::createProject(const nlohmann::json& projectJson)
    {
        m_ProjectManager->createProject(projectJson);
    }

    void Interface::openProject(const std::string& path)
    {
        //Open a Lua project or Select a C++, C++/Lua project
    }

    void Interface::compileProject()
    {
        m_ProjectManager->compileProject();
    }

    void Interface::editProject()
    {
        m_ProjectManager->editProject();
    }

    void Interface:: reloadProject()
    {
        m_ProjectManager->loadLibrary();
    }




    ////////////////////////Project and Workspace End////////////////////////

    /*void Interface::addScene(const std::string& projectName, std::function<Scene::Ptr(Scene::Context)> factory)
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

    void Interface::addLuaScene(const std::string& projectName, std::function<LuaScene::Ptr(Scene::Context)> factory)
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

    void Interface::setEditorSetting(const nlohmann::json& setting)
    {
        m_EditorSetting = setting;

        m_ProjectManager->setEditorSetting(m_EditorSetting);

        //
        std::string default_value = m_EditorSetting["project_lead"].get<std::string>();
        std::strcpy(m_InputProjectLead, default_value.c_str());

        default_value = m_EditorSetting["project_company"].get<std::string>();
        std::strcpy(m_InputProjectCompany, default_value.c_str());

        default_value = m_EditorSetting["project_description"].get<std::string>();
        std::strcpy(m_InputProjectDescription, default_value.c_str());
    }

    void Interface::loadAllProject()
    {
        m_ProjectManager->loadAllProject();
    }

    void Interface::openProject(const nlohmann::json& projectJson)
    {

    }

    sf::Sprite Interface::flipTexture(const sf::Texture& texture)
    {
        sf::Vector2u size = texture.getSize();
        sf::Sprite sprite(texture, sf::IntRect(0, size.y, size.x, -size.y));

        return sprite;
    }

    void Interface::showLogWindow()
    {

        ImGui::Begin("Logging");

        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        m_LoggerApplication.Draw("Logging");
    }

    void Interface::showResourceCategoryWindow()
    {
        ImGui::Begin("Resource");

        if(ImGui::Button("Sprite##open_sprite_resource", ImVec2(100.f, 0.f)))
        {
            open_sprite_browser = true;
        }

        ImGui::End();
    }

    void Interface::showResourceWindow()
    {
        //project manager window
        if(open_sprite_browser)
        {
            ImGui::Begin("Resource Browser", nullptr, ImGuiWindowFlags());

                if(ImGui::Button("Import File##import_sprite_resource", ImVec2(100.f, 0.f)))
                {
                    nfdchar_t *outPath = nullptr;
                    nfdresult_t result = NFD_OpenDialog( nullptr, nullptr, &outPath );


                    if ( result == NFD_OKAY ) {
                        puts("Success!");
                        puts(outPath);
                        free(outPath);

                        nero_log(outPath);

                        //openProject(std::string(outPath));
                    }
                    else if ( result == NFD_CANCEL ) {
                        puts("User pressed cancel.");
                    }
                    else {
                        printf("Error: %s\n", NFD_GetError() );
                    }
                }

                ImGui::SameLine();

                if(ImGui::Button("Import Folder##import2_sprite_resource", ImVec2(100.f, 0.f)))
                {
                    nfdchar_t *outPath = nullptr;
                    nfdresult_t result = NFD_PickFolder(nullptr, &outPath); //NFD_OpenDialog( nullptr, nullptr, &outPath );


                    if ( result == NFD_OKAY ) {
                        puts("Success!");
                        puts(outPath);
                        free(outPath);

                        nero_log(outPath);

                        //openProject(std::string(outPath));
                    }
                    else if ( result == NFD_CANCEL ) {
                        puts("User pressed cancel.");
                    }
                    else {
                        printf("Error: %s\n", NFD_GetError() );
                    }
                }

                ImGui::SameLine();


                if(ImGui::Button("Close##close_sprite_resource", ImVec2(60.f, 0.f)))
                {
                    open_sprite_browser = false;
                }

                ImGui::Separator();

            ImGui::End();
        }


    }

    void Interface::showUtilityWindow()
    {
        ImGui::Begin(ViewPool.UTILITY.c_str());

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

    void Interface::showSceneScreenWindow()
    {
        ImGui::Begin(ViewPool.SCENE_SCREEN.c_str());

            ImGui::Text("Manage Scene Screens");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.f, 5.f));

            ImVec2 button_size = ImVec2(75.f, 0.f);
            //addcreen
            if(ImGui::Button("Add##add_screen", button_size))
            {
                //
            }

            ImGui::SameLine();

            static char new_screen_name[100] = "";
            ImGui::SetNextItemWidth(130.f);
            ImGui::InputText("##screen_did", new_screen_name, IM_ARRAYSIZE(new_screen_name));

            if(ImGui::Button("Rename##add_screen", button_size))
            {
                //
            }

            ImGui::SameLine();

            static char rename_screen_name[100] = "";
            ImGui::SetNextItemWidth(130.f);
            ImGui::InputText("##screens_id", rename_screen_name, IM_ARRAYSIZE(rename_screen_name));

            if(ImGui::Button("Duplicate##add_screen", button_size))
            {
                //
            }

            ImGui::SameLine();

            static char duplicate_screen_name[100] = "";
            ImGui::SetNextItemWidth(130.f);
            ImGui::InputText("##screenssq_id", duplicate_screen_name, IM_ARRAYSIZE(duplicate_screen_name));

            //rename screen
            //copy

            ImGui::Dummy(ImVec2(0.f, 5.f));

            ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
            ImGui::BeginChild("scene_screen_list", ImVec2(), true, flags);

                static bool check = false;
                ImGui::Checkbox("", &check);

                ImGui::SameLine();

                ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_ReadOnly;
                static char screen_name[100] = "screen one";
                ImGui::SetNextItemWidth(120.f);
                ImGui::InputText("##screen_id", screen_name, IM_ARRAYSIZE(screen_name), flags2);

                ImGui::SameLine();

                if(ImGui::Button("X##detele_screen"))
                {
                    //
                }

           ImGui::EndChild();

        ImGui::End();
    }

    void Interface::showSceneChunckWindow()
    {
        ImGui::Begin(ViewPool.SCENE_CHUNCK.c_str());

        ImGui::Text("Manage Scene Chuncks");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImVec2 button_size = ImVec2(75.f, 0.f);
        //addchunk
        if(ImGui::Button("Add##add_chunk", button_size))
        {
            //
        }

        ImGui::SameLine();

        static char new_screen_name[100] = "";
        ImGui::SetNextItemWidth(130.f);
        ImGui::InputText("##chucnk_did", new_screen_name, IM_ARRAYSIZE(new_screen_name));

        if(ImGui::Button("Rename##rename_chunck", button_size))
        {
            //
        }

        ImGui::SameLine();

        static char rename_screen_name[100] = "";
        ImGui::SetNextItemWidth(130.f);
        ImGui::InputText("##screesns_id", rename_screen_name, IM_ARRAYSIZE(rename_screen_name));

        if(ImGui::Button("Duplicate##add_ssfcreen", button_size))
        {
            //
        }

        ImGui::SameLine();

        static char duplicate_screen_name[100] = "";
        ImGui::SetNextItemWidth(130.f);
        ImGui::InputText("##screensdsfssq_id", duplicate_screen_name, IM_ARRAYSIZE(duplicate_screen_name));

        //rename screen
        //copy

        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
        ImGui::BeginChild("chunck_screen_list", ImVec2(), true, flags);

            static bool toggle = false;
            std::string label = toggle ? "hide" : "show";
            showToggleButton(toggle, label, [this]()
            {
                toggle = !toggle;
            });

            ImGui::SameLine();

            static bool check = false;
            ImGui::Checkbox("", &check);

            ImGui::SameLine();

            ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_ReadOnly;
            static char screen_name[100] = "screen one";
            ImGui::SetNextItemWidth(120.f);
            ImGui::InputText("##scrdsdeen_id", screen_name, IM_ARRAYSIZE(screen_name), flags2);

            ImGui::SameLine();

            if(ImGui::Button("X##deteldde_screen"))
            {
                //
            }

            ImGui::EndChild();


        ImGui::End();
    }

    void Interface::showToggleButton(bool toggle, const std::string& label, std::function<void()> callback)
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

    void Interface::interfaceFirstDraw()
    {
        if(!m_InterfaceFirstDraw)
        {
            return;
        }

        auto right_node = ImGui::DockBuilderGetNode(dock_id_right);
        right_node->TabBar->NextSelectedTabId = right_node->TabBar->Tabs.front().ID;

        auto upper_left_node = ImGui::DockBuilderGetNode(dock_id_upper_left);
        upper_left_node->TabBar->NextSelectedTabId = right_node->TabBar->Tabs.front().ID;

        ImGui::SetWindowFocus(ViewPool.SCENE.c_str());

        //commit
        m_InterfaceFirstDraw = false;
    }

    void Interface::showCurrentSceneWindow()
    {
        ImGui::Begin("Scene Tools");

            ImGui::BeginChild("booosdfsdf", ImVec2(ImGui::GetContentRegionAvailWidth() * 0.5f, 0.f), true);

                ImGui::Text("Scene Screen");
                ImGui::Separator();

                static bool toggle_screen = false;
                std::string label = toggle_screen ? "Screen View" : "Screen View";
                showToggleButton(toggle_screen, label, [this]()
                {
                    toggle_screen = !toggle_screen;
                });

                ImGui::Dummy(ImVec2(0.f, 5.f));

                ImGui::Text("Mesh");
                ImGui::Separator();

                ImGui::Dummy(ImVec2(0.f, 5.f));

                ImGui::Text("Object");
                ImGui::Separator();

                ImGui::Dummy(ImVec2(0.f, 5.f));

                ImGui::Text("Layer");
                ImGui::Separator();

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("saerzr", ImVec2(), true);
                ImGui::Text("Scene Chunck");
                ImGui::Separator();

                ImGui::Dummy(ImVec2(0.f, 5.f));

                ImGui::Text("Draw");
                ImGui::Separator();

            ImGui::EndChild();

        ImGui::End();
    }
}
