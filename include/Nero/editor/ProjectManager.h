////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/editor/GameProject.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/lua/scene/LuaScene.h>
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/editor/AdvancedCamera.h>
//Json
#include <json/json.hpp>
//Boost
#include <boost/function.hpp>
//Std
#include <memory>
////////////////////////////////////////////////////////////
namespace  nero
{
    class ProjectManager
    {
        public:
			//attribute
			typedef std::shared_ptr<ProjectManager>			Ptr;
			typedef std::shared_ptr<sf::RenderTexture>		RenderTexturePtr;

        public:
			//ctr & dtr
													ProjectManager();
												   ~ProjectManager();
			//workspace
				//main
			void									createWorkspace(const Parameter& parameter);
			void									deleteWorksapce(const std::string& directory);
			void									importWorkspace(const std::string& directory);
				//utility
			const nlohmann::json					getWorkspaceTable() const;
			bool									workspaceExist(const std::string& workspaceName);
			const std::vector<std::string>			getWorkspaceNameTable() const;
			const nlohmann::json					findWorkspace(const std::string& name) const;

			//game project
				//main
			void									createProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
			void									createCppProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
			void									createLuaProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
			void									createCppLuaProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
			void									closeProject();
			void									deleteProject();
			void									compileProject(const std::string& projectDirectory, BackgroundTask::Ptr backgroundTask);
			GameProject::Ptr						openProject(const std::string& projectDirectory);
				//utiliy
			bool									projectExist(const std::string& projectName, const std::string& workspaceName);
			const std::vector<nlohmann::json>		getWorkspaceProjectTable(const std::string& workpsace);
			nlohmann::json							findProject(const std::string& workspace_name, const std::string& project_name);
			void									updateRecentProject(const std::string& projectDirectory);
			std::string								getProjectDirectory(const Parameter& parameter);


			//other
			void									setSetting(const Setting::Ptr& setting);
			std::string								getEngineDirectory() const;
			std::string								formatSceneClassName(std::vector<std::string> wordTable);
			std::string								formatHeaderGard(std::vector<std::string> wordTable);
			std::string								formatCmakeProjectName(std::vector<std::string> wordTable);
			std::string								formatCmakeProjectLibrary(std::vector<std::string> wordTable);


        private:
			GameProject::Ptr						m_GameProject;
			Setting::Ptr							m_EditorSetting;
    };
}

#endif // PROJECTMANAGER_H
