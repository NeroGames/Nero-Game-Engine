////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef SCENEEXPLORERWINDOW_H
#define SCENEEXPLORERWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/view/SceneGraphView.h>
#include <Nero/editor/view/GameObjectPropertyView.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class SceneExplorerWindow : public UIComponent
    {
      public:
        SceneExplorerWindow(EditorContext::Ptr editorContext);
        virtual ~SceneExplorerWindow() override;

        virtual void destroy() override;
        virtual void render() override;
        virtual void update(const sf::Time& timeStep) override;

      private:
        SceneGraphView         m_SceneGraphView;
        GameObjectPropertyView m_GameObjectPropertyView;
    };
} // namespace nero
#endif // SCENEEXPLORERWINDOW_H
