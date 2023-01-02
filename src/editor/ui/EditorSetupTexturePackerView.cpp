////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
#include <Nero/editor/ui/EditorSetupTexturePackerView.h>
#include <Nero/editor/EditorConstant.h>
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
        clearInput();
    }

    EditorSetupTexturePackerView::~EditorSetupTexturePackerView()
    {
        destroy();
    }

    void EditorSetupTexturePackerView::destroy()
    {
        clearInput();
    }

    void EditorSetupTexturePackerView::render()
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::Text("Configure Texture Packer");
        ImGui::PopFont();
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 10.f));

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 150.f)/2.f);
        ImGui::Image(m_EditorContext->getTextureHolder()->getTexture("texture_packer_icon"), ImVec2(150.f, 150.f));

        ImGui::Dummy(ImVec2(0.f, 10.f));

        ImGui::TextWrapped("Install and Configure Texture Packer");

        ImGui::Dummy(ImVec2(0.f, 10.f));

        ImGui::Text("After installing Texture Packer, please provide the path to its excutable.");
        ImGui::Text("Here are some examples of path");
        ImGui::BulletText("C:/Program Files/CodeAndWeb/TexturePacker/bin/TexturePackerGUI.exe");

        ImGui::Dummy(ImVec2(0.f, 10.f));

        float wording_width = 100.f;
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Download here");
        ImGui::SameLine(wording_width);
        if(ImGui::Button("https://www.codeandweb.com/texturepacker"))
        {
            cmd::launchBrowser("https://www.codeandweb.com/texturepacker");
        }

        ImGui::Dummy(ImVec2(0.f, 10.f));

        float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;
        ImGui::Text("Texture Packer");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width - 100.f);
        ImGui::InputText("##texturepacker", m_Input.texturePackerPath, sizeof(m_Input.texturePackerPath), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine(wording_width + input_width - 80.f);
        if(ImGui::Button("Browse##texturepacker", ImVec2(60.f, 0)))
        {
            file::selectFile([this](std::string outPath)
            {
                if(outPath.find("TexturePackerGUI.exe") != std::string::npos)
                {
                    string::fillCharArray(m_Input.texturePackerPath, sizeof(m_Input.texturePackerPath), outPath);

                    putenv(std::string("NERO_GAME_TP=" + outPath).c_str());

                    BTManager::startTask([outPath](BackgroundTask::Ptr backgroundTask)
                    {
                        cmd::setEnvironmnentVariable("NERO_GAME_TP", outPath);
                        backgroundTask->setCompleted(true);
                    });

                    m_EditorContext->getEditorSetting()->getSetting("environment").setString("texture_packer", outPath);
                }
                else
                {
                    ImGui::OpenPopup(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str());
                }

            });
        }

        //Error modal
        ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowSize(ImVec2(250.f, 100.f));
        if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str(), nullptr, window_flags))
        {
            ImGui::TextWrapped("The path provided is not valid");

            ImGui::SetCursorPosY(100.f - 30.f);
            ImGui::SetCursorPosX((250.f - 75.f)/2.f);
            if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        m_EditorSetup->setNextSetup(m_EditorContext->getEditorSetting()->getSetting("environment").getString("texture_packer")
                                    != StringPool.BLANK);
    }

    void EditorSetupTexturePackerView::clearInput()
    {
        string::fillCharArray(m_Input.texturePackerPath,	 sizeof(m_Input.texturePackerPath),		StringPool.BLANK);
    }
}
