////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
#include <Nero/editor/view/EditorSetupCodeEditorView.h>
#include <Nero/editor/EditorConstant.h>
// Iconfont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorSetupCodeEditorView::EditorSetupCodeEditorView(EditorContext::Ptr editorContext,
                                                         EditorSetup::Ptr   editorSetup)
        : UIComponent(std::move(editorContext))
        , m_EditorSetup(editorSetup)
    {
        clearInput();
    }

    EditorSetupCodeEditorView::~EditorSetupCodeEditorView()
    {
        destroy();
    }

    void EditorSetupCodeEditorView::destroy()
    {
        clearInput();
    }

    void EditorSetupCodeEditorView::render()
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::Text("Configure a Code Editor");
        ImGui::PopFont();
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::TextWrapped(
            "You can use QT Creator or Visual Studio 2019, We recommand using QT Creator");

        ImGui::Dummy(ImVec2(0.f, 20.f));

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 550.f) / 2.f);

        pushToolbarStyle(m_Input.selectQtCreator);
        if(ImGui::Button("QT Creator", ImVec2(250.f, 50.f)))
        {
            m_Input.selectQtCreator = true;
        }
        popToolbarStyle();

        ImGui::SameLine(0.f, 50.f);

        pushToolbarStyle(!m_Input.selectQtCreator);
        if(ImGui::Button("Visual Studio 2019", ImVec2(250.f, 50.f)))
        {
            m_Input.selectQtCreator = false;
        }
        popToolbarStyle();

        ImGui::Dummy(ImVec2(0.f, 50.f));

        if(m_Input.selectQtCreator)
        {
            ImGui::TextWrapped("Install and Configure QT Creator");

            ImGui::Dummy(ImVec2(0.f, 10.f));

            ImGui::Text("After installing QT Creator, please provide the path to its excutable");
            ImGui::Text("Here are some examples of path");
            ImGui::BulletText("C:/Qt/qtcreator-4.11.2/bin/qtcreator.exe");
            ImGui::BulletText("C:/Qt/Tools/QtCreator/bin/qtcreator.exe");

            ImGui::Dummy(ImVec2(0.f, 10.f));

            float wording_width = 100.f;
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Download here");
            ImGui::SameLine(wording_width);
            if(ImGui::Button("https://www.qt.io/offline-installers"))
            {
                cmd::launchBrowser("https://www.qt.io/offline-installers");
            }

            ImGui::Dummy(ImVec2(0.f, 10.f));

            float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;
            ImGui::Text("QtCreator");
            ImGui::SameLine(wording_width);
            ImGui::SetNextItemWidth(input_width - 100.f);
            ImGui::InputText("##qtcreator",
                             m_Input.qtCreatorPath,
                             sizeof(m_Input.qtCreatorPath),
                             ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine(wording_width + input_width - 80.f);
            if(ImGui::Button("Browse##editor_path", ImVec2(60.f, 0)))
            {
                file::selectFile(
                    [this](std::string outPath)
                    {
                        if(outPath.find("qtcreator.exe") != std::string::npos)
                        {
                            string::fillCharArray(m_Input.qtCreatorPath,
                                                  sizeof(m_Input.qtCreatorPath),
                                                  outPath);

                            putenv(std::string("NERO_GAME_QT=" + outPath).c_str());

                            BTManager::startTask(
                                [outPath](BackgroundTask::Ptr backgroundTask)
                                {
                                    cmd::setEnvironmnentVariable("NERO_GAME_QT", outPath);
                                    backgroundTask->setCompleted(true);
                                });

                            m_EditorContext->getEditorSetting()
                                ->getSetting("environment")
                                .setString("qt_creator", outPath);
                        }
                        else
                        {
                            ImGui::OpenPopup(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str());
                        }
                    });
            }
        }
        else
        {
            ImGui::TextWrapped("Install and Configure Visual Studio 2019");

            ImGui::Dummy(ImVec2(0.f, 10.f));

            ImGui::Text(
                "After installing Visual Studio, please provide the path to its excutable.");
            ImGui::Text("Here are some examples of path");
            ImGui::BulletText("C:/Program Files (x86)/Microsoft Visual "
                              "Studio/2019/Preview/Common7/IDE/devenv.exe");

            ImGui::Dummy(ImVec2(0.f, 10.f));

            float wording_width = 100.f;
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Download here");
            ImGui::SameLine(wording_width);
            if(ImGui::Button("https://visualstudio.microsoft.com/downloads/"))
            {
                cmd::launchBrowser("https://visualstudio.microsoft.com/downloads/");
            }

            ImGui::Dummy(ImVec2(0.f, 10.f));

            float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;
            ImGui::Text("Visual Studio");
            ImGui::SameLine(wording_width);
            ImGui::SetNextItemWidth(input_width - 100.f);
            ImGui::InputText("##visualstudio",
                             m_Input.visualStudioPath,
                             sizeof(m_Input.visualStudioPath),
                             ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine(wording_width + input_width - 80.f);
            if(ImGui::Button("Browse##editor_path", ImVec2(60.f, 0)))
            {
                file::selectFile(
                    [this](std::string outPath)
                    {
                        if(outPath.find("devenv.exe") != std::string::npos)
                        {
                            string::fillCharArray(m_Input.visualStudioPath,
                                                  sizeof(m_Input.visualStudioPath),
                                                  outPath);

                            putenv(std::string("NERO_GAME_VS=" + outPath).c_str());

                            BTManager::startTask(
                                [outPath](BackgroundTask::Ptr backgroundTask)
                                {
                                    cmd::setEnvironmnentVariable("NERO_GAME_VS", outPath);
                                    backgroundTask->setCompleted(true);
                                });

                            m_EditorContext->getEditorSetting()
                                ->getSetting("environment")
                                .setString("visual_studio", outPath);
                        }
                        else
                        {
                            ImGui::OpenPopup(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str());
                        }
                    });
            }
        }

        // Error modal
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowSize(ImVec2(250.f, 100.f));
        if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_INVALID_PATH.c_str(),
                                  nullptr,
                                  window_flags))
        {
            ImGui::TextWrapped("The path provided is not valid");

            ImGui::SetCursorPosY(100.f - 30.f);
            ImGui::SetCursorPosX((250.f - 75.f) / 2.f);
            if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        Setting setting = m_EditorContext->getEditorSetting()->getSetting("environment");

        m_EditorSetup->setNextSetup(setting.getString("qt_creator") != StringPool.BLANK ||
                                    setting.getString("visual_studio") != StringPool.BLANK);
    }

    void EditorSetupCodeEditorView::clearInput()
    {
        m_Input.selectQtCreator = true;
        string::fillCharArray(m_Input.qtCreatorPath,
                              sizeof(m_Input.qtCreatorPath),
                              StringPool.BLANK);
        string::fillCharArray(m_Input.visualStudioPath,
                              sizeof(m_Input.visualStudioPath),
                              StringPool.BLANK);
    }
} // namespace nero
