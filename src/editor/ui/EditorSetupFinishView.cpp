////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorSetupFinishView.h>
//Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorSetupFinishView::EditorSetupFinishView(EditorContext::Ptr editorContext,
                                                 EditorSetup::Ptr editorSetup):
         UIComponent(editorContext)
        ,m_EditorSetup(editorSetup)
    {

    }

    EditorSetupFinishView::~EditorSetupFinishView()
    {
        destroy();
    }

    void EditorSetupFinishView::destroy()
    {

    }

    void EditorSetupFinishView::render()
    {

    }
}
