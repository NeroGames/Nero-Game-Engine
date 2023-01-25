////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMESCREENWINDOW_H
#define GAMESCREENWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameScreenWindow : public UIComponent
    {
      public:
        GameScreenWindow(EditorContext::Ptr editorContext);
        virtual ~GameScreenWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // GAMESCREENWINDOW_H
