////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERWINDOW_H
#define RESOURCEBROWSERWINDOW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserWindow : public UIComponent
    {
        public:
                                    ResourceBrowserWindow(EditorContext::Ptr editorContext);
            virtual                ~ResourceBrowserWindow() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // RESOURCEBROWSERWINDOW_H
