////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/NewGameScreenPopup.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    NewGameScreenPopup::NewGameScreenPopup(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))

    {
        clearInput();
    }

    NewGameScreenPopup::~NewGameScreenPopup()
    {
        destroy();
    }

    void NewGameScreenPopup::destroy()
    {
        clearInput();
    }

    void NewGameScreenPopup::render()
    {
        // Window flags
        /*ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
                //Winsow size
                ImVec2 winsow_size = ImVec2(400.f, 200.f);
                ImGui::SetNextWindowSize(winsow_size);

                 //Begin window
                 if(ImGui::BeginPopupModal("Create Game Screen", nullptr, window_flags))
                 {
                         float wording_width = 130.f;
                         float input_width = ImGui::GetWindowContentRegionWidth() * 0.8f;

                          ImGui::Text("Screen Name");
                          ImGui::SameLine(wording_width);
                          ImGui::InputText("##new_screen_name", m_NewGameScreenInput.name,
          sizeof(m_NewGameScreenInput.name)); ImGui::Dummy(ImVec2(0.0f, 1.0f));

                           ImGui::Text("parent");
                           ImGui::SameLine(wording_width);
                           const char* parents[] = { "None"};
                           static int parent = 0;
                           ImGui::Combo("parent", &parent, parents, IM_ARRAYSIZE(parents));
                           ImGui::Dummy(ImVec2(0.0f, 1.0f));


                            ImGui::Text("Template");
                            ImGui::SameLine(wording_width);
                            const char* prototypes[] = { "None"};
                            static int prototype = 0;
                            ImGui::Combo("template", &prototype, prototypes,
        IM_ARRAYSIZE(prototypes)); ImGui::Dummy(ImVec2(0.0f, 1.0f));

                             ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 102.f);
                             ImGui::SetCursorPosY(winsow_size.y * 0.85f - 40.f);
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
                                              parameter.setString("screen_name",
              std::string(m_NewGameScreenInput.name));
                                              //parameter.setString("parent",
              std::string(m_NewGameScreenInput.parent)); parameter.setString("template",
              std::string(m_NewGameScreenInput.prototype));

                                             createGameScreen(parameter);
                                     }
                             }

                              ImGui::Separator();
                              ImGui::Dummy(ImVec2(0.0f, 4.0f));
                              ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
                              if (ImGui::Button("Close##close_new_screen", ImVec2(100, 0)))
                              {
                                      ImGui::CloseCurrentPopup();
                              }

                             ImGui::EndPopup();
             }*/
    }

    void NewGameScreenPopup::clearInput()
    {
    }
} // namespace nero
