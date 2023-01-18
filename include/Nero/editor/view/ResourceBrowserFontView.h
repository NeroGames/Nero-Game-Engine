////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERFONTVIEW_H
#define RESOURCEBROWSERFONTVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserFontView : public UIComponent {
      public:
        ResourceBrowserFontView(EditorContext::Ptr editorContext);
        virtual ~ResourceBrowserFontView() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        sf::Texture& getFontTexture(const std::string& fontName);
    };
} // namespace nero
#endif // RESOURCEBROWSERFONTVIEW_H
