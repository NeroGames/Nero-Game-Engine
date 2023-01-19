////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORUTILITYWINDOW_H
#define EDITORUTILITYWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorUtilityWindow : public UIComponent
    {
      public:
        EditorUtilityWindow(EditorContext::Ptr editorContext);
        virtual ~EditorUtilityWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // EDITORUTILITYWINDOW_H
