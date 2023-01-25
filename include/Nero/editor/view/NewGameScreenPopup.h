////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NEWGAMESCREENPOPUP_H
#define NEWGAMESCREENPOPUP_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NewGameScreenPopup : public UIComponent
    {
      public:
        NewGameScreenPopup(EditorContext::Ptr editorContext);
        virtual ~NewGameScreenPopup() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void clearInput();

      private:
        struct GameScreenInput
        {
        };

      private:
        GameScreenInput m_Input;
    };
} // namespace nero
#endif // NEWGAMESCREENPOPUP_H
