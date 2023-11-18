////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RENDERCANVASWINDOW_H
#define RENDERCANVASWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class RenderCanvasWindow : public UIComponent
    {
      public:
        using RenderTexturePtr = std::shared_ptr<sf::RenderTexture>;

      public:
        RenderCanvasWindow(EditorContext::Ptr editorContext);
        virtual ~RenderCanvasWindow() override;

        virtual void destroy() override;
        virtual void render() override;
        virtual void update(const sf::Time& timeStep) override;

      private:
        void        updateRenderContext();
        bool        mouseOnCanvas();
        std::string getString(const EditorMode& editorMode);

      private:
        RenderTexturePtr   m_RenderTexture;
        RenderTexturePtr   m_FrontScreenTexture;
        RenderContext::Ptr m_RenderContext;
        EditorCamera::Ptr  m_EditorCamera;
        std::string        m_MousePositionString;
        sf::RectangleShape m_CameraXAxis;
        sf::RectangleShape m_CameraYAxis;
        sf::RectangleShape m_CanvasXAxis;
        sf::RectangleShape m_CanvasYAxis;
        sf::View           m_CanvasFrontView;
        sf::Text           m_GameModeInfo;
        sf::Text           m_GameBuilderInfo;
        // Mouse and Camara Position
        sf::Vector2f       m_WindowPadding;
        float              m_TitleBarHeight;
        sf::Vector2f       m_CanvasPosition;
        sf::Vector2f       m_CanvasSize;
        sf::Vector2f       m_FrontScreenSize;
        sf::Vector2f       m_MousePosition;
        // Rendering Level Builder and Game Level
        AdvancedScene::Ptr m_AdvancedScene;
        LevelBuilder::Ptr  m_LevelBuilder;
        bool               m_RenderLevelBuilder;
        bool               m_RenderAdvancedScene;
        sf::Color          m_ClearColor;
    };
} // namespace nero
#endif // RENDERCANVASWINDOW_H
