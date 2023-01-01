////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/OpenProjectTab.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    OpenProjectTab::OpenProjectTab(EditorContext::Ptr editorContext,
                                   TabSelectionHandler::Ptr tabSelectionHandler):
         UIComponent(editorContext)
        ,m_TabSelectionHandler(tabSelectionHandler)
    {

    }

    OpenProjectTab::~OpenProjectTab()
    {
        destroy();
    }

    void OpenProjectTab::destroy()
    {

    }

    void OpenProjectTab::render()
    {
        float wordingWidth = 200.f;
        float inputWidth	= ImGui::GetWindowContentRegionWidth() - 150.f;

        ImGui::Text("Open a Project and continous where you left");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::BeginChild("project list", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

            ImGui::Dummy(ImVec2(0.f, 10.f));

            for(const std::string workspace : m_EditorContext->getProjectManager()->getWorkspaceNameTable())
            {
                if (ImGui::CollapsingHeader(workspace.c_str()))
                {
                    ImGui::SetCursorPosX(20.f);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    for(const nlohmann::json& project : m_EditorContext->getProjectManager()->getWorkspaceProjectTable(workspace))
                    {
                        std::string projectName = project["project_name"].get<std::string>();

                        ImGui::SetCursorPosX(20.f);
                        if (ImGui::CollapsingHeader(projectName.c_str()))
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
                            ImGui::SameLine(wordingWidth);
                            ImGui::Text(project_company.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Lead");
                            ImGui::SameLine(wordingWidth);
                            ImGui::Text(project_lead.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Create Date");
                            ImGui::SameLine(wordingWidth);
                            ImGui::Text(creation_date.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Modification Date");
                            ImGui::SameLine(wordingWidth);
                            ImGui::Text(modification_date.c_str());
                            ImGui::Dummy(ImVec2(0.0f, 2.0f));

                            ImGui::SetCursorPosX(40.f);
                            ImGui::Text("Project Description");
                            ImGui::SetCursorPosX(40.f);
                            ImGui::BeginChild(projectName.c_str(), ImVec2(0.f, 100.f), true, ImGuiWindowFlags_ScrollToolbar);
                                ImGui::TextWrapped("%s", project_description.c_str());
                            ImGui::EndChild();
                            ImGui::Dummy(ImVec2(0.0f, 5.0f));


                            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 130.f);
                            pushToolbarStyle();
                            if (ImGui::Button("Open Project", ImVec2(125, 30.f)))
                            {
                                m_EditorContext->getEditorProxy()->openProject(
                                            file::getPath({m_EditorContext->getProjectManager()->findWorkspace(workspace)["workspace_directory"].get<std::string>(),
                                                           "Project",
                                                            projectName}));

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
}
