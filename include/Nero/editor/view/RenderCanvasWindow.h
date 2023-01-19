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

      private:
        void        renderCanvasMenu();
        void        buildRenderContext();
        void        prepareRenderTexture();
        bool        mouseOnCanvas();
        void        renderCamera();
        void        renderGameModeInfo();
        std::string getString(const EditorMode& editorMode);
        sf::Sprite  flipTexture(const sf::Texture& texture);

      private:
        RenderTexturePtr    m_RenderTexture;
        RenderContext::Ptr  m_RenderContext;
        AdvancedCamera::Ptr m_EditorCamera;
        std::string         m_MousePositionString;
        sf::RectangleShape  m_CameraXAxis;
        sf::RectangleShape  m_CameraYAxis;
        sf::RectangleShape  m_CanvasXAxis;
        sf::RectangleShape  m_CanvasYAxis;
        sf::View            m_CanvasFrontView;
        sf::Text            m_GameModeInfo;
        sf::Text            m_GameBuilderInfo;
    };
} // namespace nero
#endif // RENDERCANVASWINDOW_H
