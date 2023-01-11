////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/ObjectLayerWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ObjectLayerWindow::ObjectLayerWindow(EditorContext::Ptr editorContext):
         UIComponent(std::move(editorContext))
    {

    }

    ObjectLayerWindow::~ObjectLayerWindow()
    {
        destroy();
    }

    void ObjectLayerWindow::destroy()
    {

    }

    void ObjectLayerWindow::render()
    {

    }
}
