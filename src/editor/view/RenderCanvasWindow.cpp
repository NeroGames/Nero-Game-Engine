////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/RenderCanvasWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    RenderCanvasWindow::RenderCanvasWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_RenderTexture(m_EditorContext->getRenderTexture())
        , m_RenderContext(m_EditorContext->getRenderContext())
        , m_EditorCamera(m_EditorContext->getEditorCamera())
        , m_MousePositionString("Mouse Position")
    {
        m_CameraXAxis.setSize(sf::Vector2f(20.f, -2.f));
        m_CameraXAxis.setFillColor(sf::Color::Red);
        m_CameraXAxis.setPosition(sf::Vector2f(20.f, 20.f));

        m_CameraYAxis.setSize(sf::Vector2f(20.f, -2.f));
        m_CameraYAxis.setFillColor(sf::Color::Green);
        m_CameraYAxis.setPosition(sf::Vector2f(20.f, 20.f));
        m_CameraYAxis.setRotation(90.f);

        m_CanvasXAxis.setSize(sf::Vector2f(20.f, 2.f));
        m_CanvasXAxis.setOrigin(sf::Vector2f(10.f, 1.f));
        m_CanvasXAxis.setFillColor(sf::Color(255, 0, 0, 100));
        m_CanvasXAxis.setPosition(sf::Vector2f(0.f, 0.f));
        m_CanvasYAxis.setSize(sf::Vector2f(20.f, 2.f));
        m_CanvasYAxis.setOrigin(sf::Vector2f(10.f, 1.f));
        m_CanvasYAxis.setFillColor(sf::Color(0, 255, 0, 100));
        m_CanvasYAxis.setPosition(sf::Vector2f(0.f, 0.f));
        m_CanvasYAxis.setRotation(90.f);

        m_GameModeInfo.setFont(m_EditorContext->getFontHolder()->getDefaultFont());
        m_GameModeInfo.setCharacterSize(18.f);
        m_GameModeInfo.setFillColor(sf::Color::White);

        m_GameBuilderInfo.setFont(m_EditorContext->getFontHolder()->getDefaultFont());
        m_GameBuilderInfo.setCharacterSize(18.f);
        m_GameBuilderInfo.setFillColor(sf::Color::White);
    }

    RenderCanvasWindow::~RenderCanvasWindow()
    {
        destroy();
    }

    void RenderCanvasWindow::destroy()
    {
    }

    void RenderCanvasWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

        buildRenderContext();

        ImGui::SameLine();

        if(mouseOnCanvas() && ImGui::IsWindowFocused())
        {
            sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
                sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
                m_RenderTexture->getView());

            std::string canvas_pos_string =
                "Canvas x = " + toString(m_RenderContext->mousePosition.x) +
                " y = " + toString(m_RenderContext->mousePosition.y);
            std::string wrold_pos_string =
                "World x = " + toString(world_pos.x) + " y = " + toString(world_pos.y);
            std::string camera_pos_string =
                "Camera x = " + toString(m_EditorCamera->getPosition().x) +
                " y = " + toString(m_EditorCamera->getPosition().y);

            m_MousePositionString =
                canvas_pos_string + " | " + wrold_pos_string + " | " + camera_pos_string;
        }
        float start = (ImGui::GetWindowContentRegionWidth() -
                       ImGui::CalcTextSize(m_MousePositionString.c_str()).x) /
                      2.f;
        ImGui::SetCursorPosX(start);
        ImGui::Text(m_MousePositionString.c_str());

        prepareRenderTexture();

        const auto editorMode = m_EditorContext->getEditorMode();
        if(editorMode == EditorMode::World_Builder)
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();
            if(levelBuilder)
            {
                levelBuilder->render();
            }
        }
        else if(editorMode == EditorMode::Play_Game)
        {
            auto advancedScene = m_EditorContext->getAdvancedScene();
            if(advancedScene)
            {
                advancedScene->render();
                advancedScene->renderShape();
            }
        }

        // Render on Front Screen
        m_RenderTexture->setView(m_RenderTexture->getDefaultView());
        // TODO render Game Screen here
        renderCamera();
        renderGameModeInfo();
        m_RenderTexture->setView(m_EditorCamera->getView());

        m_RenderTexture->draw(m_CanvasXAxis);
        m_RenderTexture->draw(m_CanvasYAxis);

        ImGui::Image(flipTexture(m_RenderTexture->getTexture()));

        if(m_EditorContext->getBuilderMode() != BuilderMode::Mesh)
        {
            renderCanvasMenu();
        }

        ImGui::End();
    }

    void RenderCanvasWindow::renderCanvasMenu()
    {
        if(ImGui::BeginPopupCanvasWindow())
        {
            if(ImGui::BeginMenu("Editor Mode"))
            {
                // ImGui::MenuItem("(choose editor mode)", NULL, false, false);

                if(ImGui::MenuItem("World Builder"))
                {
                    m_EditorContext->setEditorMode(EditorMode::World_Builder);
                }

                if(ImGui::MenuItem("Screen Builder"))
                {
                    m_EditorContext->setEditorMode(EditorMode::Screen_Builder);
                }

                if(ImGui::MenuItem("Object Builder"))
                {
                    m_EditorContext->setEditorMode(EditorMode::Factory);
                }

                ImGui::EndMenu();
            }

            ImGui::Separator();

            if(ImGui::BeginMenu("Website"))
            {
                // ImGui::MenuItem("(useful links)", NULL, false, false);

                if(ImGui::MenuItem("Learn"))
                {
                }

                if(ImGui::MenuItem("Forum"))
                {
                }

                if(ImGui::MenuItem("Code Snippet"))
                {
                }

                if(ImGui::MenuItem("Engine API"))
                {
                }

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }
    }

    void RenderCanvasWindow::buildRenderContext()
    {
        sf::Vector2f windowPosition = ImGui::GetWindowPos();
        sf::Vector2f windowSize     = ImGui::GetWindowSize();
        sf::Vector2f mousePosition  = ImGui::GetMousePos();
        sf::Vector2f windowPadding  = ImGui::GetStyle().WindowPadding;
        float titleBarHeight = ImGui::GetFontSize() * 2 + ImGui::GetStyle().FramePadding.y * 4;

        RenderContext renderContext;
        renderContext.canvasPosition =
            sf::Vector2f(windowPosition.x + windowPadding.x,
                         windowPosition.y + windowPadding.y + titleBarHeight);

        renderContext.canvasSize =
            sf::Vector2f(windowSize.x - windowPadding.x * 2,
                         windowSize.y - windowPadding.y * 2 - titleBarHeight);

        if(renderContext.canvasSize.x < 100.f)
            renderContext.canvasSize.x = 100.f;

        if(renderContext.canvasSize.y < 100.f)
            renderContext.canvasSize.y = 100.f;

        renderContext.mousePosition =
            sf::Vector2f(mousePosition.x - renderContext.canvasPosition.x,
                         mousePosition.y - renderContext.canvasPosition.y);

        renderContext.focus = ImGui::IsWindowFocused();

        *m_RenderContext    = renderContext;
    }

    void RenderCanvasWindow::prepareRenderTexture()
    {
        if(m_RenderTexture->getSize().x != m_RenderContext->canvasSize.x ||
           m_RenderTexture->getSize().y != m_RenderContext->canvasSize.y)
        {
            m_RenderTexture->create(m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y);
            m_EditorCamera->updateView(
                sf::Vector2f(m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y));
        }

        sf::Color clearColor = sf::Color::Black;

        if(m_EditorContext->getEditorMode() == EditorMode::Play_Game)
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder && levelBuilder->getLevelSetting()->getBool("enable_light"))
            {
                clearColor =
                    levelBuilder->getLevelSetting()->getSetting("lighting").getColor("clear_color");
            }
        }

        m_RenderTexture->clear(clearColor);
        m_RenderTexture->setView(m_EditorCamera->getView());
    }

    bool RenderCanvasWindow::mouseOnCanvas()
    {
        sf::Rect<float> canvas(m_RenderContext->canvasPosition.x,
                               m_RenderContext->canvasPosition.y,
                               m_RenderContext->canvasSize.x,
                               m_RenderContext->canvasSize.y);

        sf::Vector2i    mousePosition = ImGui::GetMousePos();

        return canvas.contains(mousePosition.x, mousePosition.y);
    }

    void RenderCanvasWindow::renderGameModeInfo()
    {
        std::string gameMode  = getString(m_EditorContext->getEditorMode());
        std::string frameRate = toString(m_EditorContext->getFrameRate()) + " fps";
        std::string frameTime = toString(m_EditorContext->getFrameTime() * 1000.f) + " ms";

        std::string info      = gameMode + "  |  " + frameRate + "  |  " + frameTime;

        m_GameModeInfo.setString(info);
        m_GameBuilderInfo.setString(m_EditorContext->getOpengedGameLevelName());

        sf::Vector2f position;
        position.x = m_RenderTexture->getSize().x - m_GameModeInfo.getLocalBounds().width - 20.f;
        position.y = m_RenderTexture->getSize().y - m_GameModeInfo.getLocalBounds().height - 20.f;

        m_GameModeInfo.setPosition(position);

        m_GameBuilderInfo.setPosition(sf::Vector2f(20.f, position.y));

        m_RenderTexture->draw(m_GameModeInfo);
        m_RenderTexture->draw(m_GameBuilderInfo);
    }

    void RenderCanvasWindow::renderCamera()
    {
        m_CameraXAxis.setRotation(m_EditorCamera->getView().getRotation());
        m_CameraYAxis.setRotation(m_EditorCamera->getView().getRotation() + 90.f);

        m_RenderTexture->draw(m_CameraXAxis);
        m_RenderTexture->draw(m_CameraYAxis);
    }

    std::string RenderCanvasWindow::getString(const EditorMode& editorMode)
    {
        switch(editorMode)
        {
            case EditorMode::World_Builder:
            {
                const auto builderMode = m_EditorContext->getBuilderMode();

                if(builderMode == BuilderMode::Object)
                {
                    return "World Builder - Object";
                }
                else if(builderMode == BuilderMode::Mesh)
                {
                    return "World Builder - Mesh";
                }
                else if(builderMode == BuilderMode::Joint)
                {
                    return "World Builder - Joint";
                }

                return "World Builder";
            }

            case EditorMode::Screen_Builder:
                return "Screen Builder - Object";

            case EditorMode::Factory:
            {
                const auto builderMode = m_EditorContext->getBuilderMode();

                if(builderMode == BuilderMode::Object)
                {
                    return "Factory - Object";
                }
                else if(builderMode == BuilderMode::Mesh)
                {
                    return "Factory - Mesh";
                }
                else if(builderMode == BuilderMode::Joint)
                {
                    return "Factory - Joint";
                }

                return "Factory";
            }

            case EditorMode::Play_Game:
                return "Play Game";

            case EditorMode::Render_Game:
                return "Render Game";

            default:
                return "Game Editor";
        }
    }

    sf::Sprite RenderCanvasWindow::flipTexture(const sf::Texture& texture)
    {
        sf::Vector2u size = texture.getSize();
        sf::Sprite   sprite(texture, sf::IntRect(0, size.y, size.x, -size.y));

        return sprite;
    }
} // namespace nero
