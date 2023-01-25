////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMESETTINGWINDOW_H
#define GAMESETTINGWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameSettingWindow : public UIComponent
    {
      public:
        GameSettingWindow(EditorContext::Ptr editorContext);
        virtual ~GameSettingWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // GAMESETTINGWINDOW_H
