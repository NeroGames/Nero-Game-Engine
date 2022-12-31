////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/Toolbar.h>
#include <Nero/editor/ui/ToolbarScrollButton.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    Toolbar::Toolbar(EditorContext::Ptr editorContext):
        UIComponent(editorContext)
        ,m_ToolbarContentWindow(nullptr)
        ,m_ScrollButtonLeft(editorContext,
                            m_ToolbarContentWindow,
                            ToolbarScrollButton::Direction::Left)
        ,m_ScrollButtonRight(editorContext,
                             m_ToolbarContentWindow,
                             ToolbarScrollButton::Direction::Right)
        ,m_ToolbarButtonGroup(editorContext)
    {

    }

    Toolbar::~Toolbar()
    {

    }

    void Toolbar::destroy()
    {

    }

    void Toolbar::render()
    {
        // Begin toolbar window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::Begin(EditorConstant.WINDOW_TOOLBAR.c_str(), nullptr, ImGuiWindowFlags_NoDecoration);

        // Check scrolling
        const float toolbarContentWindowWidth	= ImGui::GetWindowContentRegionWidth();
        const bool  showScrollButton			= toolbarContentWindowWidth < EditorConstant.WINDOW_TOOLBAR_MIN_WIDTH;

        // Scrool left button
        if(showScrollButton)
        {
            m_ScrollButtonLeft.render();
            ImGui::SameLine(32.f);
        }

        // Begin toolbar content window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.f, 6.f));
        if(showScrollButton)
        {
            ImGui::SetNextWindowContentWidth(EditorConstant.WINDOW_TOOLBAR_MIN_WIDTH);
            ImGui::BeginChild("##toolbarwindowcontent", ImVec2(toolbarContentWindowWidth -67.f, 0.f), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_ScrollToolbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysUseWindowPadding);
            m_ToolbarContentWindow = ImGui::GetCurrentWindow();
        }
        else
        {
            ImGui::BeginChild("##toolbarwindowcontent", ImVec2(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding);
        }

        // Render toolbar
        renderToolbarContent();

        // End toolbar content window
        ImGui::EndChild();
        ImGui::PopStyleVar();

        // Scrool right button
        if(showScrollButton)
        {
            m_ScrollButtonRight.render();
        }

        // End toolbar window
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void Toolbar::renderToolbarContent()
    {
        // Button
        pushToolbarStyle();
        m_ToolbarButtonGroup.render();
        popToolbarStyle();

        // Popup
        renderToolbarPopupGroup();
    }

    void Toolbar::renderToolbarPopupGroup()
    {
        //m_ProjectManagerPopup.render();
        //m_NewGameLevelPopup.render();
        //m_NewGameScreenPopup.render();
        //m_NewGameScriptPopup.render();
    }
}
