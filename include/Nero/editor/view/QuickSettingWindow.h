////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef QUICKSETTINGWINDOW_H
#define QUICKSETTINGWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class QuickSettingWindow : public UIComponent
    {
      public:
        QuickSettingWindow(EditorContext::Ptr editorContext);
        virtual ~QuickSettingWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // QUICKSETTINGWINDOW_H
