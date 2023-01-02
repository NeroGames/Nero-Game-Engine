////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorSetupCodeEditorView.h>
//Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorSetupCodeEditorView::EditorSetupCodeEditorView(EditorContext::Ptr editorContext,
                                                         EditorSetup::Ptr editorSetup):
         UIComponent(editorContext)
        ,m_EditorSetup(editorSetup)
    {

    }

    EditorSetupCodeEditorView::~EditorSetupCodeEditorView()
    {
        destroy();
    }

    void EditorSetupCodeEditorView::destroy()
    {

    }

    void EditorSetupCodeEditorView::render()
    {

    }
}
