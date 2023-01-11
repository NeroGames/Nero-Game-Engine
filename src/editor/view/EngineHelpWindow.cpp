////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/EngineHelpWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EngineHelpWindow::EngineHelpWindow(EditorContext::Ptr editorContext):
        UIComponent(std::move(editorContext))
    {

    }

    EngineHelpWindow::~EngineHelpWindow()
    {
        destroy();
    }

    void EngineHelpWindow::destroy()
    {

    }

    void EngineHelpWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_HELP.c_str());

        ImGui::End();
    }
}
