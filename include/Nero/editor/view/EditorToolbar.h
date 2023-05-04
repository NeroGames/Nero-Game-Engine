////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORTOOLBAR_H
#define EDITORTOOLBAR_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/view/ToolbarScrollButton.h>
#include <Nero/editor/view/ToolbarButtonGroup.h>
#include <Nero/editor/view/ProjectManagerPopup.h>
#include <Nero/editor/view/NewGameLevelPopup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorToolbar : public UIComponent
    {
      public:
        EditorToolbar(EditorContext::Ptr editorContext);
        virtual ~EditorToolbar() override;

        virtual void destroy() override;
        virtual void render() override;
        virtual void update(const sf::Time& timeStep) override;

      private:
        void renderToolbarContent();
        void renderToolbarPopupGroup();

      private:
        ImGuiWindow*        m_ToolbarContentWindow;
        ToolbarScrollButton m_ScrollButtonLeft;
        ToolbarScrollButton m_ScrollButtonRight;
        ToolbarButtonGroup  m_ToolbarButtonGroup;
        ProjectManagerPopup m_ProjectManagerPopup;
        NewGameLevelPopup   m_NewGameLevelPopup;
    };
} // namespace nero
#endif // EDITORTOOLBAR_H
