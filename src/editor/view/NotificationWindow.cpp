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

        // FIXME-VIEWPORT: Select a default viewport
        const float DISTANCE = 10.0f;
        static int  corner   = 3;
        if(corner != -1)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2         window_pos =
                ImVec2((corner & 1) ? (viewport->Pos.x + viewport->Size.x - DISTANCE)
                                    : (viewport->Pos.x + DISTANCE),
                       (corner & 2) ? (viewport->Pos.y + viewport->Size.y - DISTANCE)
                                    : (viewport->Pos.y + DISTANCE));
            ImVec2 window_pos_pivot =
                ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.000f, 1.000f, 1.000f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.911f, 0.499f, 0.146f, 1.000f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.00f));
        if(ImGui::Begin("##corner_window",
                        nullptr,
                        (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDocking |
                            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            for(const std::string& message : messageTable)
            {
                ImGui::Text("%s", message.c_str());
            }

            for(BackgroundTask::Ptr task : taskTable)
            {
                if(!task->completed())
                {
                    ImGui::Text("%s", task->getMessage().c_str());
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
    }
} // namespace nero
