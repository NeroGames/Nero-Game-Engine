////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ObjectLayerWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ObjectLayerWindow::ObjectLayerWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_SelectedObjectLayerId(-1)
    {
    }

    ObjectLayerWindow::~ObjectLayerWindow()
    {
        destroy();
    }

    void ObjectLayerWindow::destroy()
    {
    }

    void ObjectLayerWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_LAYER.c_str());

        ImGui::Text("Manage Object Layers");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.f, 2.f));

        float width = 90.f;

        ImGui::Dummy(ImVec2(0.f, 2.f));

        ImVec2 button_size = ImVec2(width, 0.f);

        if(ImGui::Button("Add##add_object_layer", button_size))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            levelBuilder->getSelectedChunk()->getWorldBuilder()->addLayer();
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove##remove_object_layer", button_size))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            worldBuilder->deleteLayer(worldBuilder->getSelectedLayer()->getObjectId());
        }

        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImGui::BeginChild("##manage_object_layer", ImVec2(), true);

        auto levelBuilder = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            if(worldBuilder)
            {
                m_SelectedObjectLayerId = worldBuilder->getSelectedLayer()->getObjectId();

                for(const auto& objectLayer : worldBuilder->getLayerTable())
                {
                    std::string itemId = "##select_layer" + toString(objectLayer->getObjectId());
                    ImGui::RadioButton(itemId.c_str(),
                                       &m_SelectedObjectLayerId,
                                       objectLayer->getObjectId());

                    if(ImGui::IsItemClicked())
                    {
                        worldBuilder->setSelectedLayer(objectLayer);
                    }

                    ImGui::SameLine();

                    itemId = "##layer_visible" + toString(objectLayer->getObjectId());
                    ImGui::Checkbox(itemId.c_str(), &objectLayer->getVisibility());

                    ImGui::SameLine();

                    auto color = getLayerColor(objectLayer->getSecondType());

                    ImGui::PushStyleColor(ImGuiCol_FrameBg, std::get<0>(color));
                    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, std::get<1>(color));

                    char layer_name[100];
                    string::fillCharArray(layer_name, sizeof(layer_name), objectLayer->getName());
                    ImGui::SetNextItemWidth(118.f);
                    itemId = "##layer_name" + toString(objectLayer->getObjectId());
                    ImGui::InputText(itemId.c_str(), layer_name, sizeof(layer_name));

                    if(ImGui::IsItemEdited())
                    {
                        objectLayer->setName(std::string(layer_name));
                    }

                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }

    std::tuple<ImVec4, ImVec4> ObjectLayerWindow::getLayerColor(Object::Type type)
    {
        switch(type)
        {
            case Object::Sprite_Object:
                return {
                    ImVec4(0.3f, 0.6f, 0.5f, 0.5f),
                    ImVec4(0.3f, 0.6f, 0.5f, 1.f),
                };

            default:
                return {ImGui::GetStyle().Colors[ImGuiCol_FrameBg],
                        ImGui::GetStyle().Colors[ImGuiCol_TextSelectedBg]};
        }
    }
} // namespace nero
