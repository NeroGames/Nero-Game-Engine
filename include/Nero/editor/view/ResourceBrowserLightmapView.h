////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERLIGHTMAPVIEW_H
#define RESOURCEBROWSERLIGHTMAPVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserLightmapView : public UIComponent
    {
        public:
                                    ResourceBrowserLightmapView(EditorContext::Ptr editorContext);
            virtual                ~ResourceBrowserLightmapView() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // RESOURCEBROWSERLIGHTMAPVIEW_H
