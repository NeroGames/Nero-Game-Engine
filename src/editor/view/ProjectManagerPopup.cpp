////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/ProjectManagerPopup.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ProjectManagerPopup::ProjectManagerPopup(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
        ,m_TabSelectionHandler(std::make_shared<TabSelectionHandler>())
        ,m_RecentProjectTab(editorContext, m_TabSelectionHandler)
        ,m_NewProjectTab(editorContext, m_TabSelectionHandler)
        ,m_OpenProjectTab(editorContext, m_TabSelectionHandler)
        ,m_WorkspaceTab(editorContext, m_TabSelectionHandler)
    {
        m_TabSelectionHandler->registerTab(
        {
            EditorConstant.TAB_RECENT_PROJECT,
            EditorConstant.TAB_CREATE_PROJECT,
            EditorConstant.TAB_OPEN_PROJECT,
            EditorConstant.TAB_WORKSPACE
        });
    }

    ProjectManagerPopup::~ProjectManagerPopup()
    {
        destroy();
    }

    void ProjectManagerPopup::destroy()
    {

    }

    TabSelectionHandler::Ptr ProjectManagerPopup::getTabSelectionHandler() const
    {
        return m_TabSelectionHandler;
    }

    void ProjectManagerPopup::render()
    {
        // Window flags
        ImGuiWindowFlags windowFlags   = ImGuiWindowFlags_NoDocking |
                                         ImGuiWindowFlags_Modal |
                                         ImGuiWindowFlags_NoResize |
                                         ImGuiWindowFlags_NoCollapse |
                                         ImGuiWindowFlags_NoScrollbar;
        //Winsow size
        ImVec2 winsowSize              = EditorConstant.WINDOW_PROJECT_MANAGER_SIZE;

        //Project manager window
        ImGui::SetNextWindowSize(winsowSize);
        //Begin window
        if(ImGui::BeginPopupModal(EditorConstant.WINDOW_PROJECT_MANAGER.c_str(), nullptr, windowFlags))
        {
            // Save cursor position
            ImVec2 cursor = ImGui::GetCursorPos();

            //Panel 1 : Engine Banner
            ImGui::BeginChild("##project_manager_panel_1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.33f, winsowSize.y - 20.f));
                ImGui::Image(m_EditorContext->getTextureHolder()->getTexture("editor_project_manager"));
            ImGui::EndChild();

            //Panel 2 : Window tabs
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.33f);
            ImGui::SetCursorPosY(cursor.y);
            ImGui::BeginChild("##project_manager_panel_2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.67f, winsowSize.y * 0.85f));

                if (ImGui::BeginTabBar("##project_manager_tabbar"))
                {
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if (ImGui::BeginTabItem(EditorConstant.TAB_RECENT_PROJECT.c_str(), nullptr, m_TabSelectionHandler->getTabStatus(EditorConstant.TAB_RECENT_PROJECT)))
                    {
                        m_RecentProjectTab.render();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(EditorConstant.TAB_CREATE_PROJECT.c_str(), nullptr, m_TabSelectionHandler->getTabStatus(EditorConstant.TAB_CREATE_PROJECT)))
                    {
                        m_NewProjectTab.render();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(EditorConstant.TAB_OPEN_PROJECT.c_str(), nullptr, m_TabSelectionHandler->getTabStatus(EditorConstant.TAB_OPEN_PROJECT)))
                    {
                        m_OpenProjectTab.render();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(EditorConstant.TAB_WORKSPACE.c_str(), nullptr, m_TabSelectionHandler->getTabStatus(EditorConstant.TAB_WORKSPACE)))
                    {
                       m_WorkspaceTab.render();

                        ImGui::EndTabItem();
                    }

                    m_TabSelectionHandler->reset();

                    ImGui::EndTabBar();
                }

            ImGui::EndChild();

            ImGui::SetCursorPosY(winsowSize.y - 38.f);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 4.0f));
            ImGui::SetCursorPosX(winsowSize.x/2.f - 50.f);
            if (ImGui::Button("Close##close_project_manager_window", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
