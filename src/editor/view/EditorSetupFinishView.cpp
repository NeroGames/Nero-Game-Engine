////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorSetupFinishView.h>
// Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorSetupFinishView::EditorSetupFinishView(EditorContext::Ptr editorContext,
                                                 EditorSetup::Ptr   editorSetup)
        : UIComponent(std::move(editorContext))
        , m_EditorSetup(editorSetup)
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
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::Text("Configuration Completed");
        ImGui::PopFont();
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::TextWrapped("Congratulation ! You now have a proper environment");

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::Text("Click Finish to close the Wizard");

        m_EditorSetup->setNextSetup(true);
    }
} // namespace nero
