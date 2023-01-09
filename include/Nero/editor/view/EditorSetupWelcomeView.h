////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORSETUPWELCOMEVIEW_H
#define EDITORSETUPWELCOMEVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/EditorSetup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorSetupWelcomeView : public UIComponent
    {
        public:
                                    EditorSetupWelcomeView(EditorContext::Ptr editorContext, EditorSetup::Ptr editorSetup);
            virtual                ~EditorSetupWelcomeView() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            EditorSetup::Ptr        m_EditorSetup;
	};
}
#endif // RECENTPROJECTTAB_H
