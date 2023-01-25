////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMEPROJECTWINDOW_H
#define GAMEPROJECTWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameProjectWindow : public UIComponent
    {
      public:
        GameProjectWindow(EditorContext::Ptr editorContext);
        virtual ~GameProjectWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // GAMEPROJECTWINDOW_H
