////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorSetupWelcomeView.h>
// Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorSetupWelcomeView::EditorSetupWelcomeView(EditorContext::Ptr editorContext,
                                                   EditorSetup::Ptr   editorSetup)
        : UIComponent(std::move(editorContext))
        , m_EditorSetup(editorSetup)
    {
    }

    EditorSetupWelcomeView::~EditorSetupWelcomeView()
    {
        destroy();
    }

    void EditorSetupWelcomeView::destroy()
    {
    }

    void EditorSetupWelcomeView::render()
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::Text("Welcome To The Configuration Wizard");
        ImGui::PopFont();
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::TextWrapped("If you see this window it's because it's your first time using the "
                           "Engine or because you are missing some configurations");
        ImGui::TextWrapped(
            "The following things need to done in order for the Engine to function properly");

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::Indent();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        if(m_EditorSetup->setupCodeEditor())
        {
            ImGui::Text(ICON_FA_CIRCLE "	Configure a Code Editor");
            ImGui::Dummy(ImVec2(0.f, 5.f));
        }
        if(m_EditorSetup->setupTexturePacker())
        {
            ImGui::Text(ICON_FA_CIRCLE "	Configure Texture Packer");
            ImGui::Dummy(ImVec2(0.f, 5.f));
        }
        if(m_EditorSetup->setupWorkspace())
        {
            ImGui::Text(ICON_FA_CIRCLE "	Create a Workspace");
            ImGui::Dummy(ImVec2(0.f, 5.f));
        }
        ImGui::PopFont();
        ImGui::Unindent();

        ImGui::Dummy(ImVec2(0.f, 15.f));

        ImGui::AlignTextToFramePadding();
        ImGui::Text("You can learn more on the necessary configurations at ");
        ImGui::SameLine();
        if(ImGui::Button("Environment Configurations"))
        {
            cmd::launchBrowser("https://nero-games.com/learn/engine-v2/environment-configuration");
        }

        ImGui::Dummy(ImVec2(0.f, 15.f));

        ImGui::Text("Click on Next to continous");

        m_EditorSetup->setNextSetup(true);
    }
} // namespace nero
