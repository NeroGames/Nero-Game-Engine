////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/NewGameScriptPopup.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    NewGameScriptPopup::NewGameScriptPopup(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))

    {
        clearInput();
    }

    NewGameScriptPopup::~NewGameScriptPopup()
    {
        destroy();
    }

    void NewGameScriptPopup::destroy()
    {
        clearInput();
    }

    void NewGameScriptPopup::render()
    {
        // Window flags
        /*ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoScrollbar;
        // Winsow size
        ImVec2 winsow_size = EditorConstant.WINDOW_SCRIPT_WIZARD_SIZE;

        // Project manager window
        ImGui::SetNextWindowSize(winsow_size);
        // Begin window
        if(ImGui::BeginPopupModal(EditorConstant.WINDOW_SCRIPT_WIZARD.c_str(),
                                  nullptr,
                                  window_flags))
        {

            if(ImGui::BeginTabBar("##scrip_wizard_tabbar"))
            {
                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                float wording_width = 130.f;
                float input_width   = ImGui::GetWindowContentRegionWidth() - 130.f;

                if(ImGui::BeginTabItem("CPP Script"))
                {
                    ImGui::Text("Class Name");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width);
                    ImGui::InputText("##class_name", m_InputClassName, sizeof(m_InputClassName));
                    ImGui::Dummy(ImVec2(0.0f, 2.0f));

                    ImGui::Text("Parent Class");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width - 110.f);
                    ImGui::InputText("##parent_class",
                                     m_InputParentClass,
                                     sizeof(m_InputParentClass),
                                     ImGuiInputTextFlags_ReadOnly);
                    ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 100.f);
                    if(ImGui::Button("Select##select_script_parent_class", ImVec2(100.f, 0)))
                    {
                        ImGui::OpenPopup("Select Script Class");
                    }

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    // ImGui::Separator();

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if(std::string(m_InputParentClass) == "Game Level Script")
                    {
                        ImGui::Text("Game Level");
                        ImGui::SameLine(wording_width);
                        ImGui::SetNextItemWidth(input_width);

                        // load workpsace
                        std::vector<std::string> gameLevelNameTable =
                            m_AdvancedScene->getGameLevelNameTable();

                        gameLevelNameTable.insert(gameLevelNameTable.begin(), StringPool.BLANK);

                        if(gameLevelNameTable.empty())
                        {
                            gameLevelNameTable.push_back("There is no Level availabled");
                        }

                        std::size_t  levelCount      = gameLevelNameTable.size();
                        const char** levelComboTable = new const char*[levelCount];

                        string::fillCharTable(levelComboTable, gameLevelNameTable);

                        m_SelectedGameLevel = levelComboTable[m_SelectedGameLevelIndex];
                        if(ImGui::BeginCombo("##game_level_combo",
                                             m_SelectedGameLevel,
                                             ImGuiComboFlags())) // The second parameter is the
                                                                 // label previewed before
                            opening the combo.
                            {
                                for(int n = 0; n < levelCount; n++)
                                {
                                    bool is_selected = (m_SelectedGameLevel == levelComboTable[n]);

                                    if(ImGui::Selectable(levelComboTable[n], is_selected))
                                    {
                                        m_SelectedGameLevel      = levelComboTable[n];
                                        m_SelectedGameLevelIndex = n;
                                    }

                                    if(is_selected)
                                    {
                                        ImGui::SetItemDefaultFocus();
                                    }
                                }
                                ImGui::EndCombo();
                            }

                        // delete array
                        for(std::size_t i = 0; i < levelCount; i++)
                        {
                            delete[] levelComboTable[i];
                        }
                        delete[] levelComboTable;
                        levelComboTable = nullptr;
                    }
                    else if(std::string(m_InputParentClass) == "Game Screen Script")
                    {
                        ImGui::Text("Screen Level");
                        ImGui::SameLine(wording_width);
                        ImGui::SetNextItemWidth(input_width);

                        // load workpsace
                        std::vector<std::string> gameScreenNameTable =
                            m_AdvancedScene->getGameScreenNameTable();

                        if(gameScreenNameTable.empty())
                        {
                                                 gameScreenNameTable.push_back("There is no Level
                    availabled");
                        }

                        std::size_t  screenCount      = gameScreenNameTable.size();
                        const char** screenComboTable = new const char*[screenCount];

                        string::fillCharTable(screenComboTable, gameScreenNameTable);

                        m_SelectedGameScreen = screenComboTable[m_SelectedGameScreenIndex];
                        if(ImGui::BeginCombo(
                               "##game_level_combo",
                               m_SelectedGameScreen,
                               ImGuiComboFlags())) // The second parameter is the label
                                                 previewed before opening the combo.
                                                 {
                                                     for(int n = 0; n < screenCount; n++)
                                                     {
                                                         bool is_selected = (m_SelectedGameScreen ==
                                                                             screenComboTable[n]);

                                                         if(ImGui::Selectable(screenComboTable[n],
                                                                              is_selected))
                                                         {
                                                             m_SelectedGameScreen =
                                                                 screenComboTable[n];
                                                             m_SelectedGameScreenIndex = n;
                                                         }

                                                         if(is_selected)
                                                         {
                                                             ImGui::SetItemDefaultFocus();
                                                         }
                                                     }
                                                     ImGui::EndCombo();
                                                 }

                        // delete array
                        for(std::size_t i = 0; i < screenCount; i++)
                        {
                                                 delete[] screenComboTable[i];
                        }
                        delete[] screenComboTable;
                        screenComboTable = nullptr;
                    }

                    ImGui::SetNextWindowSize(ImVec2(500.f, 400.f));
                    if(ImGui::BeginPopupModal("Select Script Class", nullptr, window_flags))
                    {
                        std::vector<std::string> script_type = {"Game Level Script",
                                                                "Game Screen Script",
                                                                "Startup Screen Script",
                                                                "Physic Script Object",
                                                                "Simple Script Object",
                                                                "Action Object",
                                                                "Action"};

                        for(auto type : script_type)
                        {
                                                 ImGui::Text(type.c_str());
                                                 ImGui::Separator();
                                                 ImGui::Text("Use a Game Level Script to manage "
                                                             "the behavior of a Game Level \n"
                                                             "two line description");
                                                 ImGui::Dummy(ImVec2(0.f, 2.f));
                                                 if(ImGui::Button(
                                                        type.c_str(),
                                                        ImVec2(ImGui::GetWindowContentRegionWidth(),
                                                               50.f)))
                                                 {
                                                     string::fillCharArray(
                                                         m_InputParentClass,
                                                         sizeof(m_InputParentClass),
                                                         type);
                                                     ImGui::CloseCurrentPopup();
                                                 }

                                                 ImGui::Dummy(ImVec2(0.f, 5.f));
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::EndTabItem();
                }

                if(ImGui::BeginTabItem("Lua Script"))
                {

                    ImGui::EndTabItem();
                }

                ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
                ImGui::SetCursorPosY(winsow_size.y * 0.75f);
                bool onCreate = ImGui::Button("Create##create_script_object", ImVec2(100, 0));

                bool error    = false;

                if(onCreate)
                {

                    if(error)
                    {
                        // ImGui::OpenPopup("Error Creating Project");
                    }
                    else
                    {
                        Parameter parameter;
                        parameter.setString("class_name", std::string(m_InputClassName));
                        parameter.setString("script_type", std::string(m_SelectedScriptType));

                        if(std::string(m_SelectedScriptType) == "Game Level Script")
                        {
                                                 parameter.setString(
                                                     "level_name",
                                                     m_EditorContext->getSelectedGameLevelName());
                        }
                        else if(std::string(m_SelectedScriptType) == "Game Screen Script")
                        {
                                                 parameter.setString(
                                                     "screen_name",
                                                     std::string(m_SelectedGameScreen));
                        }

                        clearScriptWizardInput();

                        // createScriptObject(parameter);

                        // ImGui::OpenPopup("Script Created");
                    }
                }

                ImGui::EndTabBar();
            }

            ImGui::SetCursorPosY(winsow_size.y - 38.f);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 4.0f));
            ImGui::SetCursorPosX(winsow_size.x / 2.f - 50.f);
            if(ImGui::Button("Close##close_script_wizard_window", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }*/
    }

    void NewGameScriptPopup::clearInput()
    {
    }
} // namespace nero
