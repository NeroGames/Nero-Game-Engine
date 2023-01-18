////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMELEVELWINDOW_H
#define GAMELEVELWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameLevelWindow : public UIComponent {
      public:
        GameLevelWindow(EditorContext::Ptr editorContext);
        virtual ~GameLevelWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // GAMELEVELWINDOW_H
