////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/GameProjectWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    GameProjectWindow::GameProjectWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    GameProjectWindow::~GameProjectWindow()
    {
        destroy();
    }

    void GameProjectWindow::destroy()
    {
    }

    void GameProjectWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_GAME_PROJECT.c_str());

        ImGui::End();
    }
} // namespace nero
