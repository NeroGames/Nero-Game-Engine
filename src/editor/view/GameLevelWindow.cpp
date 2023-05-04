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
        , m_ButtonZize(100.f, 0.f)
        , m_OpenGameLevel(false)
        , m_CloseGameLevel(false)
        , m_DeleteGameLevel(false)
        , m_ImguiStyle(ImGui::GetStyle())
    {
    }

    GameLevelWindow::~GameLevelWindow()
    {
        destroy();
    }

    void GameLevelWindow::destroy()
    {
    }

    void GameLevelWindow::update(const sf::Time&)
    {
        m_AdvancedScene = m_EditorContext->getAdvancedScene();

        if(m_AdvancedScene)
        {
            if(m_OpenGameLevel)
            {
                m_EditorContext->getEditorProxy()->openGameLevel(
                    m_EditorContext->getSelectedGameLevelName());
                m_OpenGameLevel = false;
            }
            else if(m_CloseGameLevel)
            {
                m_EditorContext->setOpenedGameLevelName(StringPool.BLANK);
                m_AdvancedScene->closeSelectedLevel();
                m_CloseGameLevel = false;
            }
            else if(m_DeleteGameLevel)
            {
                m_EditorContext->getEditorProxy()->removeGameLevel(
                    m_EditorContext->getSelectedGameLevelName());
                m_DeleteGameLevel = false;
            }

            m_LevelNameTable = m_AdvancedScene->getRegisteredLevelTable();
        }

        m_SelectedGameLevel = m_EditorContext->getSelectedGameLevelName();
        m_OpenedGameLevel   = m_EditorContext->getOpengedGameLevelName();
    }

    void GameLevelWindow::render()
    {
        if(!m_AdvancedScene)
            return;

        ImGui::Begin(EditorConstant.WINDOW_LEVEL.c_str());

        if(ImGui::Button("Open##open_game_level", m_ButtonZize))
        {
            m_OpenGameLevel = true;
        }

        ImGui::SameLine();

        if(ImGui::Button("Close##close_game_level", m_ButtonZize))
        {
            m_CloseGameLevel = true;
        }

        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 93.f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
        if(ImGui::Button("Delete##delete_game_level", m_ButtonZize))
        {
            m_DeleteGameLevel = true;
        }
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0.f, 2.f));

        ImGui::BeginChild("##show_game_level", ImVec2(), true);

        float        windowWidth = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth();

        unsigned int index       = 0;
        for(const std::string& name : m_LevelNameTable)
        {
            ImVec2 buttonSize(200.f, 75.f);

            pushGameLevelStyle(m_SelectedGameLevel == name, m_OpenedGameLevel == name);
            if(ImGui::Button(name.c_str(), buttonSize))
            {
                m_EditorContext->setSelectedGameLevelName(name);
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                m_OpenGameLevel = true;
            }
            popGameLevelStyle();

            printSameLine(index, windowWidth, m_ImguiStyle, m_LevelNameTable.size());
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void GameLevelWindow::printSameLine(unsigned int&      index,
                                        const float        windowWidth,
                                        const ImGuiStyle&  imguiStyle,
                                        const unsigned int leveCount)
    {
        float lastButtonWidth = ImGui::GetItemRectMax().x;
        float newButtonWidth  = lastButtonWidth + imguiStyle.ItemSpacing.x + 100.f;

        if(index++ + 1 < leveCount && newButtonWidth < windowWidth)
        {
            ImGui::SameLine();
        }
        else
        {
            ImGui::Dummy(ImVec2(0.0f, 4.0f));
        }
    }
} // namespace nero
