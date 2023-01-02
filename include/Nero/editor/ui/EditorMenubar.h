////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORMENUBAR_H
#define EDITORMENUBAR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorMenubar : public UIComponent
    {
        public:
                                    EditorMenubar(EditorContext::Ptr editorContext);
            virtual                ~EditorMenubar() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // EDITORMENUBAR_H
