////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NEWGAMESCRIPTPOPUP_H
#define NEWGAMESCRIPTPOPUP_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NewGameScriptPopup : public UIComponent
    {
      public:
        NewGameScriptPopup(EditorContext::Ptr editorContext);
        virtual ~NewGameScriptPopup() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void clearInput();

      private:
        struct GameScriptInput
        {
        };

      private:
        GameScriptInput m_Input;
    };
} // namespace nero
#endif // NEWGAMESCRIPTPOPUP_H
