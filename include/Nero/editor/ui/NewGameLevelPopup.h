////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NEWGAMELEVELPOPUP_H
#define NEWGAMELEVELPOPUP_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NewGameLevelPopup : public UIComponent
    {
        public:
                                    NewGameLevelPopup(EditorContext::Ptr editorContext);
            virtual                ~NewGameLevelPopup() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    clearInput();

        private:
            struct GameLevelInput
            {
                char name[100];
                bool enablePhysics;
                bool enableLight;
                char prototype[100];
            };

        private:
            GameLevelInput          m_Input;
	};
}
#endif // NEWGAMELEVELPOPUP_H
