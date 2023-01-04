////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/ResourceBrowserWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ResourceBrowserWindow::ResourceBrowserWindow(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
    {

    }

    ResourceBrowserWindow::~ResourceBrowserWindow()
    {
        destroy();
    }

    void ResourceBrowserWindow::destroy()
    {

    }

    void ResourceBrowserWindow::render()
    {

    }
}
