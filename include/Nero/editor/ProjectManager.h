#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <memory>
#include <json/json.hpp>
#include <Nero/core/engine/GameProject.h>
#include <Nero/core/scene/Scene.h>
#include <Nero/core/luascene/LuaScene.h>
#include <boost/function.hpp>

namespace  nero
{
    class ProjectManager
    {
        public:
            typedef std::unique_ptr<ProjectManager> Ptr;
            typedef Scene::Ptr (CreateCppSceneFn)(Scene::Context);
            typedef LuaScene::Ptr (CreateLuaSceneFn)(Scene::Context);

            enum Project_Type {LUA_PROJECT, CPP_PROJECT, CPP_LUA_PROJECT};

        public:
            ProjectManager();

           void createProject(const nlohmann::json& projectJson);
           void createWorkspace(const nlohmann::json& workspaceJson);

           bool isProjectExist(const std::string& projectName);

           void setEditorSetting(const nlohmann::json& setting);

           void loadAllProject();

           const std::vector<nlohmann::json>& getProjectTable() const;

           void compileProject();
           void editProject();

           void loadLibrary();

            std::string exec(const char* cmd);




        private:
           nlohmann::json      m_EditorSetting;
           std::vector<nlohmann::json>  m_ProjectTable;

           std::string m_EditorPid;

           GameProject::Ptr m_GameProject;

           boost::function<CreateCppSceneFn> m_CreateCppSceneFn;
           boost::function<CreateLuaSceneFn> m_CreateLuaSceneFn;
           Scene::Ptr m_GameScene;


    };
}



#endif // PROJECTMANAGER_H
