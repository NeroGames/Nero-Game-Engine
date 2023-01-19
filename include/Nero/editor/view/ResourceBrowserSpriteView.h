////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERSPRITEVIEW_H
#define RESOURCEBROWSERSPRITEVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserSpriteView : public UIComponent
    {
      public:
        ResourceBrowserSpriteView(EditorContext::Ptr editorContext);
        virtual ~ResourceBrowserSpriteView() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // RESOURCEBROWSERSPRITEVIEW_H
