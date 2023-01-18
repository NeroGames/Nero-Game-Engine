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

        m_CanvasXAxis.setSize(sf::Vector2f(100.f, -2.f));
        m_CanvasXAxis.setFillColor(sf::Color::Red);
        m_CanvasXAxis.setOrigin(sf::Vector2f(50.f, -1.f));
        m_CanvasXAxis.setPosition(sf::Vector2f(0.f, 0.f));
        m_CanvasYAxis.setSize(sf::Vector2f(100.f, -2.f));
        m_CanvasYAxis.setOrigin(sf::Vector2f(50.f, -1.f));
        m_CanvasYAxis.setFillColor(sf::Color::Green);
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
            sf::Vector2f world_pos = m_RenderTexture
                                         ->mapPixelToCoords(sf::Vector2i(m_RenderContext->mousePosition.x,
                                                                         m_RenderContext->mousePosition.y),
                                                            m_RenderTexture->getView());

            std::string canvas_pos_string = "Canvas x = " + toString(m_RenderContext->mousePosition.x) + " y = " + toString(m_RenderContext->mousePosition.y);
            std::string wrold_pos_string  = "World x = " + toString(world_pos.x) + " y = " + toString(world_pos.y);
            std::string camera_pos_string = "Camera x = " + toString(m_EditorCamera->getPosition().x) + " y = " + toString(m_EditorCamera->getPosition().y);

            m_MousePositionString         = canvas_pos_string + " | " + wrold_pos_string + " | " + camera_pos_string;
        }
        float start = (ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize(m_MousePositionString.c_str()).x) / 2.f;
        ImGui::SetCursorPosX(start);
        ImGui::Text(m_MousePositionString.c_str());

        prepareRenderTexture();

        auto levelBuilder = m_EditorContext->getLevelBuilder();
        if(levelBuilder)
        {
            levelBuilder->render();
        }

        // Render on Front Screen
        m_RenderTexture->setView(m_RenderTexture->getDefaultView());
        /*if(m_AdvancedScene)
                {
                    m_AdvancedScene->renderFrontScreen(m_EditorMode, m_BuilderMode);
                }*/
        renderCamera();
        renderGameModeInfo();
        m_RenderTexture->setView(m_EditorCamera->getView());

        m_CanvasXAxis.setRotation(m_EditorCamera->getView().getRotation());
        m_CanvasYAxis.setRotation(m_EditorCamera->getView().getRotation() + 90.f);

        m_RenderTexture->draw(m_CanvasXAxis);
        m_RenderTexture->draw(m_CanvasYAxis);

        ImGui::Image(flipTexture(m_RenderTexture->getTexture()));

        renderCanvasMenu();

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
                    m_EditorContext->setEditorMode(EditorMode::WORLD_BUILDER);
                }

                if(ImGui::MenuItem("Screen Builder"))
                {
                    m_EditorContext->setEditorMode(EditorMode::SCREEN_BUILDER);
                }

                if(ImGui::MenuItem("Object Builder"))
                {
                    m_EditorContext->setEditorMode(EditorMode::OBJECT_BUILDER);
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
        float        titleBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
        sf::Vector2f windowPadding  = ImGui::GetStyle().WindowPadding;

        windowSize.y -= 23.f;

        RenderContext renderContext;
        renderContext.canvasPosition = sf::Vector2f(windowPosition.x + windowPadding.x,
                                                    windowPosition.y + titleBarHeight + windowPadding.y + 22.f);
        renderContext.canvasSize     = sf::Vector2f(windowSize.x - windowPadding.x * 2,
                                                    windowSize.y - titleBarHeight - windowPadding.y * 2);
        renderContext.mousePosition  = sf::Vector2f(mousePosition.x - renderContext.canvasPosition.x,
                                                    mousePosition.y - renderContext.canvasPosition.y);
        renderContext.focus          = ImGui::IsWindowFocused();

        if(renderContext.canvasSize.x < 100.f)
        {
            renderContext.canvasSize.x = 100.f;
        }

        if(renderContext.canvasSize.y < 100.f)
        {
            renderContext.canvasSize.y = 100.f;
        }

        *m_RenderContext = renderContext;
    }

    void RenderCanvasWindow::prepareRenderTexture()
    {
        if(m_RenderTexture->getSize().x != m_RenderContext->canvasSize.x ||
           m_RenderTexture->getSize().y != m_RenderContext->canvasSize.y)
        {
            m_RenderTexture->create(m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y);
            m_EditorCamera->updateView(sf::Vector2f(m_RenderContext->canvasSize.x,
                                                    m_RenderContext->canvasSize.y));
        }

        sf::Color clearColor = sf::Color::Black;

        if(m_EditorContext->getEditorMode() == EditorMode::PLAY_GAME)
        {
            /*if(m_AdvancedScene && m_AdvancedScene->getSelectedGameLevel()->levelSetting->getBool("enable_lighting"))
            {
                clearColor = sf::Color::White;
            }*/
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
        case EditorMode::WORLD_BUILDER: {
            const auto builderMode = m_EditorContext->getBuilderMode();

            if(builderMode == BuilderMode::OBJECT)
            {
                return "World Builder - Object";
            }
            else if(builderMode == BuilderMode::MESH)
            {
                return "World Builder - Mesh";
            }
        }
        break;
        case EditorMode::SCREEN_BUILDER:
            return "Screen Builder";
            break;
        case EditorMode::OBJECT_BUILDER:
            return "Object Builder";
            break;
        case EditorMode::PLAY_GAME:
            return "Play Game";
            break;
        case EditorMode::RENDER_GAME:
            return "Render Game";
            break;

        default:
            return StringPool.BLANK;
            break;
        }
    }

    sf::Sprite RenderCanvasWindow::flipTexture(const sf::Texture& texture)
    {
        sf::Vector2u size = texture.getSize();
        sf::Sprite   sprite(texture, sf::IntRect(0, size.y, size.x, -size.y));

        return sprite;
    }
} // namespace nero
