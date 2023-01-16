////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/NewProjectTab.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    NewProjectTab::NewProjectTab(EditorContext::Ptr editorContext,
                                       TabSelectionHandler::Ptr tabSelectionHandler):
         UIComponent(std::move(editorContext))
        ,m_TabSelectionHandler(tabSelectionHandler)
        ,m_NewProjectCount(0)
    {
        clearInput();
    }

    NewProjectTab::~NewProjectTab()
    {
        destroy();
    }

    void NewProjectTab::destroy()
    {
        clearInput();
    }

    void NewProjectTab::render()
    {
        // Udpate Input using workspace data
        const auto workspaceTable = m_EditorContext->getProjectManager()->getWorkspaceTable();
        if(!workspaceTable.empty() &&
           m_Input.projectNamespace == StringPool.BLANK &&
           m_Input.projectLead == StringPool.BLANK &&
           m_Input.company == StringPool.BLANK)
        {
            updateInput(workspaceTable);
        }

        //Window flags
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking |
                                       ImGuiWindowFlags_Modal |
                                       ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoCollapse |
                                       ImGuiWindowFlags_NoScrollbar;
        //Winsow size
        ImVec2 winsowSize   = EditorConstant.WINDOW_PROJECT_MANAGER_SIZE;
        float wordingWidth  = 130.f;
        float inputWidth    = ImGui::GetWindowContentRegionWidth() - 150.f;

        ImGui::Text("Create a new Project and start a new Adventure");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::BeginChild("project form", ImVec2(0.f, 0.f), true);
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::Text("Project Name");
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);
            ImGui::InputText("##project_name", m_Input.name, sizeof(m_Input.name));
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::Text("Startup Pack");
            ImGui::SameLine(wordingWidth);
            ImGui::Checkbox("##starter_pack", &m_Input.startupPack);
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::Text("Workspace");
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);

            //load workpsace
            std::vector<std::string> workspaceNameTable = m_EditorContext->getProjectManager()->getWorkspaceNameTable();

            std::size_t worskpaceCount = workspaceNameTable.size();

            const char* workspaceComboTable[worskpaceCount];
            nero_fill_char_array(workspaceComboTable, workspaceNameTable);

            m_Input.workspace = workspaceComboTable[m_Input.workspaceIndex];
            if (ImGui::BeginCombo("##workspace_combo", m_Input.workspace, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
            {
                for (std::size_t n = 0; n < worskpaceCount; n++)
                {
                    bool is_selected = (m_Input.workspace == workspaceComboTable[n]);


                    if (ImGui::Selectable(workspaceComboTable[n], is_selected))
                    {
                        m_Input.workspace = workspaceComboTable[n];
                        m_Input.workspaceIndex = n;

                        auto workspace = m_EditorContext->getProjectManager()->findWorkspace(workspaceNameTable[n]);

                        string::fillCharArray(m_Input.company,
                                              sizeof(m_Input.company),
                                              workspace["company_name"].get<std::string>());
                        string::fillCharArray(m_Input.projectLead,
                                              sizeof(m_Input.projectLead),
                                              workspace["project_lead"].get<std::string>());
                        string::fillCharArray(m_Input.projectNamespace,
                                              sizeof(m_Input.projectNamespace),
                                              workspace["project_namespace"].get<std::string>());
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
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);
            const char* projectTypeComboTable[] = {"CPP Project"}; //Demo //{"CPP Project", "Lua Project", "CPP and Lua Project"};
            m_Input.projectType = projectTypeComboTable[m_Input.projectTypeIndex];            // Here our selection is a single pointer stored outside the object.
            if (ImGui::BeginCombo("##project_type_combo", m_Input.projectType, ImGuiComboFlags())) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(projectTypeComboTable); n++)
                {
                    bool is_selected = (m_Input.projectType == projectTypeComboTable[n]);

                    if (ImGui::Selectable(projectTypeComboTable[n], is_selected))
                    {
                        m_Input.projectType = projectTypeComboTable[n];
                        m_Input.projectTypeIndex = n;
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
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);

            std::vector<std::string> codeEditorTable;
            if(m_EditorContext->getEditorSetting()->getSetting("environment").getString("qt_creator") != StringPool.BLANK)
            {
                codeEditorTable.push_back("Qt Creator");
            }

            if(m_EditorContext->getEditorSetting()->getSetting("environment").getString("visual_studio") != StringPool.BLANK)
            {
                codeEditorTable.push_back("Visual Studio");
            }

            std::size_t editorCount = codeEditorTable.size();

            const char* codeEditorComboTable[editorCount];
            nero_fill_char_array(codeEditorComboTable, codeEditorTable);

            m_Input.codeEditor = codeEditorComboTable[m_Input.codeEditorIndex];
            if (ImGui::BeginCombo("##code-editor-combo", m_Input.codeEditor, ImGuiComboFlags()))
            {
                for (int n = 0; n < IM_ARRAYSIZE(codeEditorComboTable); n++)
                {
                    bool is_selected = (m_Input.codeEditor == codeEditorComboTable[n]);

                    if (ImGui::Selectable(codeEditorComboTable[n], is_selected))
                    {
                        m_Input.codeEditor = codeEditorComboTable[n];
                        m_Input.codeEditorIndex = n;
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
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);
            ImGui::InputText("##project_lead", m_Input.projectLead, IM_ARRAYSIZE(m_Input.projectLead));
            ImGui::Dummy(ImVec2(0.0f, 2.0f));

            ImGui::Text("Company Name");
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);
            ImGui::InputText("##project_company", m_Input.company, IM_ARRAYSIZE(m_Input.company));
            ImGui::Dummy(ImVec2(0.0f, 2.0f));

            ImGui::Text("Namesapce");
            ImGui::SameLine(wordingWidth);
            ImGui::SetNextItemWidth(inputWidth);
            ImGui::InputText("##project_namespace", m_Input.projectNamespace, IM_ARRAYSIZE(m_Input.projectNamespace));
            ImGui::Dummy(ImVec2(0.0f, 2.0f));


            ImGui::Text("Description");
            ImGui::SameLine(wordingWidth);
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            ImGui::InputTextMultiline("##project_description", m_Input.description, IM_ARRAYSIZE(m_Input.description), ImVec2(inputWidth, ImGui::GetTextLineHeight() * 5), flags);
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
            ImGui::SetCursorPosY(winsowSize.y * 0.85f - 95.f);
            bool onCreate = ImGui::Button("Create", ImVec2(100, 0));

            if(onCreate)
            {
                m_Input.errorMessage = StringPool.BLANK;
                m_Input.redirectLink = StringPool.BLANK;
                m_Input.error = true;

                //Project name blank
                if(std::string(m_Input.name) == StringPool.BLANK)
                {
                    m_Input.errorMessage = "Please enter a Project Name";
                }
                //Project name does not match regex
                else if(!string::matchPattern(std::string(m_Input.name), StringPool.REGEX_NAME_02))
                {
                    m_Input.errorMessage = "Invalid Project Name";
                    m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-project#project_name";
                }
                else if(m_EditorContext->getProjectManager()->projectExist(std::string(m_Input.name), std::string(m_Input.workspace)))
                {
                    m_Input.errorMessage = "A project with the same Id already exist, please choose another Project Name";
                    m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-project#project_name";
                }
                //Worksapce blank
                else if(std::string(m_Input.workspace) == StringPool.BLANK)
                {
                    m_Input.errorMessage = "Please select a Workspace";
                }
                //Project type blank
                else if(std::string(m_Input.projectType) == StringPool.BLANK)
                {
                    m_Input.errorMessage = "Please select a Project Type";
                }
                //Code Editor blank
                else if(std::string(m_Input.codeEditor) == StringPool.BLANK)
                {
                    m_Input.errorMessage = "Please select a Code Editor";
                }
                //workpspace company name blank
                else if(std::string(m_Input.company) == StringPool.BLANK)
                {
                    m_Input.errorMessage	= "Please enter a Company Name";
                    m_Input.redirectLink	= StringPool.BLANK;
                }
                //workpspace company not matching regex
                else if(!string::matchPattern(std::string(m_Input.company), StringPool.REGEX_NAME_01))
                {
                    m_Input.errorMessage = "Invalid Company Name";
                    m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-project#company_name";
                }
                //workpspace project lead name blank
                else if(std::string(m_Input.projectLead) == StringPool.BLANK)
                {
                    m_Input.errorMessage	= "Please enter a Project Lead";
                    m_Input.redirectLink	= StringPool.BLANK;
                }
                //workpspace project lead not matching regex
                else if(!string::matchPattern(std::string(m_Input.projectLead), StringPool.REGEX_NAME_01))
                {
                    m_Input.errorMessage	= "Invalid Project Lead";
                    m_Input.redirectLink	= "https://nero-games.com/learn/engine-v2/create-project#project_lead";
                }
                //workpspace namespace blank
                else if(std::string(m_Input.projectNamespace) == StringPool.BLANK)
                {
                    m_Input.errorMessage	= "Please enter a Project Namesapce";
                    m_Input.redirectLink	= StringPool.BLANK;
                }
                //workpspace namespace not matching regex
                else if(!string::matchPattern(std::string(m_Input.projectNamespace), StringPool.REGEX_NAMESPACE))
                {
                    m_Input.errorMessage = "Invalid Project Namespace";
                    m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-project#namespace";
                }
                else
                {
                    m_Input.error = false;
                }
            }

            if (onCreate && m_Input.error)
            {
                ImGui::OpenPopup(EditorConstant.MODAL_ERROR_CREATING_PROJECT.c_str());
            }
            else if(onCreate)
            {
                Parameter parameter;
                parameter.setString("project_name",			std::string(m_Input.name));
                parameter.setString("workspace_name",		std::string(m_Input.workspace));
                parameter.setString("project_type",			std::string(m_Input.projectType));
                parameter.setString("project_namespace",	std::string(m_Input.projectNamespace));
                parameter.setString("project_lead",			std::string(m_Input.projectLead));
                parameter.setString("company_name",			std::string(m_Input.company));
                parameter.setString("description",			std::string(m_Input.description));
                parameter.setString("code_editor",			std::string(m_Input.codeEditor));
                parameter.setBool("startup_pack",			m_Input.startupPack);

                m_Input.lastCreatedProject = m_EditorContext->getProjectManager()->getProjectDirectory(parameter);

                clearInput();
                updateInput(m_EditorContext->getProjectManager()->getWorkspaceTable());

                //start new thread
                m_EditorContext->getEditorProxy()->createProject(parameter, m_NewProjectCount++);

                ImGui::OpenPopup(EditorConstant.MODAL_WAITING_PROJECT_CREATION.c_str());
            }

            //Error modal
            ImGuiWindowFlags modal_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                              ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
            ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
            if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_CREATING_PROJECT.c_str(), nullptr, modal_flags))
            {
                ImGui::TextWrapped("%s", m_Input.errorMessage.c_str());

                if(m_Input.redirectLink != StringPool.BLANK)
                {
                    ImGui::SetCursorPosY(130.f - 60.f);
                    ImGui::SetCursorPosX((300.f - 150.f)/2.f);
                    if(ImGui::Button("Learn more here", ImVec2(150.f, 0.f)))
                    {
                        cmd::launchBrowser(m_Input.redirectLink);
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
            if(ImGui::BeginPopupModal(EditorConstant.MODAL_WAITING_PROJECT_CREATION.c_str(), nullptr, windowFlags))
            {
                std::string taskName = EditorConstant.TASK_CREATE_PROJECT + toString(m_NewProjectCount-1);
                BackgroundTask::Ptr createProjectTask = BTManager::findTaskByName(taskName);

                if(createProjectTask)
                {
                    BTManager::pauseTask(taskName, 1);
                    std::string message = createProjectTask->getMessage();

                    ImGui::TextWrapped("%s", message.c_str());

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if(createProjectTask->completed())
                    {
                        ImGui::SetCursorPosY(200.f - 40.f);
                        ImGui::SetCursorPosX(50.f);
                        if (ImGui::Button("Open project", ImVec2(100, 0)))
                        {
                            ImGui::CloseCurrentPopup();
                            ImGui::ClosePopupToLevel(0, true);

                            m_EditorContext->getEditorProxy()->openProject(m_Input.lastCreatedProject);
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

    void NewProjectTab::clearInput()
    {
        string::fillCharArray(m_Input.name,				sizeof(m_Input.name),				StringPool.BLANK);
        string::fillCharArray(m_Input.projectLead,		sizeof(m_Input.projectLead),		StringPool.BLANK);
        string::fillCharArray(m_Input.company,			sizeof(m_Input.company),			StringPool.BLANK);
        string::fillCharArray(m_Input.projectNamespace,	sizeof(m_Input.projectNamespace),	StringPool.BLANK);
        string::fillCharArray(m_Input.description,		sizeof(m_Input.description),		StringPool.BLANK);
        m_Input.projectTypeIndex = 0;
        m_Input.codeEditorIndex = 0;
        m_Input.workspaceIndex = 0;

        m_Input.startupPack = true;
    }

    void NewProjectTab::updateInput(const nlohmann::json workspaceTable)
    {
        if(workspaceTable.empty())
        {
            clearInput();
            return;
        }

        auto workspace = workspaceTable.front();

        string::fillCharArray(m_Input.company,			sizeof(m_Input.company),			workspace["company_name"].get<std::string>());
        string::fillCharArray(m_Input.projectLead,		sizeof(m_Input.projectLead),		workspace["project_lead"].get<std::string>());
        string::fillCharArray(m_Input.projectNamespace,	sizeof(m_Input.projectNamespace),	workspace["project_namespace"].get<std::string>());
        m_Input.workspaceIndex = 0;
    }
}
