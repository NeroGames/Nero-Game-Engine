////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/ShapeRenderer.h>
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/utility/Keyboard.h>
#include <Nero/core/cpp/model/Collision.h>
#include <Nero/core/cpp/scene/PhysicalWorld.h> //TODO delete
#include <Nero/core/cpp/scene/LightEngine.h>
#include <Nero/core/cpp/scene/ContactListener.h>
#include <Nero/core/cpp/scene/ObjectManager.h>
#include <Nero/core/cpp/camera/Camera.h>
// SFML
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Event.hpp>
// Box2d
#include <Box2D/Dynamics/b2World.h>
// Lighting
#include <ltbl/LightSystem.hpp>
// Std
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GameLevel
    {
      public:
        // utility
        typedef std::shared_ptr<GameLevel> Ptr;

        class Context
        {
          public:
            Context(std::string                        levelName,
                    Setting::Ptr                       levelSetting,
                    std::string                        levelDirectory,
                    std::shared_ptr<sf::RenderTexture> renderTexture,
                    Camera::Ptr                        camera);

            Context();

            std::string                        levelName;
            Setting::Ptr                       levelSetting;
            std::string                        levelDirectory;
            std::shared_ptr<sf::RenderTexture> renderTexture;
            Camera::Ptr                        camera;
            ResourceManager::Ptr               resourceManager;
        };

      public:
        // ctr & dtr
        GameLevel(GameLevel::Context context);
        virtual ~GameLevel();
        // main
        virtual void init();
        virtual void destroy();
        virtual void handleEvent(const sf::Event& event);
        virtual void update(const sf::Time& timeStep);
        virtual void render();
        virtual void renderShape();
        virtual void renderLight();
        // builder
        void         loadLevel();
        void         loadChunk();
        void         loadLayer();
        void         loadObject();
        // input
        // window
        virtual void onWindowClosed();
        virtual void onWindowResized(const sf::Vector2u& size);
        virtual void onWindowFocusChanged(const bool& gainedFocus);
        // keyboard
        virtual void onKeyboardButton(const sf::Keyboard::Key& key,
                                      const bool&              isPressed,
                                      const ModifierKey&       modifier);
        virtual void onTextEntered(const std::string& c);
        // mouse
        virtual void onMouseMoved(const sf::Vector2f& position);
        virtual void onMouseButton(const sf::Mouse::Button& button,
                                   const bool&              isPressed,
                                   const sf::Vector2f&      position);
        virtual void onMouseWheel(const sf::Mouse::Wheel& wheel,
                                  const float&            delta,
                                  const sf::Vector2f&     position);
        virtual void onMouseWindowSurface(const bool& mouseEntered);
        // joystick
        virtual void onJoystickConnection(const unsigned int& joystickId, const bool& connected);
        virtual void onJoystickButton(const unsigned int& joystickId,
                                      const unsigned int& button,
                                      const bool&         isPressed);
        virtual void onJoystickAxis(const unsigned int&       joystickId,
                                    const sf::Joystick::Axis& axis,
                                    const float&              position);
        // collision
        virtual void onCollisionContactBegin(Collision collision);
        virtual void onCollisionContactEnd(Collision collision);
        virtual void onCollisionPreSolveContact(Collision collision);
        virtual void onCollisionPostSolveContact(Collision collision);
        //
        ResourceManager::Ptr               getResourceManager() const;
        Setting::Ptr                       getSetting() const;
        std::string                        getLevelDirectory() const;

        std::shared_ptr<sf::RenderTexture> getRenderTexture();
        Camera::Ptr                        getCamera();
        Object::Ptr                        getLevelRoot() const;
        std::shared_ptr<ltbl::LightSystem> getLightManager() const;
        std::shared_ptr<b2World>           getPhysicsWorld() const;
        ShapeRenderer::Ptr                 getShapeRenderer() const;

      private:
        std::shared_ptr<sf::RenderTexture> m_RenderTexture;
        Context                            m_LevelContext;
        ResourceManager::Ptr               m_ResourceManager;
        PhysicalWorld::Ptr                 m_PhysicalWorld;
        // LightEngine::Ptr                   m_LightEngine;
        Object::Ptr                        m_LevelRoot;
        std::shared_ptr<ltbl::LightSystem> m_LightManager;
        std::shared_ptr<b2World>           m_PhysicsWorld;
        ShapeRenderer::Ptr                 m_ShapeRenderer;
        ObjectManager::Ptr                 m_ObjectManager;
        ContactListener::Ptr               m_ContactListener;
        std::vector<Screen::Ptr>           m_ScreenTable;
    };
} // namespace nero

#endif // GAMELEVEL_H
