////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef TOOLBARBUTTONGROUP_H
#define TOOLBARBUTTONGROUP_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/view/ToolbarScrollButton.h>
// Std
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
    class ToolbarButtonGroup : public UIComponent
    {
      public:
        ToolbarButtonGroup(EditorContext::Ptr editorContext);
        virtual ~ToolbarButtonGroup() override;

        virtual void destroy() override;
        virtual void render() override;
        virtual void update(const sf::Time& timeStep) override;

      private:
        void renderLeftButtonGroup();
        void renderMiddleButtonGroup();
        void renderRightButtonGroup();

      private:
        bool              m_PlayGameLevel;
        std::future<int>  m_PlayGameFuture;
        LevelBuilder::Ptr m_LevelBuilder;
        EditorMode        m_EditorMode;
        bool              m_UndoAction;
        bool              m_RedoAction;
    };
} // namespace nero
#endif // TOOLBARBUTTONGROUP_H
