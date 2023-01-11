////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/WorkspaceTab.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    WorkspaceTab::WorkspaceTab(EditorContext::Ptr editorContext,
                               TabSelectionHandler::Ptr tabSelectionHandler):
         UIComponent(std::move(editorContext))
        ,m_TabSelectionHandler(std::move(tabSelectionHandler))
        ,m_NewWorkspaceView(m_EditorContext)
        ,m_ImportWorkspaceView(m_EditorContext)
        ,m_WorkspaceListView(m_EditorContext)
    {

    }

    WorkspaceTab::~WorkspaceTab()
    {
        destroy();
    }

    void WorkspaceTab::destroy()
    {

    }

    void WorkspaceTab::render()
    {
        m_NewWorkspaceView.render();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        m_ImportWorkspaceView.render();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        m_WorkspaceListView.render();
    }
}
