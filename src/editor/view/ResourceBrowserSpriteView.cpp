////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ResourceBrowserSpriteView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ResourceBrowserSpriteView::ResourceBrowserSpriteView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    ResourceBrowserSpriteView::~ResourceBrowserSpriteView()
    {
        destroy();
    }

    void ResourceBrowserSpriteView::destroy()
    {
    }

    void ResourceBrowserSpriteView::render()
    {
        auto resourceManager = m_EditorContext->getCurrentResourceManager();

        if(!resourceManager)
            return;

        auto  textureHolder  = resourceManager->getTextureHolder();

        auto& spriteTable    = textureHolder->getSpriteTable();
        int   spriteCount    = spriteTable.size();
        float xWindowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for(int i = 0; i < spriteCount; i++)
        {
            sf::Vector2u textureSize = textureHolder->getSpriteTexture(spriteTable[i]).getSize();
            sf::Vector2u nextTextureSize = textureSize;

            if(i < spriteCount - 1)
            {
                nextTextureSize = textureHolder->getSpriteTexture(spriteTable[i + 1]).getSize();
            }

            sf::Vector2f spriteSize(textureSize.x, textureSize.y);
            spriteSize = formatTexturetSize(spriteSize, 250);

            sf::Vector2f nextSpriteSize(nextTextureSize.x, nextTextureSize.y);
            nextSpriteSize = formatTexturetSize(nextSpriteSize, 250);

            if(ImGui::ImageButton(textureHolder->getSpriteTexture(spriteTable[i]),
                                  ImVec2(spriteSize.x, spriteSize.y)))
            {
                if(m_EditorContext->getBuilderMode() == BuilderMode::Object)
                {
                    auto worldBuilder =
                        m_EditorContext->getLevelBuilder()->getSelectedChunk()->getWorldBuilder();

                    worldBuilder->addObject(Object::Sprite_Object,
                                            spriteTable[i],
                                            m_EditorContext->getNewGameObjectPosition());
                }
            }

            if(ImGui::BeginPopupContextItem())
            {
                if(ImGui::Button("Delete"))
                {
                    // TODO
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if(ImGui::IsItemHovered())
            {
                // TODO
            }

            float xLastButton = ImGui::GetItemRectMax().x;
            float xNextButton = xLastButton + ImGui::GetStyle().ItemSpacing.x + nextSpriteSize.x;

            if(i + 1 < spriteCount && xNextButton < xWindowVisible)
                ImGui::SameLine();
        }
    }
} // namespace nero
