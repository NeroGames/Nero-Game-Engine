////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include <Nero/core/cpp/scene/GameScene.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    GameScene::Context::Context(std::string                        sceneName,
                                std::shared_ptr<sf::RenderTexture> renderTexture,
                                Camera::Ptr                        camera,
                                Setting::Ptr                       setting,
                                EngineType                         engineType,
                                PlatformType                       platformType)
        : renderTexture(renderTexture)
        , camera(camera)
        , setting(setting)
        , engineType(engineType)
        , platformType(platformType)
        , sceneName(sceneName)
    {
    }

    GameScene::Context::Context()
    {
    }

    GameScene::GameScene(GameScene::Context context)
        : m_SceneContext(context)
        , m_GameLevel(nullptr)
        , m_SceneRoot(std::make_shared<Object>())

    {
    }

    GameScene::~GameScene()
    {
        destroy();
    }

    void GameScene::init()
    {
    }

    void GameScene::destroy()
    {
    }

    void GameScene::handleEvent(const sf::Event& event)
    {
        if(m_GameLevel)
            m_GameLevel->handleEvent(event);
    }

    void GameScene::update(const sf::Time& timeStep)
    {
        if(m_GameLevel)
        {
            m_GameLevel->update(timeStep);
        }
    }

    void GameScene::render()
    {
        m_SceneContext.renderTexture->draw(*m_SceneRoot);

        if(m_GameLevel)
        {
            m_GameLevel->render();
            m_GameLevel->renderLight();
        }
    }

    void GameScene::renderShape()
    {
        if(m_GameLevel)
        {
            m_GameLevel->renderShape();
        }
    }

    void GameScene::renderFrontScreen()
    {
    }

    std::shared_ptr<sf::RenderTexture> GameScene::getRenderTexture()
    {
        return m_SceneContext.renderTexture;
    }

    Camera::Ptr GameScene::getCamera()
    {
        return m_SceneContext.camera;
    }

    void GameScene::setGameLevel(const GameLevel::Ptr& gameLevel)
    {
        m_GameLevel = gameLevel;
        m_SceneRoot->addChild(m_GameLevel->getLevelRoot());
    }

    void GameScene::notifyUpdate(const std::string& component, const std::string& update)
    {
        if(component == "game_level")
        {
            if(m_GameLevel)
                m_GameLevel->notifyUpdate(update);
        }
    }

} // namespace nero
