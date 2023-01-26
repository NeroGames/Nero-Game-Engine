////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/NotificationWindow.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
#include <Nero/editor/NotificationManager.h>
////////////////////////////////////////////////////////////
namespace nero
{
    NotificationWindow::NotificationWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_CornerPosition(3)
        , m_BorderOffset(10.f)
    {
    }

    NotificationWindow::~NotificationWindow()
    {
        destroy();
    }

    void NotificationWindow::destroy()
    {
    }

    void NotificationWindow::render()
    {
        auto& messageTable = m_EditorContext->getNotificationManager()->getMessageTable();
        auto& taskTable    = BTManager::getTaskTable();

        if(messageTable.empty() && taskTable.empty())
            return;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2         windowPosition =
            ImVec2((m_CornerPosition & 1) ? (viewport->Pos.x + viewport->Size.x - m_BorderOffset)
                                          : (viewport->Pos.x + m_BorderOffset),
                   (m_CornerPosition & 2) ? (viewport->Pos.y + viewport->Size.y - m_BorderOffset)
                                          : (viewport->Pos.y + m_BorderOffset));
        ImVec2 windowPosistionPivot =
            ImVec2((m_CornerPosition & 1) ? 1.0f : 0.0f, (m_CornerPosition & 2) ? 1.0f : 0.0f);

        ImGuiWindowFlags windowFlags =
            (m_CornerPosition != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if(!taskTable.empty())
        {
            ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always, windowPosistionPivot);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.000f, 1.000f, 1.000f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.911f, 0.499f, 0.146f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.00f));
            if(ImGui::Begin("##background_task", nullptr, windowFlags))
            {

                for(BackgroundTask::Ptr task : taskTable)
                {
                    if(!task->completed() && !task->failed())
                    {
                        ImGui::Text("%s", task->getMessage().c_str());
                    }
                }

                // Update next position
                windowPosition.y -= ImGui::GetWindowHeight() + 5.f;
            }
            ImGui::End();
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
        }

        if(!messageTable.empty())
        {
            ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always, windowPosistionPivot);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.000f, 1.000f, 1.000f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.499f, 0.146f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.00f));
            if(ImGui::Begin("##notification", nullptr, windowFlags))
            {
                for(const std::string& message : messageTable)
                {
                    ImGui::Text("%s", message.c_str());
                }
            }
            ImGui::End();
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
        }
    }
} // namespace nero
