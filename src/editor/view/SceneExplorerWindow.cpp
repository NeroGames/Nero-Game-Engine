////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/SceneExplorerWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    SceneExplorerWindow::SceneExplorerWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_SceneGraphView(m_EditorContext)
        , m_GameObjectPropertyView(m_EditorContext)
    {
    }

    SceneExplorerWindow::~SceneExplorerWindow()
    {
        destroy();
    }

    void SceneExplorerWindow::destroy()
    {
    }

    void SceneExplorerWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_EXPLORER.c_str());

        m_SceneGraphView.render();

        if(m_EditorContext->getEditorMode() != EditorMode::Play_Game)
        {
            m_GameObjectPropertyView.render();
        }

        ImGui::End();
    }
} // namespace nero
