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
        , m_FrontScreenTexture(std::make_shared<sf::RenderTexture>())
        , m_RenderContext(m_EditorContext->getRenderContext())
        , m_EditorCamera(m_EditorContext->getEditorCamera())
        , m_MousePositionString("Mouse Position")
        , m_AdvancedScene(nullptr)
        , m_LevelBuilder(nullptr)
        , m_RenderLevelBuilder(false)
        , m_RenderAdvancedScene(false)
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

    void RenderCanvasWindow::update(const sf::Time&)
    {
        updateRenderContext();

        if(mouseOnCanvas())
        {
            sf::Vector2f worldPosition = m_RenderTexture->mapPixelToCoords(
                sf::Vector2i(int(m_RenderContext->mousePosition.x),
                             int(m_RenderContext->mousePosition.y)),
                m_RenderTexture->getView());

            std::string canvasPositionString =
                "Canvas x = " + toString(m_RenderContext->mousePosition.x) +
                " y = " + toString(m_RenderContext->mousePosition.y);

            std::string worldPositionString =
                "World x = " + toString(worldPosition.x) + " y = " + toString(worldPosition.y);

            std::string cameraPositionString =
                "Camera x = " + toString(m_EditorCamera->getPosition().x) +
                " y = " + toString(m_EditorCamera->getPosition().y);

            m_MousePositionString =
                canvasPositionString + " | " + worldPositionString + " | " + cameraPositionString;
        }

        const auto editorMode = m_EditorContext->getEditorMode();
        m_LevelBuilder        = m_EditorContext->getLevelBuilder();
        m_AdvancedScene       = m_EditorContext->getAdvancedScene();
        m_RenderLevelBuilder  = (editorMode == EditorMode::World_Builder) && m_LevelBuilder;
        m_RenderAdvancedScene = (editorMode == EditorMode::Play_Game) && m_AdvancedScene;

        m_CameraXAxis.setRotation(m_EditorCamera->getView().getRotation());
        m_CameraYAxis.setRotation(m_EditorCamera->getView().getRotation() + 90.f);

        std::string gameMode  = getString(m_EditorContext->getEditorMode());
        std::string frameRate = toString(m_EditorContext->getFrameRate()) + " fps";
        std::string frameTime = toString(m_EditorContext->getFrameTime() * 1000.f) + " ms";

        std::string info      = gameMode + "  |  " + frameRate + "  |  " + frameTime;

        m_GameModeInfo.setString(info);
        m_GameBuilderInfo.setString(m_EditorContext->getOpengedGameLevelName());

        sf::Vector2f position;
        position.x = float(m_FrontScreenSize.x) - m_GameModeInfo.getLocalBounds().width - 20.f;
        position.y = float(m_FrontScreenSize.y) - m_GameModeInfo.getLocalBounds().height - 20.f;

        m_GameModeInfo.setPosition(position);
        m_GameBuilderInfo.setPosition(sf::Vector2f(20.f, position.y));

        if(m_RenderTexture->getSize().x != m_RenderContext->canvasSize.x ||
           m_RenderTexture->getSize().y != m_RenderContext->canvasSize.y)
        {
            m_RenderTexture->create(m_RenderContext->canvasSize.x, m_RenderContext->canvasSize.y);
            m_FrontScreenTexture->create(m_FrontScreenSize.x, m_FrontScreenSize.y);
            m_EditorCamera->updateView(sf::Vector2f(float(m_RenderContext->canvasSize.x),
                                                    float(m_RenderContext->canvasSize.y)));
        }

        m_ClearColor = sf::Color::Black;

        if(editorMode == EditorMode::Play_Game && m_LevelBuilder)
        {
            auto levelSetting = m_LevelBuilder->getLevelSetting();

            if(levelSetting->getBool("enable_light"))
            {
                m_ClearColor = levelSetting->getSetting("lighting").getColor("clear_color");
            }
        }
    }

    void RenderCanvasWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_GAME_SCENE.c_str());

        // Get current window and mouse position
        m_CanvasPosition               = ImGui::GetWindowPos();
        m_CanvasSize                   = ImGui::GetWindowSize();
        m_MousePosition                = ImGui::GetMousePos();
        m_RenderContext->canvasOnFocus = ImGui::IsWindowFocused();

        ImGui::SameLine();

        const float textPosition = (ImGui::GetWindowContentRegionWidth() -
                                    ImGui::CalcTextSize(m_MousePositionString.c_str()).x) /
                                   2.f;
        ImGui::SetCursorPosX(textPosition);
        ImGui::Text("%s", m_MousePositionString.c_str());

        m_RenderTexture->clear(m_ClearColor);
        m_RenderTexture->setView(m_EditorCamera->getView());
        m_FrontScreenTexture->clear(sf::Color::Transparent);

        if(m_RenderLevelBuilder)
        {
            m_LevelBuilder->render();
        }
        else if(m_RenderAdvancedScene)
        {
            m_AdvancedScene->render();
            m_AdvancedScene->renderShape();
        }

        m_RenderTexture->draw(m_CanvasXAxis);
        m_RenderTexture->draw(m_CanvasYAxis);

        // Front Screen
        m_RenderTexture->setView(m_RenderTexture->getDefaultView());
        if(m_RenderAdvancedScene)
        {
            m_AdvancedScene->renderFrontScreen();
        }
        m_RenderTexture->setView(m_EditorCamera->getView());

        m_FrontScreenTexture->draw(m_CameraXAxis);
        m_FrontScreenTexture->draw(m_CameraYAxis);
        m_FrontScreenTexture->draw(m_GameModeInfo);
        m_FrontScreenTexture->draw(m_GameBuilderInfo);

        const auto imagePosition = ImGui::GetCursorPos();
        ImGui::Image(*m_RenderTexture, m_RenderContext->textureFactor);
        ImGui::SetCursorPos(imagePosition);
        ImGui::Image(*m_FrontScreenTexture);

        ImGui::End();
    }

    void RenderCanvasWindow::updateRenderContext()
    {
        m_WindowPadding  = ImGui::GetStyle().WindowPadding;
        m_TitleBarHeight = ImGui::GetFontSize() * 2 + ImGui::GetStyle().FramePadding.y * 4;

        m_RenderContext->textureFactor = 2.5f;

        m_RenderContext->canvasPosition =
            sf::Vector2f(m_CanvasPosition.x + m_WindowPadding.x,
                         m_CanvasPosition.y + m_WindowPadding.y + m_TitleBarHeight);

        auto textureSize = sf::Vector2f(m_CanvasSize.x - m_WindowPadding.x * 2,
                                        m_CanvasSize.y - m_WindowPadding.y * 2 - m_TitleBarHeight);

        if(textureSize.x < 100.f)
            textureSize.x = 100.f;

        if(textureSize.y < 100.f)
            textureSize.y = 100.f;

        m_FrontScreenSize           = textureSize;

        textureSize.x               /= m_RenderContext->textureFactor;
        textureSize.y               /= m_RenderContext->textureFactor;

        m_RenderContext->canvasSize = sf::Vector2u(textureSize.x, textureSize.y);

        auto mosuePosition = sf::Vector2f(m_MousePosition.x - m_RenderContext->canvasPosition.x,
                                          m_MousePosition.y - m_RenderContext->canvasPosition.y);

        mosuePosition.x    /= m_RenderContext->textureFactor;
        mosuePosition.y    /= m_RenderContext->textureFactor;
        m_RenderContext->mousePosition = mosuePosition;
    }

    bool RenderCanvasWindow::mouseOnCanvas()
    {
        sf::Rect<float> canvas(
            m_RenderContext->canvasPosition.x,
            m_RenderContext->canvasPosition.y,
            float(m_RenderContext->canvasSize.x) * m_RenderContext->textureFactor,
            float(m_RenderContext->canvasSize.y) * m_RenderContext->textureFactor);

        sf::Vector2i mousePosition = ImGui::GetMousePos();

        return canvas.contains(float(mousePosition.x), float(mousePosition.y));
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
} // namespace nero
