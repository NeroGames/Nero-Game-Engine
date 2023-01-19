////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef BACKGROUNDTASKWINDOW_H
#define BACKGROUNDTASKWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class BackgroundTaskWindow : public UIComponent
    {
      public:
        BackgroundTaskWindow(EditorContext::Ptr editorContext);
        virtual ~BackgroundTaskWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // BACKGROUNDTASKWINDOW_H
