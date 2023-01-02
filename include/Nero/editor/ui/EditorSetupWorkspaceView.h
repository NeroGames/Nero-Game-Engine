////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORSETUPWORKSPACEVIEW_H
#define EDITORSETUPWORKSPACEVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/EditorSetup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorSetupWorkspaceView : public UIComponent
    {
        public:
                                    EditorSetupWorkspaceView(EditorContext::Ptr editorContext, EditorSetup::Ptr editorSetup);
            virtual                ~EditorSetupWorkspaceView() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            EditorSetup::Ptr        m_EditorSetup;
	};
}
#endif // EDITORSETUPWORKSPACEVIEW_H
