////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorSetupWorkspaceView.h>
// Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorSetupWorkspaceView::EditorSetupWorkspaceView(EditorContext::Ptr editorContext,
                                                       EditorSetup::Ptr   editorSetup)
        : UIComponent(std::move(editorContext))
        , m_EditorSetup(editorSetup)
        , m_NewWorkspaceView(m_EditorContext)
        , m_ImportWorkspaceView(m_EditorContext)
    {
        clearInput();
    }

    EditorSetupWorkspaceView::~EditorSetupWorkspaceView()
    {
        destroy();
    }

    void EditorSetupWorkspaceView::destroy()
    {
        clearInput();
    }

    void EditorSetupWorkspaceView::render()
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::Text("Create or Import Workspace");
        ImGui::PopFont();
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::TextWrapped("Create a new workspace or import an existing one");

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 550.f) / 2.f);

        pushToolbarStyle(!m_Input.importWorkspace);
        if(ImGui::Button("Create", ImVec2(250.f, 50.f)))
        {
            m_Input.importWorkspace = false;
        }
        popToolbarStyle();

        ImGui::SameLine(0.f, 50.f);

        pushToolbarStyle(m_Input.importWorkspace);
        if(ImGui::Button("Import", ImVec2(250.f, 50.f)))
        {
            m_Input.importWorkspace = true;
        }
        popToolbarStyle();

        ImGui::Dummy(ImVec2(0.f, 50.f));

        if(m_Input.importWorkspace)
        {
            m_ImportWorkspaceView.render();
        }
        else
        {
            m_NewWorkspaceView.render();
        }

        m_EditorSetup->setNextSetup(
            !m_EditorContext->getProjectManager()->getWorkspaceTable().empty());
    }

    void EditorSetupWorkspaceView::clearInput()
    {
        m_Input.importWorkspace = false;
    }
} // namespace nero
