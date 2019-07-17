#include <Nero/editor/ProjectManager.h>
#include <Nero/core/utility/StringUtil.h>
#include <Nero/core/utility/FileUtil.h>
#include <Nero/core/utility/DateUtil.h>
#include <Nero/core/utility/LogUtil.h>
#include <experimental/filesystem>

namespace nero
{
    ProjectManager::ProjectManager():
        m_ProjectTable()
    {

    }


    void ProjectManager::createProject(const nlohmann::json& projectJson, Project_Type type)
    {
        std::string project_name = projectJson["project_name"].get<std::string>();

        assert(!isProjectExist(project_name));

        //get project name
        project_name = formatString(project_name);

        //create project directory
        createDirectory(getPath({m_EditorSetting["workspace_folder"].get<std::string>(), project_name}));

        //project json
        nlohmann::json project_json;
        project_json["project_name"]            = projectJson["project_name"].get<std::string>();
        project_json["project_id"]              = project_name;
        project_json["creation_date"]           = formatDateTime(getCurrentDateTime());
        project_json["modification_date"]       = formatDateTime(getCurrentDateTime());
        project_json["project_lead"]            = projectJson["project_lead"].get<std::string>();
        project_json["project_company"]         = projectJson["project_company"].get<std::string>();
        project_json["project_description"]     = projectJson["project_description"].get<std::string>();

        //save project json
        saveFile(getPath({m_EditorSetting["workspace_folder"].get<std::string>(), project_name, "nero_project"}, StringPool.EXTANSION_JSON), project_json.dump(3));


        //lua project
        //create scene folder, script, chunk, startup, saving
        //create resource folder
        //create setting folder
    }

    void ProjectManager::createProject(const std::string& projectName, Project_Type type)
    {
        assert(!isProjectExist(projectName));

        //get project name
        std::string project_name = formatString(projectName);

        //create project directory
        createDirectory(getPath({m_EditorSetting["workspace_folder"].get<std::string>(), project_name}));

        //project json
        nlohmann::json project_json;
        project_json["project_name"]            = projectName;
        project_json["project_id"]              = project_name;
        project_json["creation_date"]           = formatDateTime(getCurrentDateTime());
        project_json["modification_date"]       = formatDateTime(getCurrentDateTime());
        project_json["project_lead"]            = m_EditorSetting["project_lead"].get<std::string>();
        project_json["project_company"]         = m_EditorSetting["project_company"].get<std::string>();
        project_json["project_description"]     = m_EditorSetting["project_description"].get<std::string>();

        //save project json
        saveFile(getPath({m_EditorSetting["workspace_folder"].get<std::string>(), project_name, "nero_project"}, StringPool.EXTANSION_JSON), project_json.dump(3));
    }


    bool ProjectManager::isProjectExist(const std::string& projectName)
    {
        //check if project director exist
        if(!directoryExist(getPath({m_EditorSetting["workspace_folder"].get<std::string>(), formatString(projectName)})))
        {
            return false;
        }

        //check if project file exist
        std::string project_file = getPath({m_EditorSetting["workspace_folder"].get<std::string>(), formatString(projectName), "nero_project"}, StringPool.EXTANSION_JSON);
        if(!fileExist(project_file))
        {
            return false;
        }

        //check if project_id exist
        nlohmann::json project_json = loadJson(project_file, true);
        std::string project_id = project_json["project_id"].get<std::string>();
        if(project_id != formatString(projectName))
        {
            return false;
        }

        return true;
    }

    void ProjectManager::setEditorSetting(const nlohmann::json& setting)
    {
        m_EditorSetting = setting;
    }

    void ProjectManager::loadAllProject()
    {
        using namespace  std::experimental::filesystem;
        path folder_path(m_EditorSetting["workspace_folder"].get<std::string>());

        directory_iterator it{folder_path};
        while (it != directory_iterator{})
        {
            std::string project_folder = it->path().string();
            std::string project_file = getPath({project_folder, "nero_project"}, StringPool.EXTANSION_JSON);

            if(fileExist(project_file))
            {
                m_ProjectTable.push_back(loadJson(project_file, true));
            }

            it++;
        }
    }

    const std::vector<nlohmann::json>& ProjectManager::getProjectTable() const
    {
        return m_ProjectTable;
    }
}
