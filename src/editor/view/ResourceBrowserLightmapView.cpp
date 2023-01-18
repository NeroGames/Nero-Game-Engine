////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ResourceBrowserLightmapView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ResourceBrowserLightmapView::ResourceBrowserLightmapView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    ResourceBrowserLightmapView::~ResourceBrowserLightmapView()
    {
        destroy();
    }

    void ResourceBrowserLightmapView::destroy()
    {
    }

    void ResourceBrowserLightmapView::render()
    {
        auto resourceManager = m_EditorContext->getCurrentResourceManager();

        if(!resourceManager)
            return;

        auto  lightmapHolder = resourceManager->getLightmapHolder();

        auto& spriteTable    = lightmapHolder->getSpriteTable();
        int   spriteCount    = spriteTable.size();
        float xWindowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for(int i = 0; i < spriteCount; i++)
        {
            sf::Vector2u textureSize     = lightmapHolder->getSpriteTexture(spriteTable[i]).getSize();
            sf::Vector2u nextTextureSize = textureSize;

            if(i < spriteCount - 1)
            {
                nextTextureSize = lightmapHolder->getSpriteTexture(spriteTable[i + 1]).getSize();
            }

            sf::Vector2f spriteSize(textureSize.x, textureSize.y);
            spriteSize = formatTexturetSize(spriteSize, 250);

            sf::Vector2f nextSpriteSize(nextTextureSize.x, nextTextureSize.y);
            nextSpriteSize = formatTexturetSize(nextSpriteSize, 250);

            if(ImGui::ImageButton(lightmapHolder->getSpriteTexture(spriteTable[i]), ImVec2(spriteSize.x, spriteSize.y)))
            {
                if(m_EditorContext->getBuilderMode() == BuilderMode::OBJECT)
                {
                    auto worldBuilder = m_EditorContext->getLevelBuilder()->getSelectedChunk()->getWorldBuilder();
                    worldBuilder->addObject(Object::Light_Object,
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
