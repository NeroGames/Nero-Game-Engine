////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORMENUBAR_H
#define EDITORMENUBAR_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/view/ProjectManagerPopup.h>
#include <Nero/editor/view/AboutEnginePopup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorMenubar : public UIComponent
    {
      public:
        EditorMenubar(EditorContext::Ptr editorContext);
        virtual ~EditorMenubar() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void clearInput();
        void handleMenubarInput();

      private:
        struct MenubarInput
        {
            bool newProject;
            bool openProject;
            bool newWorkspace;
            bool aboutEngine;
        };

      private:
        ProjectManagerPopup      m_ProjectManagerPopup;
        AboutEnginePopup         m_AboutEnginePopup;
        TabSelectionHandler::Ptr m_ProjectManagerPopupTabHandler;
        MenubarInput             m_Input;
    };
} // namespace nero
#endif // EDITORMENUBAR_H
