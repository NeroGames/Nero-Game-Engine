////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include <Nero/core/cpp/scene/GameScene.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    GameScene::Context::Context(std::string sceneName, std::shared_ptr<sf::RenderTexture> renderTexture, Camera::Ptr camera, Setting::Ptr setting, EngineType engineType, PlatformType platformType)
        : sceneName(sceneName)
        , renderTexture(renderTexture)
        , camera(camera)
        , setting(setting)
        , engineType(engineType)
        , platformType(platformType)
    {
    }

    GameScene::Context::Context()
    {
        // empty
    }

    GameScene::GameScene(GameScene::Context context)
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
    }

    void GameScene::renderShape()
    {
    }

    void GameScene::renderFrontScreen()
    {
    }
} // namespace nero
