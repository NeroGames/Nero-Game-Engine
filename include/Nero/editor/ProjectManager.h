#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <memory>
#include <json/json.hpp>
namespace  nero
{
    class ProjectManager
    {
        public:
            typedef std::unique_ptr<ProjectManager> Ptr;

            enum Project_Type {LUA_PROJECT, CPP_PROJECT, LUA_CPP_PROJECT};

        public:
            ProjectManager();

           void createProject(const nlohmann::json& projectJson, Project_Type type);
           void createProject(const std::string& projectName, Project_Type type);

           bool isProjectExist(const std::string& projectName);

           void setEditorSetting(const nlohmann::json& setting);

           void loadAllProject();

           const std::vector<nlohmann::json>& getProjectTable() const;


        private:
           nlohmann::json      m_EditorSetting;
           std::vector<nlohmann::json>  m_ProjectTable;

    };
}



#endif // PROJECTMANAGER_H
