////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NOTIFICATIONWINDOW_H
#define NOTIFICATIONWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NotificationWindow : public UIComponent
    {
      public:
        NotificationWindow(EditorContext::Ptr editorContext);
        virtual ~NotificationWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // NOTIFICATIONWINDOW_H
