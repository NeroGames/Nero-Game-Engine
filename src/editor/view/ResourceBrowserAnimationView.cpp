////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/ResourceBrowserAnimationView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ResourceBrowserAnimationView::ResourceBrowserAnimationView(EditorContext::Ptr editorContext):
         UIComponent(std::move(editorContext))
    {

    }

    ResourceBrowserAnimationView::~ResourceBrowserAnimationView()
    {
        destroy();
    }

    void ResourceBrowserAnimationView::destroy()
    {

    }

    void ResourceBrowserAnimationView::render()
    {
        auto resourceManager = m_EditorContext->getCurrentResourceManager();

        if(!resourceManager)
            return;

        auto animationHolder = resourceManager->getAnimationHolder();

        auto& animationTable        = animationHolder->getAnimationTable();
        unsigned int animationCount = animationTable.size();
        float xWindowVisible        = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (unsigned int i = 0; i < animationCount; i++)
        {
            sf::Texture& texture            = animationHolder->getTexture(animationTable[i]);
            sf::IntRect bound               = animationHolder->getAnimationBound(animationTable[i]);
            sf::Vector2u textureSize        = sf::Vector2u(bound.width, bound.height);
            sf::Vector2u nextTextureSize    = textureSize;

            if(i < animationCount - 1)
            {
                sf::IntRect nextBound = animationHolder->getAnimationBound(animationTable[i+1]);

                nextTextureSize = sf::Vector2u(nextBound.width, nextBound.height);
            }

            sf::Vector2f spriteSize(textureSize.x, textureSize.y);
            spriteSize = formatTexturetSize(spriteSize, 250);

            sf::Vector2f nextSpriteSize(nextTextureSize.x, nextTextureSize.y);
            nextSpriteSize = formatTexturetSize(nextSpriteSize, 250);

            sf::Sprite animationSprite(texture, bound);
            animationSprite.scale(2.f, 2.f);

            if(ImGui::ImageButton(animationSprite, ImVec2(spriteSize.x, spriteSize.y)))
            {
                if(m_EditorContext->getBuilderMode() == BuilderMode::OBJECT)
                {
                    auto worldBuilder = m_EditorContext->getLevelBuilder()
                                                       ->getSelectedChunk()
                                                       ->getWorldBuilder();

                    worldBuilder->addObject(Object::Animation_Object,
                                            animationTable[i],
                                            m_EditorContext->getNewGameObjectPosition());
                }
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Button("Delete"))
                {
                    //TODO
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if(ImGui::IsItemHovered())
            {
                //TODO
            }

            float xLastButton = ImGui::GetItemRectMax().x;
            float xNextButton = xLastButton + ImGui::GetStyle().ItemSpacing.x + nextSpriteSize.x;

            if (i + 1 < animationCount && xNextButton < xWindowVisible)
                ImGui::SameLine();
        }
    }
}
