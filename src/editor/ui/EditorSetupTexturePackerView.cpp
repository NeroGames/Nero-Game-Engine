////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorSetupTexturePackerView.h>
//Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorSetupTexturePackerView::EditorSetupTexturePackerView(EditorContext::Ptr editorContext,
                                                               EditorSetup::Ptr editorSetup):
         UIComponent(editorContext)
        ,m_EditorSetup(editorSetup)
    {

    }

    EditorSetupTexturePackerView::~EditorSetupTexturePackerView()
    {
        destroy();
    }

    void EditorSetupTexturePackerView::destroy()
    {

    }

    void EditorSetupTexturePackerView::render()
    {

    }
}
