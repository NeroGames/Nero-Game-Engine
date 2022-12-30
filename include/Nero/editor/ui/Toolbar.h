////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef TOOLBAR_H
#define TOOLBAR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/ui/ToolbarScrollButton.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class Toolbar : public UIComponent
    {
        public:
                                    Toolbar(UIComponent::EditorContext editorContext);
            virtual                ~Toolbar() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    renderToolbar();

        private:
            ImGuiWindow*            m_ToolbarContentWindow;
            ToolbarScrollButton     m_ScrollButtonLeft;
            ToolbarScrollButton     m_ScrollButtonRight;
	};
}
#endif // TOOLBAR_H
