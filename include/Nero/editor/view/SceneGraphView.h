////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef SCENEGRAPHVIEW_H
#define SCENEGRAPHVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class SceneGraphView : public UIComponent
    {
      public:
        SceneGraphView(EditorContext::Ptr editorContext);
        virtual ~SceneGraphView() override;

        virtual void destroy() override;
        virtual void render() override;
        virtual void update(const sf::Time&) override;

      private:
        void renderSceneGraph();
        void renderLevelProperty();
        void renderWorldChunkProperty();

      private:
        ImVec4     m_LightingAmbientColor;
        ImVec4     m_LightingClearColor;
        EditorMode m_EditorMode;
    };
} // namespace nero
#endif // SCENEGRAPHVIEW_H
