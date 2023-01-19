////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/NewGameLevelPopup.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    NewGameLevelPopup::NewGameLevelPopup(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))

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
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                       ImGuiWindowFlags_NoScrollbar;
        // Window size
        ImVec2 winsowSize = ImVec2(400.f, 180.f);
        ImGui::SetNextWindowSize(winsowSize);

        // Begin window
        // TODO replace all string (popup title and wording) with constants
        if(ImGui::BeginPopupModal("Create Game Level", nullptr, windowFlags))
        {
            float wordingWidth = 130.f;
            float inputWidth   = ImGui::GetWindowContentRegionWidth() * 0.8f;

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

            // TODO
            ImGui::Text("Template");
            ImGui::SameLine(wordingWidth);
            const char* itemTable[] = {"None"};
            static int  currentItem = 0;
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

            if(onCreate)
            {
                m_Input.errorMessage = StringPool.BLANK;
                m_Input.redirectLink = StringPool.BLANK;
                m_Input.error        = true;

                if(std::string(m_Input.name) == StringPool.BLANK)
                {
                    m_Input.errorMessage = "Please enter a Level Name";
                    m_Input.redirectLink = StringPool.BLANK;
                }
                else
                {
                    m_Input.error = false;
                }

                if(onCreate && m_Input.error)
                {
                    ImGui::OpenPopup(EditorConstant.MODAL_ERROR_CREATING_LEVEL.c_str());
                }
                else if(onCreate)
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

            ImGuiWindowFlags modal_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
                                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                           ImGuiWindowFlags_NoScrollbar;
            ImGui::SetNextWindowSize(ImVec2(300.f, 130.f));
            if(ImGui::BeginPopupModal(EditorConstant.MODAL_ERROR_CREATING_LEVEL.c_str(),
                                      nullptr,
                                      modal_flags))
            {
                ImGui::TextWrapped("%s", m_Input.errorMessage.c_str());

                if(m_Input.redirectLink != StringPool.BLANK)
                {
                    ImGui::Dummy(ImVec2(0.f, 5.f));

                    ImGui::SetCursorPosX((300.f - 150.f) / 2.f);
                    if(ImGui::Button("Learn more here", ImVec2(150.f, 0.f)))
                    {
                        cmd::launchBrowser(m_Input.redirectLink);
                    }
                }

                ImGui::Dummy(ImVec2(0.0f, 35.0f));

                ImGui::SetCursorPosY(130.f - 30.f);
                ImGui::SetCursorPosX((300.f - 75.f) / 2.f);
                if(ImGui::Button("Close", ImVec2(75.f, 20.f)))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            ImGui::EndPopup();
        }
    }

    void NewGameLevelPopup::clearInput()
    {
        string::fillCharArray(m_Input.name, sizeof(m_Input.name), StringPool.BLANK);
        string::fillCharArray(m_Input.prototype, sizeof(m_Input.prototype), StringPool.BLANK);
        m_Input.enableLight   = true;
        m_Input.enablePhysics = true;
        m_Input.errorMessage  = StringPool.BLANK;
        m_Input.redirectLink  = StringPool.BLANK;
        m_Input.error         = true;
    }
} // namespace nero
