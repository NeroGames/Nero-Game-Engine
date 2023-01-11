////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINEHELPWINDOW_H
#define ENGINEHELPWINDOW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EngineHelpWindow : public UIComponent
    {
        public:
                                    EngineHelpWindow(EditorContext::Ptr editorContext);
            virtual                ~EngineHelpWindow() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // ENGINEHELPWINDOW_H
