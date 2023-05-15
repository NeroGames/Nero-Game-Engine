////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/QuickSettingWindow.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    QuickSettingWindow::QuickSettingWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    QuickSettingWindow::~QuickSettingWindow()
    {
        destroy();
    }

    void QuickSettingWindow::destroy()
    {
    }

    void QuickSettingWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_QUICK_SETTING.c_str());

        if(m_EditorContext->getGameProject())
        {
            ImGui::BeginChild("##compilation", ImVec2(0.f, 90.f), true);
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
                               compilationSetting.toString(),
                               true);
            }

            ImGui::Dummy(ImVec2(0.f, 1.f));

            const float wordingWidth = 35.f;
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
                               compilationSetting.toString(),
                               true);
            }

            ImGui::EndChild();

            ImGui::BeginChild("##editor_texture", ImVec2(0.f, 90.f), true);
            ImGui::Text("Editor Texture");
            ImGui::Separator();

            ImGui::Text("Texture Factor");
            ImGui::SetNextItemWidth(inputWidth - 30.f);
            float textureFactor = m_EditorContext->getRenderContext()->textureFactor;
            ImGui::InputFloat("##editor_texture_factor", &textureFactor, 1.f, 1.0f, "%.3f");
            if(ImGui::IsItemEdited())
            {
                if(textureFactor < 1.f)
                {
                    textureFactor = 1.f;
                }

                m_EditorContext->getRenderContext()->textureFactor = textureFactor;
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
} // namespace nero
