////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef OBJECTLAYERWINDOW_H
#define OBJECTLAYERWINDOW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ObjectLayerWindow : public UIComponent
    {
        public:
                                    ObjectLayerWindow(EditorContext::Ptr editorContext);
            virtual                ~ObjectLayerWindow() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // OBJECTLAYERWINDOW_H
