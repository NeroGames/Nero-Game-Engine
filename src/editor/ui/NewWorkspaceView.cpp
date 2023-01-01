////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/NewWorkspaceView.h>
#include <Nero/editor/EditorUtility.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    NewWorkspaceView::NewWorkspaceView(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
    {
        clearInput();
    }

    NewWorkspaceView::~NewWorkspaceView()
    {
        destroy();
    }

    void NewWorkspaceView::destroy()
    {

    }

    void NewWorkspaceView::render()
    {
        float wording_width = 150.f;
        float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

        //create workspace
        ImGui::Text("Location");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width - 65.f);
        ImGui::InputText("##workspace_location", m_Input.location, sizeof(m_Input.location), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine(wording_width + input_width - 60.f);
        if(ImGui::Button("Browse##choose_workspace_location", ImVec2(60, 0)))
        {
            file::selectDirectory([this](std::string outPath)
            {
                string::fillCharArray(m_Input.location, sizeof(m_Input.location), outPath);
            });
        }
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Workspace Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_name", m_Input.name, sizeof( m_Input.name));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Company Name");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_company",  m_Input.company, sizeof( m_Input.company));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Project Lead");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_lead",  m_Input.projectLead, sizeof( m_Input.projectLead));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Text("Project Namespace");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText("##workspace_namespace",  m_Input.projectNamespace, sizeof( m_Input.projectNamespace));
        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::SameLine(wording_width + input_width - 130.f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);

        bool onCreate = ImGui::Button("Create Workspace", ImVec2(130.f, 0));

        if(onCreate)
        {
            m_Input.errorMessage = StringPool.BLANK;
            m_Input.redirectLink = StringPool.BLANK;
            m_Input.error = true;

            //workpspace location blank
            if(std::string(m_Input.location) == StringPool.BLANK)
            {
                m_Input.errorMessage   = "Please select a Location Directory";
                m_Input.redirectLink   = StringPool.BLANK;
            }
            //workpspace location not valid
            else if(!file::directoryExist(std::string(m_Input.location)))
            {
                m_Input.errorMessage   = "The selected location is not a valid directory";
                m_Input.redirectLink   = StringPool.BLANK;
            }
            //workpspace name blank
            else if(std::string(m_Input.name) == StringPool.BLANK)
            {
                m_Input.errorMessage   = "Please enter a Workspace Name";
                m_Input.redirectLink   = StringPool.BLANK;
            }
            //workpspace name not matching regex
            else if(!string::matchPattern(std::string(m_Input.name), StringPool.REGEX_NAME_01))
            {
                m_Input.errorMessage = "Invalid Workspace Name";
                m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-workspace#workspace_name";
            }
            //workpspace already exist
            else if(m_EditorContext->getProjectManager()->workspaceExist(m_Input.name))
            {
                m_Input.errorMessage   = "A Workspace with the same Id already exist, Please choose another Name";
                m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-workspace#workspace_name";
            }
            //workpspace company name blank
            else if(std::string(m_Input.company) == StringPool.BLANK)
            {
                m_Input.errorMessage   = "Please enter a Company Name";
                m_Input.redirectLink   = StringPool.BLANK;
            }
            //workpspace company not matching regex
            else if(!string::matchPattern(std::string(m_Input.company), StringPool.REGEX_NAME_01))
            {
                m_Input.errorMessage = "Invalid Company Name";
                m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-workspace#company_name";
            }
            //workpspace project lead name blank
            else if(std::string(m_Input.projectLead) == StringPool.BLANK)
            {
                m_Input.errorMessage   = "Please enter a Project Lead";
                m_Input.redirectLink   = StringPool.BLANK;
            }
            //workpspace project lead not matching regex
            else if(!string::matchPattern(std::string(m_Input.projectLead), StringPool.REGEX_NAME_01))
            {
                m_Input.errorMessage   = "Invalid Project Lead";
                m_Input.redirectLink   = "https://nero-games.com/learn/engine-v2/create-workspace#project_lead";
            }
            //workpspace namespace blank
            else if(std::string(m_Input.projectNamespace) == StringPool.BLANK)
            {
                m_Input.errorMessage   = "Please enter a Project Namesapce";
                m_Input.redirectLink   = StringPool.BLANK;
            }
            //workpspace namespace not matching regex
            else if(!string::matchPattern(std::string(m_Input.projectNamespace), StringPool.REGEX_NAMESPACE))
            {
                m_Input.errorMessage = "Invalid Project Namespace";
                m_Input.redirectLink = "https://nero-games.com/learn/engine-v2/create-workspace#namespace";
            }
            else
            {
                m_Input.error = false;
            }
        }

        if (onCreate && m_Input.error)
        {
            ImGui::OpenPopup(EditorConstant.MODAL_ERROR_CREATING_WORKSPACE.c_str());
        }
        else if(onCreate)
        {
            Parameter parameter;

            parameter.setString("workspace_location",	std::string(m_Input.location));
            parameter.setString("workspace_name",		std::string(m_Input.name));
            parameter.setString("company_name",			std::string(m_Input.company));
            parameter.setString("project_lead",			std::string(m_Input.projectLead));
            parameter.setString("project_namespace",	std::string(m_Input.projectNamespace));

            clearInput();

            m_EditorContext->getEditorProxy()->createWorkspace(parameter);
        }

        ImGuiWindowFlags modal_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
        if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_CREATING_WORKSPACE.c_str(), nullptr, modal_flags))
        {
            ImGui::TextWrapped("%s", m_Input.errorMessage.c_str());

            if(m_Input.redirectLink != StringPool.BLANK)
            {
                ImGui::Dummy(ImVec2(0.f, 5.f));

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
    }

    void NewWorkspaceView::clearInput()
    {
        string::fillCharArray(m_Input.location,			sizeof(m_Input.location),			StringPool.BLANK);
        string::fillCharArray(m_Input.name,				sizeof(m_Input.name),				StringPool.BLANK);
        string::fillCharArray(m_Input.projectLead,		sizeof(m_Input.projectLead),		StringPool.BLANK);
        string::fillCharArray(m_Input.company,			sizeof(m_Input.company),			StringPool.BLANK);
        string::fillCharArray(m_Input.projectNamespace,	sizeof(m_Input.projectNamespace),	StringPool.BLANK);
        string::fillCharArray(m_Input.locationImport,	sizeof(m_Input.locationImport),		StringPool.BLANK);
    }
}
