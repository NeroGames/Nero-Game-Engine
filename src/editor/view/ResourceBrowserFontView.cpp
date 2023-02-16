////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ResourceBrowserFontView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ResourceBrowserFontView::ResourceBrowserFontView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    ResourceBrowserFontView::~ResourceBrowserFontView()
    {
        destroy();
    }

    void ResourceBrowserFontView::destroy()
    {
    }

    void ResourceBrowserFontView::render()
    {
        auto resourceManager = m_EditorContext->getCurrentResourceManager();

        if(!resourceManager)
            return;

        auto         fontHolder = resourceManager->getFontHolder();
        auto&        fontTable  = fontHolder->getFontTable();
        unsigned int fontCount  = fontTable.size();

        for(unsigned int i = 0; i < fontCount; i++)
        {
            if(ImGui::ImageButton(getFontTexture(fontTable.at(i))))
            {
                if(m_EditorContext->getBuilderMode() == BuilderMode::Object)
                {
                    auto worldBuilder =
                        m_EditorContext->getLevelBuilder()->getSelectedChunk()->getWorldBuilder();

                    auto selectedObject = worldBuilder->getSelectedObject();
                    if(selectedObject && selectedObject->getSecondType() == Object::Text_Object)
                    {
                        auto textObject = TextObject::Cast(selectedObject);
                        textObject->getText().setFont(
                            m_EditorContext->getCurrentResourceManager()->getFontHolder()->getFont(
                                fontTable[i]));
                        textObject->setFont(fontTable[i]);
                    }
                    else
                    {
                        worldBuilder->addObject(Object::Text_Object,
                                                fontTable[i],
                                                m_EditorContext->getNewGameObjectPosition());
                    }
                }
            }
        }
    }

    sf::Texture& ResourceBrowserFontView::getFontTexture(const std::string& fontName)
    {
        std::string fontPath =
            file::getPath({"resource/editor/texture", fontName}, StringPool.EXT_PNG);

        // Create new font texture if not exist
        if(!file::fileExist(fontPath))
        {
            sf::RenderTexture renderTexture;
            renderTexture.create(300.f, 100.f);
            renderTexture.clear(sf::Color::White);
            sf::Text text;
            text.setFont(
                m_EditorContext->getCurrentResourceManager()->getFontHolder()->getFont(fontName));
            text.setString(fontName);
            text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
            text.setFillColor(sf::Color::Black);
            text.setPosition(150.f, 50.f);
            text.setScale(1.f, -1.f);
            renderTexture.draw(text);

            // Save to new texture
            renderTexture.getTexture().copyToImage().saveToFile(fontPath);

            // Load new texture
            m_EditorContext->getTextureHolder()->loadFile(fontPath);
        }

        return m_EditorContext->getTextureHolder()->getTexture(fontName);
    }
} // namespace nero
