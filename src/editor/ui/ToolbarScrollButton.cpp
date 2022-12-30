////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/ToolbarScrollButton.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ToolbarScrollButton::ToolbarScrollButton(UIComponent::EditorContext editorContext,
                                             ImGuiWindow* toolbarContentWindow,
                                             const Direction& direction):
        UIComponent(editorContext)
        ,m_Direction(direction)
        ,m_ToolbarContentWindow(toolbarContentWindow)
    {

    }

    ToolbarScrollButton::~ToolbarScrollButton()
    {
        destroy();
    }

    void ToolbarScrollButton::destroy()
    {
        m_ToolbarContentWindow = nullptr;
    }

    void ToolbarScrollButton::render()
    {
       switch(m_Direction)
       {
           case Direction::Left:
           {
               renderLeft();
               break;
           }
           case Direction::Right:
           {
               renderRight();
               break;
           }
           case Direction::None:
           {
               // TODO [ToolbarScrollButton] handle None case log + exception
               break;
           }
       }
    }

    void ToolbarScrollButton::renderLeft()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::BeginChild("##toolbarscrollleft", ImVec2(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.590f, 0.980f, 1.000f));
        if(ImGui::Button("<<##toolbarscrollleftbutton", ImVec2(32.f, 38.f)))
        {
            if(m_ToolbarContentWindow)
            {
                ImVec2 max_step;
                max_step.x = m_ToolbarContentWindow->InnerRect.GetSize().x * 0.67f;
                max_step.y = m_ToolbarContentWindow->InnerRect.GetSize().y * 0.67f;

                float scroll_step = ImFloor(ImMin(2 * m_ToolbarContentWindow->CalcFontSize(), max_step.x));
                ImGui::SetWindowScrollX(m_ToolbarContentWindow, m_ToolbarContentWindow->Scroll.x - 1 * scroll_step);
            }
        }
        ImGui::PopStyleColor();

        ImGui::EndChild();
        ImGui::PopStyleVar();
    }

    void ToolbarScrollButton::renderRight()
    {
        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 32.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::BeginChild("##toolbarscrollright", ImVec2(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.590f, 0.980f, 1.000f));
            if(ImGui::Button(">>##toolbarscrollrightbutton", ImVec2(32.f, 38.f)))
            {
                if(m_ToolbarContentWindow)
                {
                    ImVec2 max_step;
                    max_step.x = m_ToolbarContentWindow->InnerRect.GetSize().x * 0.67f;
                    max_step.y = m_ToolbarContentWindow->InnerRect.GetSize().y * 0.67f;

                    float scroll_step = ImFloor(ImMin(2 * m_ToolbarContentWindow->CalcFontSize(), max_step.x));
                    ImGui::SetWindowScrollX(m_ToolbarContentWindow, m_ToolbarContentWindow->Scroll.x + 1 * scroll_step);
                }
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        ImGui::EndChild();
    }
}
