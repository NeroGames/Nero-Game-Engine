////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINEEDITOR_H
#define ENGINEEDITOR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/engine/CoreEngine.h>
#include <Nero/core/engine/Setting.h>
#include <Nero/core/camera/AdvancedCamera.h>
#include <Nero/editor/LoadingScreen.h>
#include <Nero/editor/EditorInterface.h>
//STD
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
	class EngineEditor : public CoreEngine
    {
        public:
									EngineEditor();
			virtual                ~EngineEditor() override;

    private:
			//game loop
			void                    handleEvent()                    override;
			void                    update(const sf::Time& timeStep) override;
			void                    render()                         override;
			//startup
			int                     startEngine(bool& engineStarted, const int duration);
			void                    createRenderWindow();
			void					createLoadingScreen();
			void					startBackgroundTask();
			void					loadEarlySetting();
			void					initializeLogging();
			//destroy
			void					destroyEditor();
			//background task
			void					buildDirectory();
			void					loadSetting();
			void					checkEnvironmentVariable();
			void					loadEditorResource();
			void					loadStarterResourcePack();
			void					loadWorkspaceResource();
			void					createEditorInterface();
			void					openLastProject();
			void					createCamera();
			void					checkWorkspace();


         private:
            //Startup Thread
			std::future<int>        m_StartEditorFuture;
			bool                    m_EditorStarted;
            //Startup Screen
            LoadingScreen::Ptr      m_LoadingScreen;
            //Editor Interface
            EditorInterface::Ptr    m_Interface;
            //Resource Manager
			ResourceManager::Ptr    m_ResourceManager;
			//Editor resource
			TextureHolder::Ptr		m_EditorTextureHolder;
			FontHolder::Ptr			m_EditorFontHolder;
			SoundHolder::Ptr		m_EditorSoundHolder;
			//Editor camera
			AdvancedCamera::Ptr     m_AdvancedCamera;
			//
			Setting::Ptr			m_Setting;
    };
}

#endif // ENGINEEDITOR_H
