////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef TOOLBARSCROLLBUTTON_H
#define TOOLBARSCROLLBUTTON_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ToolbarScrollButton : public UIComponent
    {
        public:
            enum class Direction {Left, Right, None};

        public:
                                    ToolbarScrollButton(UIComponent::EditorContext editorContext,
                                                        ImGuiWindow* toolbarContentWindow,
                                                        const Direction& direction = Direction::None);
            virtual                ~ToolbarScrollButton() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    renderLeft();
            void                    renderRight();

        private:
            Direction               m_Direction;
            ImGuiWindow*            m_ToolbarContentWindow;
	};
}
#endif // TOOLBARSCROLLBUTTON_H
