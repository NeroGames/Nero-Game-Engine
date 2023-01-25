////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/GameSettingWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    GameSettingWindow::GameSettingWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    GameSettingWindow::~GameSettingWindow()
    {
        destroy();
    }

    void GameSettingWindow::destroy()
    {
    }

    void GameSettingWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_GAME_SETTING.c_str());

        ImGui::End();
    }
} // namespace nero
