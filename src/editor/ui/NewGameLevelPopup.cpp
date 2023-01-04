////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/NewGameLevelPopup.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    NewGameLevelPopup::NewGameLevelPopup(EditorContext::Ptr editorContext):
        UIComponent(editorContext)

    {
        clearInput();
    }

    NewGameLevelPopup::~NewGameLevelPopup()
    {
        destroy();
    }

    void NewGameLevelPopup::destroy()
    {
        clearInput();
    }

    void NewGameLevelPopup::render()
    {
        // Window flags
        ImGuiWindowFlags windowFlags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        // Window size
        ImVec2 winsowSize = ImVec2(400.f, 180.f);
        ImGui::SetNextWindowSize(winsowSize);

        // Begin window
        // TODO replace all string (popup title and wording) with constants
        if(ImGui::BeginPopupModal("Create Game Level", nullptr, windowFlags))
        {
            float wordingWidth  = 130.f;
            float inputWidth    = ImGui::GetWindowContentRegionWidth() * 0.8f;

            ImGui::Text("Level Name");
            ImGui::SameLine(wordingWidth);
            ImGui::InputText("##new_level_name", m_Input.name, sizeof(m_Input.name));
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::Text("Enable Physics");
            ImGui::SameLine(wordingWidth);
            ImGui::Checkbox("##new_level_physics", &m_Input.enablePhysics);
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::Text("Enable Light");
            ImGui::SameLine(wordingWidth);
            ImGui::Checkbox("##new_level_light", &m_Input.enableLight);
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::Text("Template");
            ImGui::SameLine(wordingWidth);
            const char* itemTable[] = {"None"};
            static int currentItem = 0;
            ImGui::Combo("combo", &currentItem, itemTable, IM_ARRAYSIZE(itemTable));
            ImGui::Dummy(ImVec2(0.0f, 1.0f));

            ImGui::SetCursorPosY(winsowSize.y - 30.f);

            ImGui::SetCursorPosX(50.f);
            if(ImGui::Button("Close##clow_new_level", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine(50.f + 100.f + 100.f);
            bool onCreate = ImGui::Button("Create", ImVec2(100, 0));
            ImGui::Dummy(ImVec2(0.0f, 4.0f));

            bool error = false;

            if(onCreate)
            {
                if(error)
                {

                }
                else
                {
                    Parameter parameter;
                    parameter.setString("level_name", string::trim(std::string(m_Input.name)));
                    parameter.setBool("enable_physics", m_Input.enablePhysics);
                    parameter.setBool("enable_light", m_Input.enableLight);
                    parameter.setString("template", string::trim(std::string(m_Input.prototype)));

                    m_EditorContext->getEditorProxy()->createGameLevel(parameter);

                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

    void NewGameLevelPopup::clearInput()
    {
        string::fillCharArray(m_Input.name,         sizeof(m_Input.name),       StringPool.BLANK);
        string::fillCharArray(m_Input.prototype,    sizeof(m_Input.prototype),  StringPool.BLANK);
        m_Input.enableLight = true;
        m_Input.enablePhysics = true;
    }
}
