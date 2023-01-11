////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/GameLevelWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    GameLevelWindow::GameLevelWindow(EditorContext::Ptr editorContext):
         UIComponent(std::move(editorContext))
    {

    }

    GameLevelWindow::~GameLevelWindow()
    {
        destroy();
    }

    void GameLevelWindow::destroy()
    {

    }

    void GameLevelWindow::render()
    {

    }
}
