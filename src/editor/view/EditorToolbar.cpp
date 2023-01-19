////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorToolbar.h>
#include <Nero/editor/view/ToolbarScrollButton.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorToolbar::EditorToolbar(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_ToolbarContentWindow(nullptr)
        , m_ScrollButtonLeft(m_EditorContext,
                             m_ToolbarContentWindow,
                             ToolbarScrollButton::Direction::Left)
        , m_ScrollButtonRight(m_EditorContext,
                              m_ToolbarContentWindow,
                              ToolbarScrollButton::Direction::Right)
        , m_ToolbarButtonGroup(m_EditorContext)
        , m_ProjectManagerPopup(m_EditorContext)
        , m_NewGameLevelPopup(m_EditorContext)
    {
    }

    EditorToolbar::~EditorToolbar()
    {
        destroy();
    }

    void EditorToolbar::destroy()
    {
    }

    void EditorToolbar::render()
    {
        // Begin toolbar window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::Begin(EditorConstant.WINDOW_TOOLBAR.c_str(), nullptr, ImGuiWindowFlags_NoDecoration);

        // Check scrolling
        const float toolbarContentWindowWidth = ImGui::GetWindowContentRegionWidth();
        const bool  showScrollButton =
            toolbarContentWindowWidth < EditorConstant.WINDOW_TOOLBAR_MIN_WIDTH;

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
            ImGui::BeginChild("##toolbarwindowcontent",
                              ImVec2(toolbarContentWindowWidth - 67.f, 0.f),
                              false,
                              ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollbar |
                                  ImGuiWindowFlags_ScrollToolbar | ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_AlwaysUseWindowPadding);
            m_ToolbarContentWindow = ImGui::GetCurrentWindow();
        }
        else
        {
            ImGui::BeginChild("##toolbarwindowcontent",
                              ImVec2(),
                              false,
                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                                  ImGuiWindowFlags_AlwaysUseWindowPadding);
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

    void EditorToolbar::renderToolbarContent()
    {
        // Button
        pushToolbarStyle();
        m_ToolbarButtonGroup.render();
        popToolbarStyle();

        // Popup
        renderToolbarPopupGroup();
    }

    void EditorToolbar::renderToolbarPopupGroup()
    {
        m_ProjectManagerPopup.render();
        m_NewGameLevelPopup.render();
        // TODO
        // m_NewGameScreenPopup.render();
        // m_NewGameScriptPopup.render();
    }
} // namespace nero
