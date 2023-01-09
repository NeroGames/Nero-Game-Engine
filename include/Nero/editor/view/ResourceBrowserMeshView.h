////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERMESHVIEW_H
#define RESOURCEBROWSERMESHVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserMeshView : public UIComponent
    {
        public:
                                    ResourceBrowserMeshView(EditorContext::Ptr editorContext);
            virtual                ~ResourceBrowserMeshView() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // RESOURCEBROWSERMESHVIEW_H
