////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorUtilityWindow.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
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

        ImGui::BeginChild("##builder_mode", ImVec2(0.f, 105.f), true);
        ImGui::Text("Choose Builder Mode");
        ImGui::Separator();

        const auto editorMode       = m_EditorContext->getEditorMode();
        const auto builderMode      = m_EditorContext->getBuilderMode();
        int        builderModeIndex = 0;

        switch(builderMode)
        {
            case BuilderMode::Object:
                builderModeIndex = 0;
                break;
            case BuilderMode::Mesh:
                builderModeIndex = 1;
                break;
            case BuilderMode::Joint:
                builderModeIndex = 2;
                break;
            case BuilderMode::None:
                builderModeIndex = 3;
                break;
        }

        ImGui::RadioButton("Object", &builderModeIndex, 0);
        if(ImGui::IsItemEdited() && builderMode != BuilderMode::None)
        {
            m_EditorContext->setBuilderMode(BuilderMode::Object);
        }

        if(editorMode != EditorMode::Screen_Builder)
        {
            ImGui::RadioButton("Mesh", &builderModeIndex, 1);
            if(ImGui::IsItemEdited() && builderMode != BuilderMode::None)
            {
                m_EditorContext->setBuilderMode(BuilderMode::Mesh);
            }

            ImGui::RadioButton("Joint", &builderModeIndex, 2);
            if(ImGui::IsItemEdited() && builderMode != BuilderMode::None)
            {
                m_EditorContext->setBuilderMode(BuilderMode::Joint);
            }
        }

        ImGui::EndChild();

        ImGui::BeginChild("##save_and_load", ImVec2(0.f, 85.f), true);
        ImGui::Text("Save & Load");
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 2.f));

        bool autoSavedEnabled = m_EditorContext->autoSaveEnabled();
        ImGui::Checkbox("Auto save", &autoSavedEnabled);
        if(ImGui::IsItemEdited())
        {
            m_EditorContext->setAutoSaveEnabled(autoSavedEnabled);
        }

        ImVec2 buttonSize = ImVec2((ImGui::GetWindowContentRegionWidth() - 8.f) / 2.f, 0.f);

        if(ImGui::Button("Save", buttonSize))
        {
            m_EditorContext->getEditorProxy()->saveProject();
            m_EditorContext->getNotificationManager()->notify("Project Saved");
        }

        ImGui::SameLine();

        if(ImGui::Button("Load", buttonSize))
        {
            m_EditorContext->getEditorProxy()->loadProject();
        }

        ImGui::EndChild();

        if(m_EditorContext->getGameProject())
        {

            ImGui::BeginChild("##compilation", ImVec2(0.f, 85.f), true);
            ImGui::Text("Compilation Setting");
            ImGui::Separator();

            ImGui::Dummy(ImVec2(0.f, 2.f));

            auto compilationSetting =
                m_EditorContext->getGameProject()->getProjectSetting()->getSetting("compilation");
            bool cleanBuild = compilationSetting.getBool("clean");
            ImGui::Checkbox("Clean Build", &cleanBuild);
            if(ImGui::IsItemEdited())
            {
                compilationSetting.setBool("clean", cleanBuild);
                const auto projectDirectory =
                    m_EditorContext->getGameProject()->getProjectDirectory();
                file::saveFile(file::getPath({projectDirectory, "Setting", "compilation"},
                                             StringPool.EXT_JSON),
                               compilationSetting.toString());
            }

            const float wordingWidth = 50.f;
            const float inputWidth   = ImGui::GetWindowContentRegionWidth() - wordingWidth;
            ImGui::Text("Timeout");
            ImGui::SameLine(wordingWidth + 30.f);
            ImGui::SetNextItemWidth(inputWidth - 30.f);
            int buildTimeout = compilationSetting.getInt("timeout");
            ImGui::InputInt("##build_timeout", &buildTimeout);
            if(ImGui::IsItemEdited())
            {
                compilationSetting.setInt("timeout", buildTimeout);
                const auto projectDirectory =
                    m_EditorContext->getGameProject()->getProjectDirectory();
                file::saveFile(file::getPath({projectDirectory, "Setting", "compilation"},
                                             StringPool.EXT_JSON),
                               compilationSetting.toString());
            }

            ImGui::EndChild();
        }

        ImGui::BeginChild("access_button", ImVec2(0.f, 90.f), true);
        ImGui::Text("Access Website");
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 2.f));

        if(ImGui::Button("Learn", buttonSize))
        {
            cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/learn/engine-v2");
        }

        ImGui::SameLine();

        if(ImGui::Button("Forum", buttonSize))
        {
            cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/forum");
        }

        if(ImGui::Button("Snippet", buttonSize))
        {
            cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/snippet/engine-v2");
        }

        ImGui::SameLine();

        if(ImGui::Button("API", buttonSize))
        {
            cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/learn/engine-v2/api");
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace nero
