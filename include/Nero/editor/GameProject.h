////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMEPROJECT_H
#define GAMEPROJECT_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/editor/AdvancedScene.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/lua/scene/LuaScene.h>
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/editor/AdvancedCamera.h>
//Json
#include <json/json.hpp>
//Boost
#include <boost/function.hpp>
//STD
#include <memory>
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

			void								init(const Parameter& parameter);
			void								loadProject();
			void								saveProject();
			void								saveGameLevel();
			void								loadGameLevel();
			//void								saveGameScreen();
			void								loadGameScreen();
			//void								loadLibrary();
			void								openEditor();
			void								compileProject(const BackgroundTask::Ptr backgroundTask);
			void								render();
			void								destroyScene();
			void								handleEvent(const sf::Event& event);
			void								update(const sf::Time& timestep);

			AdvancedScene::Ptr					getAdvancedScene();
			void								setRenderTexture(const RenderTexturePtr& renderTexture);
			void								setCamera(const Camera::Ptr& camera);
			void								openQtCreator(const std::string& file = StringPool.BLANK);
			void								openVisualStudio(const std::string& file = StringPool.BLANK);
			void								setRenderContext(const RenderContext::Ptr& renderContext);
			void								setSetting(const Setting::Ptr& setting);
			void								close();
			//AdvancedScene::GameLevelPtr			loadGameLevel(const nlohmann::json& level);
			std::string							getProjectName() const;
			std::vector<BackgroundTask>&		getBackgroundTaskTable();
			void								createScriptObject(const Parameter& parameter);
			ResourceManager::Ptr				getResourceManager();
			void								loadResource(const Parameter& parameter);

			const std::string					getResourceFoler() const;


		private:
			BackgroundTask&						createBackgroundTask(const std::string& name, const std::string& category);

		private:
			Scene::Ptr							m_Scene;
			std::vector<BackgroundTask>			m_BackgroundTaskTable;
			Parameter							m_ProjectParameter;
			RenderTexturePtr					m_RenderTexture;
			Camera::Ptr							m_Camera;
			ResourceManager::Ptr				m_ResourceManager;
			Setting::Ptr						m_EngineSetting;
			RenderContext::Ptr					m_RenderContext;
			AdvancedScene::Ptr					m_AdvancedScene;
			std::string							m_EditorProcessId;
			boost::function<CreateCppSceneFn>	m_CreateCppSceneFn;
			//boost::function<CreateLuaSceneFn> m_CreateLuaSceneFn;
    };
}


#endif // GAMEPROJECT_H
