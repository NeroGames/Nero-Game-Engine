////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERANIMATIONVIEW_H
#define RESOURCEBROWSERANIMATIONVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserAnimationView : public UIComponent
    {
      public:
        ResourceBrowserAnimationView(EditorContext::Ptr editorContext);
        virtual ~ResourceBrowserAnimationView() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // RESOURCEBROWSERANIMATIONVIEW_H
