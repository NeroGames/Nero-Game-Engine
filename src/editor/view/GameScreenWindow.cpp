////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/GameScreenWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    GameScreenWindow::GameScreenWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    GameScreenWindow::~GameScreenWindow()
    {
        destroy();
    }

    void GameScreenWindow::destroy()
    {
    }

    void GameScreenWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_SCREEN.c_str());

        ImGui::Text("Manage Game Screen");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.f, 4.f));

        ImVec2 button_size = ImVec2((ImGui::GetWindowContentRegionWidth() - 8.f), 0.f);

        if(ImGui::Button("New Game Screen##add_game_screen", button_size))
        {
            ImGui::OpenPopup("Create Game Screen");
        }

        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImGui::BeginChild("##manage_game_screen", ImVec2(), true);

        /*if(m_AdvancedScene)
        {
                m_InputSelectedGameScreenId = m_AdvancedScene->getSelectedGameScreen()->screenId;

                 for(const auto& gameScreen : m_AdvancedScene->getGameScreenTable())
                 {
                         std::string itemId = "##select_screen" + toString(gameScreen->screenId);
                         ImGui::RadioButton(itemId.c_str(), &m_InputSelectedGameScreenId,
         gameScreen->screenId);

                          if(ImGui::IsItemClicked())
                          {
                                  m_AdvancedScene->setSelectedGameScreen(gameScreen);
                          }

                           ImGui::SameLine();

                            itemId = "##screen_visible" + toString(gameScreen->screenId);
                            ImGui::Checkbox(itemId.c_str(), &gameScreen->visible);

                             ImGui::SameLine();

                              char screen_name[100];
                              string::fillCharArray(screen_name, sizeof(screen_name),
        gameScreen->name); ImGui::SetNextItemWidth(118.f); itemId = "##screen_name" +
        toString(gameScreen->screenId); ImGui::InputText(itemId.c_str(), screen_name,
        sizeof(screen_name));

                             if(ImGui::IsItemEdited())
                             {
                                     gameScreen->name = std::string(screen_name);
                             }
                     }
             }*/

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace nero
