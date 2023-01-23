////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/GameObjectPropertyView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    GameObjectPropertyView::GameObjectPropertyView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    GameObjectPropertyView::~GameObjectPropertyView()
    {
        destroy();
    }

    void GameObjectPropertyView::destroy()
    {
    }

    void GameObjectPropertyView::render()
    {
        auto              levelBuilder = m_EditorContext->getLevelBuilder();
        WorldBuilder::Ptr worldBuilder = nullptr;

        if(levelBuilder)
            worldBuilder = levelBuilder->getSelectedChunk()->getWorldBuilder();

        if(worldBuilder)
        {
            Object::Ptr selectedObject = worldBuilder->getSelectedObject();

            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.000f, 0.000f, 0.000f, 0.675f));

            if(ImGui::CollapsingHeader("Game Object", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                float          height   = viewport->Size.y * 0.50f;
                viewport                = nullptr;
                ImGui::BeginChild("game_object", ImVec2(), true);

                ImGui::SetCursorPosX(10.f);
                if(ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::BeginChild("object_general", ImVec2(0.f, 110.f), true);
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    float wording_width = 70.f;
                    float input_width   = ImGui::GetWindowContentRegionWidth() - 70.f;

                    ImGui::Text("Name");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width);
                    char object_name[100];
                    string::fillCharArray(object_name,
                                          sizeof(object_name),
                                          selectedObject ? selectedObject->getName() : "");
                    std::string input_id =
                        selectedObject
                            ? std::string("##object_name") + toString(selectedObject->getObjectId())
                            : std::string("##object_name");
                    ImGui::InputText(input_id.c_str(), object_name, sizeof(object_name));
                    if(selectedObject && ImGui::IsItemEdited())
                    {
                        selectedObject->setName(std::string(object_name));
                    }
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));

                    ImGui::Text("Type");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width);
                    char object_type[100];
                    string::fillCharArray(object_type,
                                          sizeof(object_type),
                                          selectedObject ? selectedObject->getTypeString() : "");
                    ImGui::InputText("##object_type",
                                     object_type,
                                     sizeof(object_type),
                                     ImGuiInputTextFlags_ReadOnly);
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));

                    ImGui::Text("Category");
                    ImGui::SameLine(wording_width);
                    ImGui::SetNextItemWidth(input_width);
                    if(ImGui::BeginCombo("##object_category", {}, ImGuiComboFlags()))
                    {
                        ImGui::EndCombo();
                    }
                    ImGui::EndChild();
                }

                ImGui::SetCursorPosX(10.f);
                if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::BeginChild("object_transfrom", ImVec2(0.f, 150.f), true);
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    float wording_width = 70.f;
                    float input_width   = ImGui::GetWindowContentRegionWidth() - 70.f;

                    ImGui::Text("Position");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(" x ");
                    ImGui::SameLine(wording_width + 30.f);
                    ImGui::SetNextItemWidth(input_width - 30.f);
                    float positionx = selectedObject ? selectedObject->getPosition().x : 0.00f;
                    ImGui::InputFloat("##position_x", &positionx, 1.f, 1.0f, "%.3f");
                    bool posxchanged = ImGui::IsItemEdited();
                    ImGui::Text("");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(" y ");
                    ImGui::SameLine(wording_width + 30.f);
                    ImGui::SetNextItemWidth(input_width - 30.f);
                    float positiony = selectedObject ? selectedObject->getPosition().y : 0.00f;
                    ImGui::InputFloat("##position_y", &positiony, 1.f, 1.0f, "%.3f");
                    bool posychanged = ImGui::IsItemEdited();
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));

                    if(selectedObject && (posxchanged || posychanged))
                    {
                        selectedObject->setPosition(positionx, positiony);
                    }

                    ImGui::Text("Scale");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(" x ");
                    ImGui::SameLine(wording_width + 30.f);
                    ImGui::SetNextItemWidth(input_width - 30.f);
                    float scalex = selectedObject ? selectedObject->getScale().x : 0.00f;
                    float scaley = selectedObject ? selectedObject->getScale().y : 0.00f;
                    ImGui::InputFloat("##scale_x", &scalex, 0.1f, 1.0f, "%.3f");
                    bool scalexchanged = ImGui::IsItemEdited();
                    ImGui::Text("");
                    ImGui::SameLine(wording_width);
                    ImGui::Text(" y ");
                    ImGui::SameLine(wording_width + 30.f);
                    ImGui::SetNextItemWidth(input_width - 30.f);
                    ImGui::InputFloat("##scale_y", &scaley, 0.1f, 1.0f, "%.3f");
                    bool scaleychanged = ImGui::IsItemEdited();
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));

                    if(selectedObject && (scalexchanged || scaleychanged))
                    {
                        selectedObject->setScale(scalex, scaley);
                    }

                    ImGui::Text("Rotation");
                    ImGui::SameLine(wording_width + 30.f);
                    ImGui::SetNextItemWidth(input_width - 30.f);
                    float rotation = selectedObject ? selectedObject->getRotation() : 0.00f;
                    ImGui::InputFloat("", &rotation, 1.f, 1.0f, "%.3f");
                    if(selectedObject && ImGui::IsItemEdited())
                    {
                        selectedObject->setRotation(rotation);
                    }

                    ImGui::EndChild();
                }

                if(selectedObject)
                {
                    std::vector<Object::Ptr> componentTable = getComponentTable(selectedObject);
                    for(Object::Ptr component : componentTable)
                    {
                        switch(component->getFirstType())
                        {
                            case Object::Sprite_Object:
                            {
                                ImGui::SetCursorPosX(10.f);
                                if(ImGui::CollapsingHeader("Sprite",
                                                           ImGuiTreeNodeFlags_DefaultOpen))
                                {
                                    ImGui::BeginChild("object_sprite", ImVec2(0.f, 70.f), true);

                                    float wording_width = 70.f;
                                    float input_width = ImGui::GetWindowContentRegionWidth() - 70.f;

                                    SpriteObject::Ptr spriteObject = SpriteObject::Cast(component);

                                    ImGui::Text("Sprite");
                                    ImGui::SameLine(wording_width);
                                    ImGui::SetNextItemWidth(input_width);
                                    char* object_sprite = "";
                                    ImGui::InputText("##object_sprite",
                                                     object_sprite,
                                                     sizeof(object_sprite),
                                                     ImGuiInputTextFlags_ReadOnly);
                                    ImGui::Dummy(ImVec2(0.0f, 1.0f));
                                    ImGui::Text("Texture");
                                    ImGui::SameLine(wording_width);
                                    ImGui::SetNextItemWidth(input_width);
                                    char* sprite_texture = "";
                                    ImGui::InputText("##sprite_texture",
                                                     object_sprite,
                                                     sizeof(sprite_texture),
                                                     ImGuiInputTextFlags_ReadOnly);
                                    ImGui::Dummy(ImVec2(0.0f, 1.0f));

                                    ImGui::EndChild();
                                }
                            }
                            break;

                            case Object::Mesh_Object:
                            {
                                if(ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
                                {
                                    ImGui::BeginChild("mesh_object", ImVec2(0.f, 100.f), true);

                                    float wording_width = 70.f;
                                    float input_width = ImGui::GetWindowContentRegionWidth() - 70.f;
                                    MeshObject::Ptr meshObject = MeshObject::Cast(component);

                                    ImGui::Text("Shape");
                                    ImGui::SameLine(wording_width);
                                    ImGui::SetNextItemWidth(input_width);
                                    char* object_sprite = "";
                                    ImGui::InputText("##mesh_shape",
                                                     object_sprite,
                                                     sizeof(object_sprite),
                                                     ImGuiInputTextFlags_ReadOnly);
                                    ImGui::Dummy(ImVec2(0.0f, 1.0f));
                                    ImGui::Text("Type");
                                    ImGui::SameLine(wording_width);
                                    ImGui::SetNextItemWidth(input_width);
                                    char* sprite_texture = "";
                                    ImGui::InputText("##mesh_type",
                                                     object_sprite,
                                                     sizeof(sprite_texture),
                                                     ImGuiInputTextFlags_ReadOnly);
                                    ImGui::Dummy(ImVec2(0.0f, 1.0f));

                                    ImGui::EndChild();
                                }

                                if(ImGui::CollapsingHeader("Physics",
                                                           ImGuiTreeNodeFlags_DefaultOpen))
                                {
                                    ImGui::BeginChild("physics_data", ImVec2(0.f, 100.f), true);

                                    MeshObject::Ptr meshObject = MeshObject::Cast(component);

                                    ImGui::EndChild();
                                }
                            }
                            break;
                        }
                    }
                }

                ImGui::EndChild();
            }

            ImGui::PopStyleColor();
        }
    }

    std::vector<Object::Ptr> GameObjectPropertyView::getComponentTable(Object::Ptr root)
    {
        std::vector<Object::Ptr> result;

        getComponentTable(root, result);

        return result;
    }

    void GameObjectPropertyView::getComponentTable(Object::Ptr               object,
                                                   std::vector<Object::Ptr>& result)
    {
        if(object != nullptr)
        {
            result.push_back(object);
        }

        auto childTab = object->getAllChild();
        for(auto it = childTab->rbegin(); it != childTab->rend(); it++)
        {
            getComponentTable(*it, result);
        }
    }

} // namespace nero
