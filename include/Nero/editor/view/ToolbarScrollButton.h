////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef TOOLBARSCROLLBUTTON_H
#define TOOLBARSCROLLBUTTON_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ToolbarScrollButton : public UIComponent {
      public:
        enum class Direction
        {
            Left,
            Right,
            None
        };

      public:
        ToolbarScrollButton(EditorContext::Ptr editorContext,
                            ImGuiWindow*       toolbarContentWindow,
                            const Direction&   direction = Direction::None);
        virtual ~ToolbarScrollButton() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void renderLeft();
        void renderRight();

      private:
        Direction    m_Direction;
        ImGuiWindow* m_ToolbarContentWindow;
    };
} // namespace nero
#endif // TOOLBARSCROLLBUTTON_H
