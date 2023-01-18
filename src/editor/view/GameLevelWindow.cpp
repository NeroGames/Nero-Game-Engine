////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/GameLevelWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    GameLevelWindow::GameLevelWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    GameLevelWindow::~GameLevelWindow()
    {
        destroy();
    }

    void GameLevelWindow::destroy()
    {
    }

    void GameLevelWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_LEVEL.c_str());

        ImVec2 button_size = ImVec2(100.f, 0.f);
        if(ImGui::Button("Open##open_game_level", button_size))
        {
            m_EditorContext->getEditorProxy()->openGameLevel(m_EditorContext->getSelectedGameLevelName());
        }

        ImGui::SameLine();

        if(ImGui::Button("Edit##edit_game_level", button_size))
        {
        }

        ImGui::SameLine();

        if(ImGui::Button("Copy##copy_game_level", button_size))
        {
        }

        ImGui::SameLine();

        if(ImGui::Button("Close##close_game_level", button_size))
        {
            // TODO transfer to EditorProxy
            auto advancedScene = m_EditorContext->getAdvancedScene();

            if(!advancedScene)
                return;

            m_EditorContext->setOpenedGameLevelName(StringPool.BLANK);
            advancedScene->closeSelectedLevel();
        }

        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 93.f);

        if(ImGui::Button("Delete##delete_game_level", button_size))
        {
        }

        ImGui::Dummy(ImVec2(0.f, 2.f));

        ImGui::BeginChild("##show_game_level", ImVec2(), true);

        auto        levelNameTable    = m_EditorContext->getAdvancedScene()->getRegisteredLevelTable();
        ImGuiStyle& style             = ImGui::GetStyle();
        float       window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth();

        int         level_count       = levelNameTable.size();
        int         count             = 0;
        auto        printSameLine     = [&count, &level_count, &style, &window_visible_x2]() {
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.f;
            if(count++ + 1 < level_count && next_button_x2 < window_visible_x2)
            {
                ImGui::SameLine();
            }
            else
            {
                ImGui::Dummy(ImVec2(0.0f, 4.0f));
            }
        };

        for(const std::string& name : levelNameTable)
        {
            ImVec2 button_size(200.f, 75.f);

            pushGameLevelStyle(m_EditorContext->getSelectedGameLevelName() == name,
                               m_EditorContext->getOpengedGameLevelName() == name);
            if(ImGui::Button(name.c_str(), button_size))
            {
                m_EditorContext->setSelectedGameLevelName(name);
            }
            popGameLevelStyle();

            printSameLine();
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace nero
