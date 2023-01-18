////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ImportWorkspaceView.h>
#include <Nero/editor/EditorUtility.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ImportWorkspaceView::ImportWorkspaceView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
        clearInput();
    }

    ImportWorkspaceView::~ImportWorkspaceView()
    {
        destroy();
    }

    void ImportWorkspaceView::destroy()
    {
        clearInput();
    }

    void ImportWorkspaceView::render()
    {
        float wordingWidth = 150.f;
        float inputWidth   = ImGui::GetWindowContentRegionWidth() - wordingWidth;

        // import workspace
        ImGui::Text("Workspace Directory");
        ImGui::SameLine(wordingWidth);
        ImGui::SetNextItemWidth(inputWidth - 130.f);
        ImGui::InputText("##workspace_import",
                         m_Input.locationImport,
                         sizeof(m_Input.locationImport),
                         ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine(wordingWidth + inputWidth - 124.f);

        if(ImGui::Button("Browse##choose_workspace_import", ImVec2(60.f, 0)))
        {
            file::selectDirectory([this](std::string outPath) {
                string::fillCharArray(m_Input.locationImport, sizeof(m_Input.locationImport), outPath);
            });
        }

        ImGui::SameLine(wordingWidth + inputWidth - 60.f);
        bool onImport = ImGui::Button("Import##import_workspace", ImVec2(60.f, 0));

        if(onImport)
        {
            m_Input.errorMessage = StringPool.BLANK;
            m_Input.redirectLink = StringPool.BLANK;
            m_Input.error        = true;

            // workpspace location blank
            if(std::string(m_Input.locationImport) == StringPool.BLANK)
            {
                m_Input.errorMessage = "Please select a Workspace Location";
            }
            // workpspace location not valid
            else if(!file::directoryExist(std::string(m_Input.locationImport)))
            {
                m_Input.errorMessage = "The selected location is not a valid directory";
            }
            // workpspace already exist
            else if(m_EditorContext->getProjectManager()->workspaceExist(file::getFileName(std::string(m_Input.locationImport))))
            {
                m_Input.errorMessage = "A Workspace with the same Id already exist, Please choose another Name";
            }
            // workpspace location not valid
            else if(!file::fileExist(file::getPath({std::string(m_Input.locationImport), ".workspace"})))
            {
                m_Input.errorMessage = "The selected directory is not a valid Workspace";
            }
            else if(!workspaceDocumentValid(std::string(m_Input.locationImport)))
            {
                m_Input.errorMessage = "Invalid workspace document";
            }
            else
            {
                m_Input.error = false;
            }
        }

        if(onImport && m_Input.error)
        {
            ImGui::OpenPopup(EditorConstant.MODAL_ERROR_IMPORTING_WORKSPACE.c_str());
        }
        else if(onImport)
        {
            // import workspace
            m_EditorContext->getEditorProxy()->importWorkspace(std::string(m_Input.locationImport));
            // clear input
            string::fillCharArray(m_Input.locationImport, sizeof(m_Input.locationImport), StringPool.BLANK);
        }

        ImGuiWindowFlags modal_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
        if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_IMPORTING_WORKSPACE.c_str(), nullptr, modal_flags))
        {
            ImGui::TextWrapped("%s", m_Input.errorMessage.c_str());

            if(m_Input.redirectLink != StringPool.BLANK)
            {
                ImGui::Dummy(ImVec2(0.f, 5.f));

                ImGui::SetCursorPosX((300.f - 150.f) / 2.f);
                if(ImGui::Button("Learn more here", ImVec2(150.f, 0.f)))
                {
                    cmd::launchBrowser(m_Input.redirectLink);
                }
            }

            ImGui::Dummy(ImVec2(0.0f, 35.0f));

            ImGui::SetCursorPosY(130.f - 30.f);
            ImGui::SetCursorPosX((300.f - 75.f) / 2.f);
            if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void ImportWorkspaceView::clearInput()
    {
        string::fillCharArray(m_Input.locationImport, sizeof(m_Input.locationImport), StringPool.BLANK);
    }

    bool ImportWorkspaceView::workspaceDocumentValid(const std::string& workspaceDirectory)
    {
        auto              document         = file::loadJson(file::getPath({workspaceDirectory, ".workspace"}), true);
        const std::string projectNamespace = document["project_namespace"].get<std::string>();

        bool              documentValid =
            // Check document type
            document["document_type"].get<std::string>() == "nero_game_workspace" &&
            // Check engine version
            document["engine_version"].get<std::string>() == "2.0.0" &&
            // Check workspace name
            document["workspace_name"].get<std::string>() == file::getFileName(workspaceDirectory) &&
            // Check namespace
            projectNamespace != StringPool.BLANK && string::matchPattern(std::string(projectNamespace), StringPool.REGEX_NAMESPACE);

        return documentValid;
    }
} // namespace nero
