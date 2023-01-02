////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORTOOLBAR_H
#define EDITORTOOLBAR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/ui/ToolbarScrollButton.h>
#include <Nero/editor/ui/ToolbarButtonGroup.h>
#include <Nero/editor/ui/ProjectManagerPopup.h>
#include <Nero/editor/ui/NewGameLevelPopup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorToolbar : public UIComponent
    {
        public:
                                    EditorToolbar(EditorContext::Ptr editorContext);
            virtual                ~EditorToolbar() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    renderToolbarContent();
            void                    renderToolbarButtonGroup();
            void                    renderToolbarPopupGroup();

        private:
            ImGuiWindow*            m_ToolbarContentWindow;
            ToolbarScrollButton     m_ScrollButtonLeft;
            ToolbarScrollButton     m_ScrollButtonRight;
            ToolbarButtonGroup      m_ToolbarButtonGroup;
            ProjectManagerPopup     m_ProjectManagerPopup;
            NewGameLevelPopup       m_NewGameLevelPopup;
	};
}
#endif // EDITORTOOLBAR_H
