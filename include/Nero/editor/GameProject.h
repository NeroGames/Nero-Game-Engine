////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMEPROJECT_H
#define GAMEPROJECT_H
///////////////////////////HEADERS///////////////////////////
//STD
#include <memory>
#include <json/json.hpp>
#include <Nero/editor/AdvancedScene.h>
#include <boost/function.hpp>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/cpp/luascene/LuaScene.h>
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/editor/AdvancedCamera.h>

/////////////////////////////////////////////////////////////


namespace nero
{
    class GameProject
    {
        public:
            typedef std::shared_ptr<GameProject> Ptr;
			typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;

            typedef Scene::Ptr (CreateCppSceneFn)(Scene::Context);
			//typedef LuaScene::Ptr (CreateLuaSceneFn)(Scene::Context);

        public:
												GameProject();

			void								init(const Setting& parameter);
			void								loadProject();
			void								saveProject();
			void								loadProjectLibrary();
			void								openEditor();
			void								compileProject();
			std::string							exec(const char* cmd);
			AdvancedScene::Ptr					getAdvancedScene();
			void								setRenderTexture(const RenderTexturePtr& renderTexture);
			void								setResourceManager(const ResourceManager::Ptr& resourceManager);
			void								setCamera(const Camera::Ptr& camera);
			void								openQtCreator(const std::string& file = StringPool.BLANK);
			void								openVisualStudio(const std::string& file = StringPool.BLANK);
			void								setRenderContext(const RenderContextPtr& renderContext);
			void								setSetting(const Setting::Ptr& setting);
			void								close();
			AdvancedScene::GameLevelPtr			loadGameLevel(const nlohmann::json& level);
			std::string							getProjectName() const;
			std::vector<BackgroundTask>&		getBackgroundTaskTable();

		private:
			BackgroundTask&						createBackgroundTask(const std::string& name, const std::string& category);

		private:
			std::vector<BackgroundTask>			m_BackgroundTaskTable;
			Setting								m_ProjectParameter;
			RenderTexturePtr					m_RenderTexture;
			Camera::Ptr							m_Camera;
			ResourceManager::Ptr				m_ResourceManager;
			Setting::Ptr						m_EngineSetting;
			RenderContextPtr					m_RenderContext;
			AdvancedScene::Ptr					m_AdvancedScene;
			Scene::Ptr							m_Scene;
			std::string							m_EditorProcessId;
			boost::function<CreateCppSceneFn>	m_CreateCppSceneFn;
			//boost::function<CreateLuaSceneFn> m_CreateLuaSceneFn;

    };
}


#endif // GAMEPROJECT_H
