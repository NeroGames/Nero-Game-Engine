////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorSetupWorkspaceView.h>
//Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorSetupWorkspaceView::EditorSetupWorkspaceView(EditorContext::Ptr editorContext,
                                                       EditorSetup::Ptr editorSetup):
         UIComponent(editorContext)
        ,m_EditorSetup(editorSetup)
    {

    }

    EditorSetupWorkspaceView::~EditorSetupWorkspaceView()
    {
        destroy();
    }

    void EditorSetupWorkspaceView::destroy()
    {

    }

    void EditorSetupWorkspaceView::render()
    {

    }
}
