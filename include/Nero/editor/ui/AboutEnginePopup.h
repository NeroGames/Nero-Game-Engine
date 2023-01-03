////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ABOUTENGINEPOPUP_H
#define ABOUTENGINEPOPUP_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class AboutEnginePopup : public UIComponent
    {
        public:
                                    AboutEnginePopup(EditorContext::Ptr editorContext);
            virtual                ~AboutEnginePopup() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // ABOUTENGINEPOPUP_H
