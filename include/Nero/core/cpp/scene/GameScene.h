////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMESCENE_H
#define GAMESCENE_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/camera/Camera.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/scene/GameLevel.h>
// SFML
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
// STD
#include <memory>
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GameScene
    {
      public:
        using Ptr = std::shared_ptr<GameScene>;

        // utility
        enum class EngineType
        {
            RENDER,
            EDITOR,
            PREVIEW
        };

        enum class PlatformType
        {
            WINDOWS,
            LINUX,
            MACOS,
            ANDROID,
            IOS
        };

        // scene context
        class Context
        {
          public:
            Context(std::string                        sceneName,
                    std::shared_ptr<sf::RenderTexture> renderTexture,
                    Camera::Ptr                        camera,
                    Setting::Ptr                       setting,
                    EngineType                         engineType,
                    PlatformType                       platformType);

          private:
            friend class GameScene;
            friend class GameProject;
            std::shared_ptr<sf::RenderTexture> renderTexture;
            Camera::Ptr                        camera;
            Setting::Ptr                       setting;
            EngineType                         engineType;
            PlatformType                       platformType;
            std::string                        sceneName;
        };

      public:
        // ctr & dtr
        GameScene(Context context);
        virtual ~GameScene();
        // main method
        virtual void                       init();
        virtual void                       destroy();
        virtual void                       handleEvent(const sf::Event& event);
        virtual void                       update(const sf::Time& timeStep);
        virtual void                       render();
        virtual void                       renderShape();
        virtual void                       renderFrontScreen();

        std::shared_ptr<sf::RenderTexture> getRenderTexture();
        Camera::Ptr                        getCamera();
        void                               setGameLevel(const GameLevel::Ptr& gameLevel);

      private:
        Context                    m_SceneContext;
        std::shared_ptr<GameLevel> m_GameLevel;
    };
} // namespace nero

#endif // GAMESCENE_H
