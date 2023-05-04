////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ToolbarButtonGroup.h>
#include <Nero/editor/EditorConstant.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
// Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ToolbarButtonGroup::ToolbarButtonGroup(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_PlayGameLevel(false)
        , m_LevelBuilder(nullptr)
    {
    }

    ToolbarButtonGroup::~ToolbarButtonGroup()
    {
        destroy();
    }

    void ToolbarButtonGroup::destroy()
    {
    }

    void ToolbarButtonGroup::update(const sf::Time& timeStep)
    {
        m_LevelBuilder = m_EditorContext->getLevelBuilder();
        m_EditorMode   = m_EditorContext->getEditorMode();

        if(m_PlayGameLevel)
        {
            if(m_EditorMode != EditorMode::Play_Game)
            {
                m_EditorContext->getEditorProxy()->playGameScene();
            }
            else if(m_EditorMode == EditorMode::Play_Game && m_LevelBuilder)
            {
                auto leveSetting = m_LevelBuilder->getLevelSetting();
                leveSetting->setBool("level_paused", false);
                m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                  "physics_iterations");
            }

            m_PlayGameLevel = false;
        }
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

        if(m_EditorMode == EditorMode::Play_Game || m_EditorMode == EditorMode::Render_Game)
            return;

        pushGameLevelStyle(false, m_EditorMode == EditorMode::World_Builder);
        if(ImGui::Button(ICON_FA_GLOBE_AFRICA " World", ImVec2(105.f, 28.f)))
        {
            m_EditorContext->setEditorMode(EditorMode::World_Builder);
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
        }
        popGameLevelStyle();

        ImGui::SameLine();

        pushGameLevelStyle(false, m_EditorMode == EditorMode::Screen_Builder);
        if(ImGui::Button(ICON_FA_SQUARE " Screen", ImVec2(105.f, 28.f)))
        {
            m_EditorContext->setEditorMode(EditorMode::Screen_Builder);
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
        }
        popGameLevelStyle();

        ImGui::SameLine();

        pushGameLevelStyle(false, m_EditorMode == EditorMode::Factory);
        if(ImGui::Button(ICON_FA_WAREHOUSE " Factory", ImVec2(105.f, 28.f)))
        {
            m_EditorContext->setEditorMode(EditorMode::Factory);
            ImGui::SetWindowFocus(EditorConstant.WINDOW_GAME_SCENE.c_str());
        }
        popGameLevelStyle();
    }

    void ToolbarButtonGroup::renderMiddleButtonGroup()
    {
        if(!m_EditorContext->getGameProject())
            return;

        const bool gamePlaying =
            m_EditorMode == EditorMode::Play_Game || m_EditorMode == EditorMode::Render_Game;

        float offSet = ImGui::GetWindowContentRegionWidth();
        if(gamePlaying)
            offSet = (offSet - (45 * 4.f + 8.f * 3.f + 32.f)) / 2.f;
        else
            offSet = (offSet - (45 * 7.f + 8.f * 6.f + 32.f)) / 2.f;

        ImGui::SameLine(offSet);

        // undo
        if(!gamePlaying)
        {
            if(ImGui::Button(ICON_FA_UNDO_ALT, ImVec2(45.f, 28.f)))
            {
            }

            ImGui::SameLine(0.f, 24.f);
        }

        // play
        if(ImGui::Button(ICON_FA_PLAY, ImVec2(45.f, 28.f)))
        {
            m_PlayGameLevel = true;
        }

        ImGui::SameLine();

        // pause
        if(ImGui::Button(ICON_FA_PAUSE, ImVec2(45.f, 28.f)))
        {
            if(m_EditorMode == EditorMode::Play_Game && m_LevelBuilder)
            {
                auto leveSetting = m_LevelBuilder->getLevelSetting();
                leveSetting->setBool("level_paused", true);
                m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                  "physics_iterations");
            }
        }

        ImGui::SameLine();

        // step
        if(ImGui::Button(ICON_FA_STEP_FORWARD, ImVec2(45.f, 28.f)))
        {
            if(m_EditorMode == EditorMode::Play_Game && m_LevelBuilder)
            {
                auto physicsSetting = m_LevelBuilder->getLevelSetting()->getSetting("physics");

                physicsSetting.setBool("single_step", !physicsSetting.getBool("single_step"));
                m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                  "physics_iterations");
            }
        }

        ImGui::SameLine();

        // stop
        if(ImGui::Button(ICON_FA_SQUARE_FULL, ImVec2(45.f, 28.f)))
        {
            m_EditorContext->getEditorProxy()->stopPlayGameScene();
        }

        ImGui::SameLine();

        // render
        if(!gamePlaying)
        {
            if(ImGui::Button(ICON_FA_GAMEPAD, ImVec2(45.f, 28.f)))
            {
            }
        }

        // redo
        if(!gamePlaying)
        {
            ImGui::SameLine(0.f, 24.f);

            if(ImGui::Button(ICON_FA_REDO_ALT, ImVec2(45.f, 28.f)))
            {
            }
        }
    }

    void ToolbarButtonGroup::renderRightButtonGroup()
    {
        if(m_EditorContext->getEditorMode() == EditorMode::Play_Game)
            return;

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
                if(m_EditorContext->getEditorMode() == EditorMode::World_Builder)
                {
                    if(ImGui::Button(ICON_FA_PLUS_SQUARE " New Level", ImVec2(140.f, 28.f)))
                    {
                        ImGui::OpenPopup("Create Game Level");
                    }
                }
                else if(m_EditorContext->getEditorMode() == EditorMode::Screen_Builder)
                {
                    if(ImGui::Button(ICON_FA_PLUS_SQUARE " New Screen", ImVec2(140.f, 28.f)))
                    {
                        ImGui::OpenPopup("Create Game Screen");
                    }
                }
                else if(m_EditorContext->getEditorMode() == EditorMode::Factory)
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
                m_EditorContext->getEditorProxy()->openCodeEditor();
            }

            ImGui::SameLine();

            if(BTManager::BuildFailed)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.f, 0.f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.f, 0.f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
            }
            if(ImGui::Button(ICON_FA_COGS, ImVec2(45.f, 28.f)))
            {
                m_EditorContext->getEditorProxy()->compileProject();
            }
            if(BTManager::BuildFailed)
            {
                ImGui::PopStyleColor(4);
            }

            ImGui::SameLine();

            if(ImGui::Button(ICON_FA_SYNC, ImVec2(45.f, 28.f)))
            {
                m_EditorContext->getEditorProxy()->reloadProjectLibrary();
            }

            ImGui::SameLine();
        }

        if(ImGui::Button(ICON_FA_ANCHOR " Project", ImVec2(95.f, 28.f)))
        {
            ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
        }
    }
} // namespace nero
