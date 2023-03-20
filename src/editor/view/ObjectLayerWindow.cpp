////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ObjectLayerWindow.h>
#include <Nero/editor/EditorConstant.h>
// Boost
#include <boost/range/adaptor/reversed.hpp>
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

        ImGui::Dummy(ImVec2(0.f, 2.f));

        ImVec2 smallButtonSize = ImVec2((ImGui::GetWindowContentRegionWidth() - 24.f) / 4.f, 0.f);
        ImVec2 buttonSize      = ImVec2((ImGui::GetWindowContentRegionWidth() - 8.f) / 2.f, 0.f);

        if(ImGui::Button("Add##add_object_layer", smallButtonSize))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            levelBuilder->getSelectedChunk()->getWorldBuilder()->addLayer();
        }

        ImGui::SameLine();

        if(ImGui::Button("Up##moveup_object_layer", smallButtonSize))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            worldBuilder->moveLayerUp(worldBuilder->getSelectedLayer()->getObjectId());
        }

        ImGui::SameLine();

        if(ImGui::Button("Down##movedown_object_layer", smallButtonSize))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            worldBuilder->moveLayerDown(worldBuilder->getSelectedLayer()->getObjectId());
        }

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.000f));
        if(ImGui::Button("Del##remove_object_layer", smallButtonSize))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            worldBuilder->deleteLayer(worldBuilder->getSelectedLayer()->getObjectId());
        }
        ImGui::PopStyleColor();

        if(ImGui::Button("Merge Up", buttonSize))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            worldBuilder->mergeLayerUp(worldBuilder->getSelectedLayer()->getObjectId());
        }

        ImGui::SameLine();

        if(ImGui::Button("Merge Down", buttonSize))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(!levelBuilder)
                return;

            auto worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

            worldBuilder->mergeLayerDown(worldBuilder->getSelectedLayer()->getObjectId());
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

                for(const auto& objectLayer :
                    boost::adaptors::reverse(worldBuilder->getLayerTable()))
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
                    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth() - 55.f);
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
                return {ImVec4(0.f, 0.698f, 1.f, 1.f), ImVec4(0.f, 0.698f, 1.f, 1.f)};

            case Object::Light_Object:
                return {ImVec4(1.f, 0.497f, 0.f, 1.f), ImVec4(1.f, 0.497f, 0.f, 1.f)};

            case Object::Animation_Object:
                return {ImVec4(0.630f, 0.234f, 0.692f, 1.f), ImVec4(0.630f, 0.234f, 0.692f, 1.f)};

            case Object::Text_Object:
                return {ImVec4(0.f, 0.834f, 0.790f, 1.f), ImVec4(0.f, 0.834f, 0.790f, 1.f)};

            case Object::Mesh_Object:
                return {ImVec4(0.009f, 0.751f, 0.f, 1.f), ImVec4(0.009f, 0.751f, 0.f, 1.f)};

            default:
                return {ImGui::GetStyle().Colors[ImGuiCol_FrameBg],
                        ImGui::GetStyle().Colors[ImGuiCol_TextSelectedBg]};
        }
    }
} // namespace nero
