////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/AboutEnginePopup.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    AboutEnginePopup::AboutEnginePopup(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
    {

    }

    AboutEnginePopup::~AboutEnginePopup()
    {
        destroy();
    }

    void AboutEnginePopup::destroy()
    {

    }

    void AboutEnginePopup::render()
    {
        //Window flags
        ImGuiWindowFlags window_flags   = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize |
                                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        //Winsow size
        ImVec2 winsow_size              = ImVec2(400.f, 250.f);

        //Project manager window
        ImGui::SetNextWindowSize(winsow_size);
        //Begin window
        if(ImGui::BeginPopupModal(EditorConstant.WINDOW_ABOUT_ENGINE.c_str(), nullptr, window_flags))
        {
            std::string aboutEngine = "Nero Game Engine d2.0.1 (Demo version 2.0.1)\n\n"
                                      "Licence : BSD 4-Clause \"Original\" or \"Old\" License\n\n"
                                      "Built on May 24 2020\n\n"
                                      "Copyright 2016-2020 Sanou A. K. Landry. All rights reserved.\n\n"
                                      "The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.";

            ImGui::TextWrapped("%s", aboutEngine.c_str());

            ImGui::SetCursorPosY(winsow_size.y - 38.f);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 4.0f));
            ImGui::SetCursorPosX(winsow_size.x/2.f - 50.f);
            if (ImGui::Button("Close##close_about_engine", ImVec2(100, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
