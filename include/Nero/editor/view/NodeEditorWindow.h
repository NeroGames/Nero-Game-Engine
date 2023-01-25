////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NODEEDITORWINDOW_H
#define NODEEDITORWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
// Node Editor
#include <nodeeditor/imgui_node_editor.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NodeEditorWindow : public UIComponent
    {
      public:
        NodeEditorWindow(EditorContext::Ptr             editorContext,
                         ax::NodeEditor::EditorContext* nodeEditorContext);
        virtual ~NodeEditorWindow() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        ax::NodeEditor::EditorContext* m_NodeEditorContext;
    };
} // namespace nero
#endif // NODEEDITORWINDOW_H
