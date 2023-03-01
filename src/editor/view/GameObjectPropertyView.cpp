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
        , m_GameObjectColor(1.f, 1.f, 1.f, 1.f)
        , m_TextOutlineColor(1.f, 1.f, 1.f, 1.f)
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

            if(ImGui::CollapsingHeader("Game Object",
                                       ImGuiTreeNodeFlags_DefaultOpen |
                                           ImGuiWindowFlags_NoScrollWithMouse))
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                float          height   = viewport->Size.y * 0.5f;
                viewport                = nullptr;
                ImGui::BeginChild("game_object", ImVec2(), true);

                ImGui::SetCursorPosX(10.f);
                if(ImGui::CollapsingHeader("General",
                                           ImGuiTreeNodeFlags_DefaultOpen |
                                               ImGuiWindowFlags_NoScrollWithMouse))
                {
                    renderGeneralProperty(selectedObject);
                }

                ImGui::SetCursorPosX(10.f);
                if(ImGui::CollapsingHeader("Transform",
                                           ImGuiTreeNodeFlags_DefaultOpen |
                                               ImGuiWindowFlags_NoScrollWithMouse))
                {
                    renderTransformProperty(selectedObject);
                }

                if(ImGui::CollapsingHeader("Color",
                                           ImGuiTreeNodeFlags_DefaultOpen |
                                               ImGuiWindowFlags_NoScrollWithMouse))
                {
                    renderColorProperty(selectedObject);
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
                                                           ImGuiTreeNodeFlags_DefaultOpen |
                                                               ImGuiWindowFlags_NoScrollWithMouse))
                                {
                                    renderSpriteProperty(SpriteObject::Cast(component));
                                }
                            }
                            break;

                            case Object::Mesh_Object:
                            {
                                if(ImGui::CollapsingHeader("Mesh",
                                                           ImGuiTreeNodeFlags_DefaultOpen |
                                                               ImGuiWindowFlags_NoScrollWithMouse))
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
                                                           ImGuiTreeNodeFlags_DefaultOpen |
                                                               ImGuiWindowFlags_NoScrollWithMouse))
                                {
                                    ImGui::BeginChild("physics_data", ImVec2(0.f, 100.f), true);

                                    MeshObject::Ptr meshObject = MeshObject::Cast(component);

                                    ImGui::EndChild();
                                }
                            }
                            break;

                            case Object::Animation_Object:
                            {
                                if(ImGui::CollapsingHeader("Animation",
                                                           ImGuiTreeNodeFlags_DefaultOpen |
                                                               ImGuiWindowFlags_NoScrollWithMouse))
                                {
                                    renderAnimationProperty(AnimationObject::Cast(component));
                                }
                            }
                            break;

                            case Object::Text_Object:
                            {
                                if(ImGui::CollapsingHeader("Text",
                                                           ImGuiTreeNodeFlags_DefaultOpen |
                                                               ImGuiWindowFlags_NoScrollWithMouse))
                                {
                                    renderTextProperty(TextObject::Cast(component));
                                }
                            }
                            break;

                            case Object::Light_Object:
                            {
                                if(ImGui::CollapsingHeader("Light",
                                                           ImGuiTreeNodeFlags_DefaultOpen |
                                                               ImGuiWindowFlags_NoScrollWithMouse))
                                {
                                    renderLightProperty(LightIcon::Cast(component));
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

    void GameObjectPropertyView::renderGeneralProperty(Object::Ptr selectedObject)
    {
        ImGui::BeginChild("object_general", ImVec2(0.f, 140.f), true);
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
            selectedObject ? std::string("##object_name") + toString(selectedObject->getObjectId())
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
            // TODO
            ImGui::EndCombo();
        }
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImVec2 buttonSize = ImVec2((ImGui::GetWindowContentRegionWidth() - 8.f) / 2.f, 0.f);

        if(ImGui::Button("Move Up", buttonSize))
        {
            if(selectedObject)
            {
                m_EditorContext->getLevelBuilder()
                    ->getSelectedChunk()
                    ->getWorldBuilder()
                    ->moveObjectUp();
            }
        }

        ImGui::SameLine();

        if(ImGui::Button("Move Down", buttonSize))
        {
            if(selectedObject)
            {
                m_EditorContext->getLevelBuilder()
                    ->getSelectedChunk()
                    ->getWorldBuilder()
                    ->moveObjectDown();
            }
        }
        ImGui::EndChild();
    }

    void GameObjectPropertyView::renderTransformProperty(Object::Ptr selectedObject)
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

    void GameObjectPropertyView::renderColorProperty(Object::Ptr selectedObject)
    {
        ImGui::BeginChild("color_picker", ImVec2(0.f, 35.f), true);
        float wording_width = 70.f;
        ImGui::Text("Color");
        ImGui::SameLine(wording_width);

        if(selectedObject)
        {
            sf::Color color = selectedObject->getColor();
            ImVec4    tempColor(color.r / 255.f, color.g / 255.f, color.g / 255.f, color.a / 255.f);

            if(tempColor.x != m_GameObjectColor.x && tempColor.y != m_GameObjectColor.y &&
               tempColor.z != m_GameObjectColor.z && tempColor.w != m_GameObjectColor.w)
            {
                m_GameObjectColor = tempColor;
            }
        }
        ImGui::ColorEdit4("##color",
                          (float*)&m_GameObjectColor,
                          ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop);
        if(selectedObject && ImGui::IsItemEdited())
        {
            selectedObject->setColor(sf::Color(static_cast<sf::Uint8>(m_GameObjectColor.x * 255),
                                               static_cast<sf::Uint8>(m_GameObjectColor.y * 255),
                                               static_cast<sf::Uint8>(m_GameObjectColor.z * 255),
                                               static_cast<sf::Uint8>(m_GameObjectColor.w * 255)));
        }

        ImGui::EndChild();
    }

    void GameObjectPropertyView::renderSpriteProperty(SpriteObject::Ptr spriteObject)
    {
        ImGui::BeginChild("object_sprite", ImVec2(0.f, 40.f), true);

        float wording_width = 70.f;
        float input_width   = ImGui::GetWindowContentRegionWidth() - 70.f;

        ImGui::Text("Texture");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        std::string textureName = spriteObject->getTextureName();
        char        spriteTexture[256];
        string::fillCharArray(spriteTexture, sizeof(spriteTexture), textureName);
        ImGui::InputText("##sprite_texture",
                         spriteTexture,
                         sizeof(spriteTexture),
                         ImGuiInputTextFlags_ReadOnly);
        ImGui::Dummy(ImVec2(0.0f, 1.0f));

        ImGui::EndChild();
    }

    void GameObjectPropertyView::renderTextProperty(TextObject::Ptr textObject)
    {
        ImGui::BeginChild("object_sprite", ImVec2(0.f, 220.f), true);

        float wording_width = 120.f;
        float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width;

        ImGui::Text("Texture");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        std::string content = textObject->getContent();
        char        contentChar[5000];
        string::fillCharArray(contentChar, sizeof(contentChar), content);
        ImGui::InputText("##sprite_texture", contentChar, sizeof(contentChar));
        if(ImGui::IsItemEdited())
        {
            textObject->setContent(std::string(contentChar));
        }
        ImGui::Dummy(ImVec2(0.0f, 1.0f));

        ImGui::Text("Font Size");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        float fontSize = textObject->getFontSize();
        ImGui::InputFloat("##font_size", &fontSize, 0.5f, 1.0f, "%.3f");
        if(ImGui::IsItemEdited())
        {
            textObject->setFontSize(fontSize);
        }
        ImGui::Dummy(ImVec2(0.0f, 1.0f));

        ImGui::Text("Outline Size");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        float outlineThickness = textObject->getOutlineThickness();
        ImGui::InputFloat("##outline_hickness", &outlineThickness, 0.5f, 1.0f, "%.3f");
        if(ImGui::IsItemEdited())
        {
            textObject->setOutlineThickness(outlineThickness);
        }
        ImGui::Dummy(ImVec2(0.0f, 1.0f));

        ImGui::Text("Outline Color");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        sf::Color color = textObject->getOutlineColor();
        ImVec4    tempColor(color.r / 255.f, color.g / 255.f, color.g / 255.f, color.a / 255.f);

        if(tempColor.x != m_TextOutlineColor.x && tempColor.y != m_TextOutlineColor.y &&
           tempColor.z != m_TextOutlineColor.z && tempColor.w != m_TextOutlineColor.w)
        {
            m_TextOutlineColor = tempColor;
        }

        ImGui::ColorEdit4("##outlineColor",
                          &m_TextOutlineColor.x,
                          ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop);
        if(ImGui::IsItemEdited())
        {
            textObject->setOutlineColor(
                sf::Color(static_cast<sf::Uint8>(m_TextOutlineColor.x * 255),
                          static_cast<sf::Uint8>(m_TextOutlineColor.y * 255),
                          static_cast<sf::Uint8>(m_TextOutlineColor.z * 255),
                          static_cast<sf::Uint8>(m_TextOutlineColor.w * 255)));
        }
        ImGui::Dummy(ImVec2(0.0f, 1.0f));

        ImGui::Text("Bold");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        bool bold = textObject->isBold();
        ImGui::Checkbox("##bold_style", &bold);
        if(ImGui::IsItemEdited())
        {
            textObject->setStyle(bold,
                                 textObject->isItalic(),
                                 textObject->isUnderlined(),
                                 textObject->isStrikeThrough());
        }

        ImGui::Text("Italic");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        bool italic = textObject->isItalic();
        ImGui::Checkbox("##italic_style", &italic);
        if(ImGui::IsItemEdited())
        {
            textObject->setStyle(textObject->isBold(),
                                 italic,
                                 textObject->isUnderlined(),
                                 textObject->isStrikeThrough());
        }

        ImGui::Text("Underline");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        bool underline = textObject->isUnderlined();
        ImGui::Checkbox("##underline_style", &underline);
        if(ImGui::IsItemEdited())
        {
            textObject->setStyle(textObject->isBold(),
                                 textObject->isItalic(),
                                 underline,
                                 textObject->isStrikeThrough());
        }

        ImGui::Text("Strike Through");
        ImGui::SameLine(wording_width);
        ImGui::SetNextItemWidth(input_width);
        bool strikeThrough = textObject->isStrikeThrough();
        ImGui::Checkbox("##strike_through_line", &strikeThrough);
        if(ImGui::IsItemEdited())
        {
            textObject->setStyle(textObject->isBold(),
                                 textObject->isItalic(),
                                 textObject->isUnderlined(),
                                 strikeThrough);
        }

        ImGui::EndChild();
    }

    void GameObjectPropertyView::renderAnimationProperty(AnimationObject::Ptr animationObject)
    {
        auto& sequenceMap   = animationObject->getSequenceMap();
        float wording_width = 100.f;
        float input_width   = ImGui::GetWindowContentRegionWidth() - wording_width - 10.f;

        for(auto it = sequenceMap.begin(); it != sequenceMap.end(); it++)
        {
            ImGui::BeginChild(it->first.c_str(), ImVec2(0.f, 120.f), true);

            // Sequence Name
            ImGui::Text("Sequence");
            ImGui::SameLine(wording_width);
            ImGui::SetNextItemWidth(input_width);
            char sequenceName[256];
            string::fillCharArray(sequenceName, sizeof(sequenceName), it->first);
            std::string sequenceNameId = std::string("##") + it->first + "_name";
            ImGui::InputText(sequenceNameId.c_str(),
                             sequenceName,
                             sizeof(sequenceName),
                             ImGuiInputTextFlags_ReadOnly);
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::Text("Framerate");
            ImGui::SameLine(wording_width);
            ImGui::SetNextItemWidth(input_width);
            std::string framerateId = std::string("##") + sequenceName + "_framerate";
            float       framerate   = 1.f / it->second.getFrameRate();
            ImGui::InputFloat("##framerate", &framerate, 0.5f, 1.0f, "%.3f");
            if(ImGui::IsItemEdited())
            {
                animationObject->setSequence(it->first);
                animationObject->setFrameRate(1.f / framerate);
            }
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            // Looping
            ImGui::Text("Loop");
            ImGui::SameLine(wording_width);
            ImGui::SetNextItemWidth(input_width);
            bool        loopSequence = it->second.getLoop();
            std::string loopId       = std::string("##") + it->first + "_loop";

            ImGui::Checkbox(loopId.c_str(), &loopSequence);
            if(ImGui::IsItemEdited())
            {
                animationObject->setSequence(it->first);
                animationObject->setLoop(loopSequence);
            }
            ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 70.f);
            std::string selectButton = std::string("Select##") + it->first + "_select";
            if(ImGui::Button(selectButton.c_str(), ImVec2(70.f, 0.f)))
            {
                animationObject->setSequence(it->first);
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            ImGui::EndChild();
        }
    }

    void GameObjectPropertyView::renderLightProperty(LightIcon::Ptr lightIconObject)
    {
        ImGui::BeginChild("object_light", ImVec2(0.f, 35.f), true);
        bool lightEnabled = lightIconObject->getLightEnabled();
        ImGui::Checkbox("Enable Light##light_enabled", &lightEnabled);
        if(ImGui::IsItemEdited())
        {
            lightIconObject->setLightEnabled(lightEnabled);
        }
        ImGui::EndChild();
    }

} // namespace nero
