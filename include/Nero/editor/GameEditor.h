////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMEEDITOR_H
#define GAMEEDITOR_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/engine/CoreEngine.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/editor/LoadingScreen.h>
#include <Nero/editor/EditorCamera.h>
#include <Nero/editor/EditorUI.h>
// Cpp
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameEditor : public CoreEngine
    {
      public:
        GameEditor();
        virtual ~GameEditor() override;

      private:
        // Clean up
        void destroy();
        // Game loop
        void handleEvent() override;
        void update(const sf::Time& timeStep) override;
        void render() override;
        // Startup
        void initLogging();
        void initSetting();
        void createRenderWindow();
        void createLoadingScreen();
        void backgroundStartup();
        int  startEngine(bool& editorStarted, const int duration);
        // Background startup
        void buildDirectory();
        void loadSetting();
        void checkEnvironmentVariable();
        void loadEditorResource();
        void createEditorInterface();
        void openLastProject();
        void createCamera();
        void checkWorkspace();
        void checkRecentProject();

      private:
        // Background startup
        std::future<int>   m_StartupFuture;
        bool               m_EditorStarted;
        // Loading screen
        LoadingScreen::Ptr m_LoadingScreen;
        // Editor interface
        EditorUI::Ptr      m_EditorUI;
        // Advanced camera
        EditorCamera::Ptr  m_EditorCamera;
        // editor settings
        Setting::Ptr       m_EditorSetting;
        // editor resource
        TextureHolder::Ptr m_EditorTextureHolder;
        FontHolder::Ptr    m_EditorFontHolder;
        SoundHolder::Ptr   m_EditorSoundHolder;
    };
} // namespace nero

#endif // GAMEEDITOR_H
