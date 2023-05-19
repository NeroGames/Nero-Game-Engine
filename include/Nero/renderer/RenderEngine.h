////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef RENDERENGINE_H
#define RENDERENGINE_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/engine/CoreEngine.h>
#include <Nero/core/cpp/engine/StartupScreen.h>
#include <Nero/core/cpp/object/GameLevelScriptObject.h>
#include <Nero/core/cpp/object/GameScreenScriptObject.h>
// #include <Nero/core/cpp/object/SimpleScriptObject.h>
// #include <Nero/core/cpp/object/PhysicScriptObject.h>
// SFML
#include <SFML/Window/WindowStyle.hpp>
// Std
#include <future>
// #include <map>
/////////////////////////////////////////////////////////////
namespace nero
{
    /*class RenderEngine : public CoreEngine
    {
      public:
        typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;
        // Load Class
        typedef Scene::Ptr(CreateCppScene)(Scene::Context);
        typedef StartupScreen::Ptr(CreateCppStartupScreen)();

      public:
        RenderEngine();
        virtual ~RenderEngine() override;

      private:
        bool       checkDirectory();
        //
        void       noGameFound();
        //
        void       loadGame();
        void       createRenderWindow();
        void       loadStartupScreen();
        void       startEngineInBackground();
        int        startEngine(bool& engineStarted, const unsigned int duration);
        // game loop
        void       handleEvent() override;
        void       update(const sf::Time& timeStep) override;
        void       render() override;

        // uitliy
        sf::Uint32 getWindowStyle(const std::string& style);

      private:
        // Game Scene
        Scene::Ptr                              m_GameScene;
        // Startup Screen
        StartupScreen::Ptr                      m_StartupScreen;
        std::future<int>                        m_StartEngineFuture;
        bool                                    m_EngineStarted;
        // Resource Manager
        ResourceManager::Ptr                    m_GameWorldResourceManager;
        ResourceManager::Ptr                    m_StartupScreenResourceManager;
        ResourceManager::Ptr                    m_GameScreenResourceManager;
        // Camera
        Camera::Ptr                             m_Camera;
        // Setting
        Setting::Ptr                            m_GameSetting;
        // Render Texture
        RenderTexturePtr                        m_RenderTexture;
        // Load Cpp Class
        boost::function<CreateCppScene>         m_CreateCppScene;
        boost::function<CreateCppStartupScreen> m_CreateCppStartupScreen;
    };*/

} // namespace nero

#endif // RENDERENGINE_H
