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
#include <Nero/core/scene/Scene.h>
#include <Nero/core/luascene/LuaScene.h>
#include <Nero/core/engine/BackgroundTask.h>

/////////////////////////////////////////////////////////////


namespace nero
{
    class GameProject
    {
        public:
            typedef std::shared_ptr<GameProject> Ptr;
            typedef Scene::Ptr (CreateCppSceneFn)(Scene::Context);
			//typedef LuaScene::Ptr (CreateLuaSceneFn)(Scene::Context);

        public:
            GameProject();

            void init(const nlohmann::json& project, const nlohmann::json& project_workpsace);
            void loadProject();
            void loadProjectLibrary();
            void openEditor();
            void compileProject();

            std::string exec(const char* cmd);

            AdvancedScene::Ptr  m_AdvancedScene;
            Scene::Ptr  m_Scene;
            int m_ProjectCompilationStatus;
            int m_CleanProjectResult;
            int m_ConfigureProjectResult;
            int m_BuildProjectResult;
            std::string m_CleanProjectCommand;
            std::string m_ConfigureProjectCommand;
            std::string m_BuildProjectCommand;
            std::string m_CmakeListFile;
            std::string m_EditorProcessId;
            boost::function<CreateCppSceneFn> m_CreateCppSceneFn;
			//boost::function<CreateLuaSceneFn> m_CreateLuaSceneFn;
            //Directory
            std::string m_ProjectDirectory;
            std::string m_ProjectBuildDirectory;
            std::string m_ProjectSourceDirectory;

             AdvancedScene::Ptr getAdvancedScene();
            //File

			std::string m_NeroGameNome;
			std::string m_QTCreatorExcecutable;
			std::string m_VisualStutionExecutable;

            //Library
            std::string m_ProjectLibraryFile;
            std::string m_ProjectLibraryCopyFile;

			std::vector<BackgroundTask>& getBackgroundTaskTable();

		private:
			BackgroundTask& createBackgroundTask(const std::string& name, const std::string& category);
			std::vector<BackgroundTask>  m_BackgroundTaskTable;
    };
}


#endif // GAMEPROJECT_H
