////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/RecentProjectTab.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    RecentProjectTab::RecentProjectTab(EditorContext::Ptr editorContext,
                                       TabSelectionHandler::Ptr tabSelectionHandler):
         UIComponent(editorContext)
        ,m_TabSelectionHandler(tabSelectionHandler)
    {

    }

    RecentProjectTab::~RecentProjectTab()
    {
        destroy();
    }

    void RecentProjectTab::destroy()
    {

    }

    void RecentProjectTab::render()
    {
        ImGui::Text("Open a recent project and conitnue where you left");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 16.0f));


        ImVec2 buttonSize(130, 100);
        float spacing           = (ImGui::GetWindowContentRegionWidth() - 3 * 150)/2.f;
        float xWindowVisible    = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        unsigned int buttonCount        = 6;
        auto recentProjectTable = file::loadJson(file::getPath({"setting", "recent_project"}));

        for (unsigned int i = 0; i < buttonCount; i++)
        {
            if(i < recentProjectTable.size())
            {
                auto project = recentProjectTable[recentProjectTable.size()-i-1];

                ImGui::Image(m_EditorContext->getTextureHolder()->getTexture("recent_project_" + toString(i+1)));
                ImGui::SameLine(0.f, 5.f);
                std::string projectName =  string::wrapString(project["project_name"].get<std::string>(), 17);

                ImGui::PushID(i);
                if(ImGui::Button(projectName.c_str(), buttonSize))
                {
                    ImGui::CloseCurrentPopup();

                    m_EditorContext->getEditorProxy()->openProject(project["project_directory"].get<std::string>());
                }
                ImGui::PopID();
            }

            ImGuiStyle& style = ImGui::GetStyle();
            float xLastButton = ImGui::GetItemRectMax().x;
            // Expected position if next button was on same line
            float xNextButton = xLastButton + style.ItemSpacing.x + buttonSize.x + 20.f;

            if (i + 1 < buttonCount && xNextButton < xWindowVisible)
            {
                ImGui::SameLine(0.f, spacing);
            }
            else {
                ImGui::Dummy(ImVec2(0.0f, spacing));
            }

        }

        ImGui::SetCursorPosY(EditorConstant.WINDOW_PROJECT_MANAGER_SIZE.y * 0.74f);

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 450.f)/2.f);

        pushToolbarStyle();
        if(ImGui::Button("Create New Project", ImVec2(200.f, 40.f)))
        {
            m_TabSelectionHandler->selectTab(EditorConstant.TAB_CREATE_PROJECT);
        }

        ImGui::SameLine(0.f, 50.f);

        if(ImGui::Button("Open Other Projects", ImVec2(200.f, 40.f)))
        {
            m_TabSelectionHandler->selectTab(EditorConstant.TAB_OPEN_PROJECT);
        }
        popToolbarStyle();
    }
}
