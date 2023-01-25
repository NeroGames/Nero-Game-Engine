////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/NodeEditorWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    NodeEditorWindow::NodeEditorWindow(EditorContext::Ptr             editorContext,
                                       ax::NodeEditor::EditorContext* nodeEditorContext)
        : UIComponent(std::move(editorContext))
        , m_NodeEditorContext(std::move(nodeEditorContext))
    {
    }

    NodeEditorWindow::~NodeEditorWindow()
    {
        destroy();
    }

    void NodeEditorWindow::destroy()
    {
        m_NodeEditorContext = nullptr;
    }

    void NodeEditorWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_VISUAL_SCRIPT.c_str());

        ax::NodeEditor::SetCurrentEditor(m_NodeEditorContext);

        ax::NodeEditor::Begin("My Editor");

        int uniqueId = 1;

        // Start drawing nodes.
        ax::NodeEditor::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
        ImGui::Text("-> In");
        ax::NodeEditor::EndPin();
        ImGui::SameLine();
        ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
        ImGui::Text("Out ->");
        ax::NodeEditor::EndPin();
        ax::NodeEditor::EndNode();

        ax::NodeEditor::End();

        ImGui::End();
    }
} // namespace nero
