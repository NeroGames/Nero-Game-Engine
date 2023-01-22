////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
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

    GameScene::GameScene(GameScene::Context context)
        : m_SceneContext(context)
        , m_GameLevel(nullptr)
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
    }

    void GameScene::update(const sf::Time& timeStep)
    {
    }

    void GameScene::render()
    {
        if(m_GameLevel)
            m_GameLevel->render();
    }

    void GameScene::renderShape()
    {
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
    }
} // namespace nero
