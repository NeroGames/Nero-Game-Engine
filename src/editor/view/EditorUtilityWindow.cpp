////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorUtilityWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorUtilityWindow::EditorUtilityWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    EditorUtilityWindow::~EditorUtilityWindow()
    {
        destroy();
    }

    void EditorUtilityWindow::destroy()
    {
    }

    void EditorUtilityWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_UTILITY.c_str());

        ImVec2 size = ImGui::GetWindowSize();

        ImGui::BeginChild("scene_mode", ImVec2(0.f, 105.f), true);
        ImGui::Text("Choose Scene Mode");
        ImGui::Separator();

        const auto builderMode = m_EditorContext->getBuilderMode();

        int        e           = 0;
        if(builderMode == BuilderMode::OBJECT)
            e = 0;
        else if(builderMode == BuilderMode::MESH)
            e = 1;
        ImGui::RadioButton("Object", &e, 0);
        if(ImGui::IsItemEdited())
        {
            m_EditorContext->setBuilderMode(BuilderMode::OBJECT);
        }
        ImGui::RadioButton("Mesh", &e, 1);
        if(ImGui::IsItemEdited())
        {
            m_EditorContext->setBuilderMode(BuilderMode::MESH);
        }
        ImGui::RadioButton("Joint", &e, 2);

        ImGui::EndChild();

        ImGui::BeginChild("save_load", ImVec2(0.f, 85.f), true);
        ImGui::Text("Save & Load");
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 2.f));

        static bool auto_save = false;
        ImGui::Checkbox("Auto save", &auto_save);

        ImVec2 button_size = ImVec2((ImGui::GetWindowContentRegionWidth() - 8.f) / 2.f, 0.f);

        if(ImGui::Button("Save", button_size))
        {
            m_EditorContext->getEditorProxy()->saveProject();
        }

        ImGui::SameLine();

        if(ImGui::Button("Load", button_size))
        {
            m_EditorContext->getEditorProxy()->loadProject();
        }

        ImGui::EndChild();

        ImGui::BeginChild("access_button", ImVec2(0.f, 90.f), true);
        ImGui::Text("Access Website");
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 2.f));

        if(ImGui::Button("Learn", button_size))
        {
            cmd::launchBrowser("https://nero-games.com/learn/engine-v2");
        }

        ImGui::SameLine();

        if(ImGui::Button("Forum", button_size))
        {
            cmd::launchBrowser("https://nero-games.com/forum");
        }

        if(ImGui::Button("Snippet", button_size))
        {
            cmd::launchBrowser("https://nero-games.com/snippet/engine-v2");
        }

        ImGui::SameLine();

        if(ImGui::Button("API", button_size))
        {
            cmd::launchBrowser("https://nero-games.com/learn/engine-v2/api");
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace nero
