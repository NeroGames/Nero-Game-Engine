////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/WorkspaceListView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    WorkspaceListView::WorkspaceListView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    WorkspaceListView::~WorkspaceListView()
    {
        destroy();
    }

    void WorkspaceListView::destroy()
    {
    }

    void WorkspaceListView::render()
    {
        float            wording_width = 150.f;

        ImGuiWindowFlags window_flag   = ImGuiWindowFlags_None;
        window_flag |=
            ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
        ImGui::BeginChild("##workspace_list", ImVec2(0.f, 0.f), true, window_flag);
        ImGui::Dummy(ImVec2(0.0f, 4.0f));
        for(const nlohmann::json& worksapce :
            m_EditorContext->getProjectManager()->getWorkspaceTable())
        {
            if(ImGui::CollapsingHeader(worksapce["workspace_name"].get<std::string>().c_str()))
            {
                std::string project_lead = ": " + worksapce["project_lead"].get<std::string>();
                std::string company_name = ": " + worksapce["company_name"].get<std::string>();
                std::string project_namespace =
                    ": " + worksapce["project_namespace"].get<std::string>();
                std::string workspace_directory =
                    ": " + worksapce["workspace_directory"].get<std::string>();

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
} // namespace nero
