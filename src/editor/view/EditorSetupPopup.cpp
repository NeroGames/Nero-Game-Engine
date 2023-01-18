////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorSetupPopup.h>
#include <Nero/editor/view/EditorSetupWelcomeView.h>
#include <Nero/editor/view/EditorSetupCodeEditorView.h>
#include <Nero/editor/view/EditorSetupFinishView.h>
#include <Nero/editor/view/EditorSetupTexturePackerView.h>
#include <Nero/editor/view/EditorSetupWorkspaceView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorSetupPopup::EditorSetupPopup(EditorContext::Ptr editorContext, EditorSetup::Ptr editorSetup)
        : UIComponent(std::move(editorContext))
        , m_EditorSetup(editorSetup)
        , m_CurrentViewIndex(0)
    {

        // Register welcome view
        m_ViewTable.push_back(std::make_shared<EditorSetupWelcomeView>(m_EditorContext, m_EditorSetup));

        // Register code editor view
        if(m_EditorSetup->setupCodeEditor())
        {
            m_ViewTable.push_back(std::make_shared<EditorSetupCodeEditorView>(m_EditorContext, m_EditorSetup));
        }

        // Register texture packer view
        if(m_EditorSetup->setupTexturePacker())
        {
            m_ViewTable.push_back(std::make_shared<EditorSetupTexturePackerView>(m_EditorContext, m_EditorSetup));
        }

        // Register workspace view
        if(m_EditorSetup->setupWorkspace())
        {
            m_ViewTable.push_back(std::make_shared<EditorSetupWorkspaceView>(m_EditorContext, m_EditorSetup));
        }

        // Register finish view
        m_ViewTable.push_back(std::make_shared<EditorSetupFinishView>(m_EditorContext, m_EditorSetup));

        // Update current view
        m_CurrentView = m_ViewTable.at(m_CurrentViewIndex);
    }

    EditorSetupPopup::~EditorSetupPopup()
    {
        destroy();
    }

    void EditorSetupPopup::destroy()
    {
    }

    void EditorSetupPopup::render()
    {
        ImGui::OpenPopup(EditorConstant.WINDOW_STARTER_WIZARD.c_str());

        // Window flags
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        // Winsow size
        ImVec2 winsow_size = EditorConstant.WINDOW_STARTER_WIZARD_SIZE;

        ImGui::SetNextWindowSize(winsow_size);
        if(ImGui::BeginPopupModal(EditorConstant.WINDOW_STARTER_WIZARD.c_str(), nullptr, window_flags))
        {
            ImGui::BeginChild("##banner", ImVec2(winsow_size.x * 0.2f, 0.f));
            ImGui::Image(m_EditorContext->getTextureHolder()->getTexture("editor_configuration"));
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("##editor_setup");

            m_CurrentView->render();

            ImGui::SetCursorPosY(EditorConstant.WINDOW_STARTER_WIZARD_SIZE.y - 75.f);

            if(m_CurrentViewIndex >= 1)
            {
                pushToolbarStyle();
                if(ImGui::Button("Back", ImVec2(100.f, 0.f)))
                {
                    m_CurrentView = m_ViewTable.at(--m_CurrentViewIndex);
                }
                popToolbarStyle();
            }

            if(m_EditorSetup->nextSetup())
            {
                if(m_CurrentViewIndex >= 1)
                {
                    ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 115.f);
                }
                else
                {
                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 115.f);
                }

                const bool finishSetup = m_CurrentViewIndex == m_ViewTable.size() - 1;
                pushToolbarStyle();
                if(ImGui::Button(finishSetup ? "Finish" : "Next", ImVec2(100.f, 0.f)))
                {
                    if(finishSetup)
                    {
                        m_EditorSetup->setSetupCodeEditor(false);
                        m_EditorSetup->setSetupTexturePacker(false);
                        m_EditorSetup->setSetupWorkspace(false);
                        ImGui::CloseCurrentPopup();
                    }
                    else
                    {
                        m_CurrentView = m_ViewTable.at(++m_CurrentViewIndex);
                    }
                }
                popToolbarStyle();
            }

            ImGui::EndChild();

            ImGui::EndPopup();
        }
    }
} // namespace nero
