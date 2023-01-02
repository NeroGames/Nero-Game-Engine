////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/ToolbarButtonGroup.h>
#include <Nero/editor/EditorConstant.h>
//Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ToolbarButtonGroup::ToolbarButtonGroup(EditorContext::Ptr editorContext):
        UIComponent(editorContext)
    {

    }

    ToolbarButtonGroup::~ToolbarButtonGroup()
    {
        destroy();
    }

    void ToolbarButtonGroup::destroy()
    {

    }

    void ToolbarButtonGroup::render()
    {
        renderLeftButtonGroup();
        renderMiddleButtonGroup();
        renderRightButtonGroup();
    }

    void ToolbarButtonGroup::renderLeftButtonGroup()
    {
        if(!m_EditorContext->getGameProject())
            return;

        if(m_EditorContext->getEditorMode() == EditorMode::WORLD_BUILDER)
        {
            renderScreenButton();
            ImGui::SameLine();
            renderFactoryButton();
        }
        else if(m_EditorContext->getEditorMode() == EditorMode::SCREEN_BUILDER)
        {
            renderWorldButton();
            ImGui::SameLine();
            renderFactoryButton();
        }
        else if(m_EditorContext->getEditorMode() == EditorMode::OBJECT_BUILDER)
        {
            renderScreenButton();
            ImGui::SameLine();
            renderWorldButton();
        }
    }

    void ToolbarButtonGroup::renderWorldButton()
    {
        if(ImGui::Button(ICON_FA_GLOBE_AFRICA " World", ImVec2(105.f, 28.f)))
        {
            m_EditorContext->setEditorMode(EditorMode::WORLD_BUILDER);
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
        }
    }

    void ToolbarButtonGroup::renderScreenButton()
    {
        if(ImGui::Button(ICON_FA_SQUARE " Screen", ImVec2(105.f, 28.f)))
        {
            m_EditorContext->setEditorMode(EditorMode::SCREEN_BUILDER);
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
        }
    }

    void ToolbarButtonGroup::renderFactoryButton()
    {
        if(ImGui::Button(ICON_FA_WAREHOUSE " Factory", ImVec2(105.f, 28.f)))
        {
            m_EditorContext->setEditorMode(EditorMode::OBJECT_BUILDER);
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
        }
    }

    void ToolbarButtonGroup::renderMiddleButtonGroup()
    {
        if(!m_EditorContext->getGameProject())
            return;

        float offSet = ImGui::GetWindowContentRegionWidth();
        offSet = (offSet - (45 * 7.f + 8.f * 6.f + 32.f))/2.f;

        ImGui::SameLine(offSet);

        //undo
        if(ImGui::Button(ICON_FA_UNDO_ALT, ImVec2(45.f, 28.f)))
        {

        }

        ImGui::SameLine(0.f, 24.f);

        //play
        if(ImGui::Button(ICON_FA_PLAY, ImVec2(45.f, 28.f)))
        {

        }

        ImGui::SameLine();

        //pause
        if(ImGui::Button(ICON_FA_PAUSE, ImVec2(45.f, 28.f)))
        {

        }

        ImGui::SameLine();

        //step
        if(ImGui::Button(ICON_FA_STEP_FORWARD, ImVec2(45.f, 28.f)))
        {

        }

        ImGui::SameLine();

        //reset
        if(ImGui::Button(ICON_FA_CIRCLE, ImVec2(45.f, 28.f)))
        {

        }

        ImGui::SameLine();

        //render
        if(ImGui::Button(ICON_FA_GAMEPAD, ImVec2(45.f, 28.f)))
        {

        }

        ImGui::SameLine(0.f, 24.f);

        //redo
        if(ImGui::Button(ICON_FA_REDO_ALT, ImVec2(45.f, 28.f)))
        {

        }
    }

    void ToolbarButtonGroup::renderRightButtonGroup()
    {
        float rightOffset = 95.f;

        if(m_EditorContext->getGameProject())
        {
            rightOffset += 45.f * 3.f + 8.f * 3.f + 90.f + 3.f + 140.f + 3.f + 10.f;
        }

        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - rightOffset);

        if(m_EditorContext->getGameProject())
        {
            if(m_EditorContext->getGameProject()->getAdvancedScene())
            {
                if(m_EditorContext->getEditorMode() == EditorMode::WORLD_BUILDER)
                {
                    if(ImGui::Button(ICON_FA_PLUS_SQUARE " New Level", ImVec2(140.f, 28.f)))
                    {
                        ImGui::OpenPopup("Create Game Level");
                    }
                }
                else if(m_EditorContext->getEditorMode() == EditorMode::SCREEN_BUILDER)
                {
                    if(ImGui::Button(ICON_FA_PLUS_SQUARE " New Screen", ImVec2(140.f, 28.f)))
                    {
                        ImGui::OpenPopup("Create Game Screen");
                    }
                }
                else if(m_EditorContext->getEditorMode() == EditorMode::OBJECT_BUILDER)
                {
                    if(ImGui::Button(ICON_FA_PLUS_SQUARE " New Object", ImVec2(140.f, 28.f)))
                    {

                    }
                }
            }

            ImGui::SameLine();

            if(ImGui::Button(ICON_FA_CODE, ImVec2(90.f, 28.f)))
            {
                ImGui::OpenPopup(EditorConstant.WINDOW_SCRIPT_WIZARD.c_str());
            }

            ImGui::SameLine();

            if(ImGui::Button(ICON_FA_EDIT, ImVec2(45.f, 28.f)))
            {
                //editProject();
            }

            ImGui::SameLine();

            if(ImGui::Button(ICON_FA_COGS, ImVec2(45.f, 28.f)))
            {
                //compileProject();
            }

            ImGui::SameLine();

            if(ImGui::Button(ICON_FA_SYNC, ImVec2(45.f, 28.f)))
            {
                //reloadProject();
            }

            ImGui::SameLine();
        }

        if(ImGui::Button(ICON_FA_ANCHOR " Project", ImVec2(95.f, 28.f)))
        {
            ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
            // TODO
            //m_ProjectManagerTabBarSwitch.selectTab(EditorConstant.TAB_RECENT_PROJECT);
        }
    }
}
