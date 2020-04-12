////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINEEDITOR_H
#define ENGINEEDITOR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/engine/CoreEngine.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/editor/AdvancedCamera.h>
#include <Nero/editor/LoadingScreen.h>
#include <Nero/editor/EditorInterface.h>
//Cpp
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
	class EngineEditor : public CoreEngine
    {
        public:
									EngineEditor();
			virtual                ~EngineEditor() override;
			void					destroy();

		private:
			//game loop
			void                    handleEvent()                    override;
			void                    update(const sf::Time& timeStep) override;
			void                    render()                         override;
			//startup
			void					initLogging();
			void					initSetting();
			void                    createRenderWindow();
			void					createLoadingScreen();
			void					backgroundStartup();
			int                     startEngine(bool& engineStarted, const int duration);
			//background startup
			void					buildDirectory();
			void					loadSetting();
			void					checkEnvironmentVariable();
			void					loadEditorResource();
			void					createEditorInterface();
			void					openLastProject();
			void					createCamera();
			void					checkWorkspace();
			void					checkRecentProject();

         private:
			//background startup
			std::future<int>        m_StartupFuture;
			bool                    m_EditorStarted;
			//loading screen
            LoadingScreen::Ptr      m_LoadingScreen;
			//editor interface
            EditorInterface::Ptr    m_Interface;
			//advanced camera
			AdvancedCamera::Ptr     m_EditorCamera;
			//editor settings
			Setting::Ptr			m_EditorSetting;
			//editor resource
			TextureHolder::Ptr		m_EditorTextureHolder;
			FontHolder::Ptr			m_EditorFontHolder;
			SoundHolder::Ptr		m_EditorSoundHolder;
	};
}

#endif // ENGINEEDITOR_H
