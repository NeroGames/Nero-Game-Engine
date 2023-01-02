////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORSETUPPOPUP_H
#define EDITORSETUPPOPUP_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/EditorSetup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorSetupPopup : public UIComponent
    {
        public:
                                    EditorSetupPopup(EditorContext::Ptr editorContext, EditorSetup::Ptr editorSetup);
            virtual                ~EditorSetupPopup() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            EditorSetup::Ptr                            m_EditorSetup;
            unsigned int                                m_CurrentViewIndex;
            std::shared_ptr<UIComponent>                m_CurrentView;
            std::vector<std::shared_ptr<UIComponent>>   m_ViewTable;
	};
}
#endif // EDITORSETUPPOPUP_H
