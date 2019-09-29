#include <Nero/editor/ProjectManager.h>
#include <Nero/core/utility/StringUtil.h>
#include <Nero/core/utility/FileUtil.h>
#include <Nero/core/utility/DateUtil.h>
#include <Nero/core/utility/LogUtil.h>
#include <experimental/filesystem>
//STD
#include <windows.h>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <boost/dll/import.hpp>

#include <boost/dll.hpp>

namespace nero
{
    ProjectManager::ProjectManager():
         m_ProjectTable()
        ,m_EditorPid(StringPool.BLANK)
        ,m_GameProject()
    {

    }


    void ProjectManager::createProject(const nlohmann::json& projectJson)
    {
        std::string project_name = projectJson["project_name"].get<std::string>();

        auto worksapceSetting = loadJson(getPath({"setting", "workspace_setting"}));
        nero_log(worksapceSetting);

        nlohmann::json projectWorkpsace;


        for(auto workpsace : worksapceSetting)
        {
            nero_log(workpsace.dump(3));
            nero_log(projectJson.dump(3));

            if(workpsace["workspace_name"].get<std::string>() == projectJson["workspace_name"].get<std::string>())
            {
               projectWorkpsace = workpsace;
               break;
            }
        }



        assert(!isProjectExist(project_name));

        //get project name
        project_name = formatString(project_name);

        nero_log(project_name);
        nero_log(projectWorkpsace);

        std::string projectFolder =getPath({projectWorkpsace["workspace_folder"].get<std::string>(), project_name});

        //create project directory
        createDirectory(projectFolder);
        createDirectory(getPath({projectFolder, "Source"}));
        createDirectory(getPath({projectFolder, "Build"}));
        createDirectory(getPath({projectFolder, "Scene"}));
        createDirectory(getPath({projectFolder, "Setting"}));

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
        saveFile(getPath({projectFolder, "nero_project"}, StringPool.EXTANSION_JSON), project_json.dump(3));


        //lua project
        //create scene folder, script, chunk, startup, saving
        //create resource folder
        //create setting folder
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

    void ProjectManager::createWorkspace(const nlohmann::json& workspaceJson)
    {
        //Create workspace
        //std::string  worksapce
        //createDirectory(workspaceJson["workspace_folder"].get<std::string>());

        saveFile(getPath({workspaceJson["workspace_folder"].get<std::string>(), "nero_workspace"}, StringPool.EXTANSION_JSON), workspaceJson.dump(3));

        //Update workspace_setting.json

        //nlohmann::json workspace_setting = loadJson(getPath({}));

        auto worksapceSetting = loadJson(getPath({"setting", "workspace_setting"}));

        nlohmann::json workspace;
        workspace["workspace_folder"] = workspaceJson["workspace_folder"];
        workspace["workspace_name"] = workspaceJson["workspace_name"];
        workspace["workspace_id"] = formatString(workspaceJson["workspace_name"].get<std::string>());
        workspace["default_project_lead"] = workspaceJson["default_project_lead"];
        workspace["default_company_name"] = workspaceJson["default_company_name"];
        workspace["default_namspace"] ="ng";
        workspace["count_workspace"] = worksapceSetting.size() + 1;

        worksapceSetting.push_back(workspace);

        saveFile(getPath({"setting", "workspace_setting"}, StringPool.EXTANSION_JSON), worksapceSetting.dump(3), true);
    }

    void ProjectManager::compileProject()
    {
        //remote
        system("mingw32-make -C \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\" -k clean");
        system("cmake -G \"MinGW Makefiles\" -S \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Source\" -B \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\"");
        system("mingw32-make -C \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\"");

        //WinExec("mingw32-make -C \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\" -k clean", SW_HIDE);
        //WinExec("cmake -G \"MinGW Makefiles\" -S \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Source\" -B \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\"", SW_HIDE);
        //WinExec("mingw32-make -C \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\"", SW_HIDE);

        //system("chdir");
        //system("cd \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\bonjour_25\\Build\" & chdir");
    }

    //Replace popen and pclose with _popen and _pclose for Windows
    std::string ProjectManager::exec(const char* cmd)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

        if (!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        return result;
    }

    void ProjectManager::editProject()
    {
        //system("qtcreator -client \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Source\\CMakeLists.txt\"");

        //get the list of qtcreator process
        std::string list_process    = "tasklist \/fo csv\| findstr \/i \"qtcreator\"";
        std::string processCSV      = exec(list_process.c_str());

        nero_log(processCSV);

        //If the Editor has been opened and are still available (has not been closed)
        if(m_EditorPid != StringPool.BLANK && processCSV.find(m_EditorPid) != std::string::npos)
        {
            //open it
            std::string cmd = "START \"\" qtcreator -pid " + m_EditorPid;
            nero_log(cmd);
            system(cmd.c_str());
        }
        else
        {
            //open a new process
            std::string open_editor = "START \"\" qtcreator \"C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Source\\CMakeLists.txt\"";
            system(open_editor.c_str());

            //save the process id
                 processCSV  = exec(list_process.c_str());
            auto processTab  = splitString(processCSV, '\r\n');

            nero_log(processTab.size());


            m_EditorPid = splitString(processTab.back(), ',').at(1);
            nero_log(m_EditorPid);

        }
    }

     void ProjectManager::loadLibrary()
     {

         m_GameScene = nullptr;
         m_CreateCppSceneFn.clear();





         boost::dll::fs::path full_path("C:/Users/sk-landry/Desktop/Bonjour/startspace/Build/liblearn-nero.dll");
         boost::dll::fs::path copy_path("C:/Users/sk-landry/Desktop/Bonjour/startspace/Build/liblearn-nero-copy.dll");

         if(boost::dll::fs::exists(copy_path))
         {
            boost::dll::fs::remove(copy_path);
         }
         //boost::dll::fs::copy(full_path, copy_path); //*/

         system("copy C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\\liblearn-nero.dll C:\\Users\\sk-landry\\Desktop\\Bonjour\\startspace\\Build\\liblearn-nero-copy.dll");


         if(true)
         {
             boost::dll::fs::path library_path("C:/Users/sk-landry/Desktop/Bonjour/startspace/Build");
             library_path /= "liblearn-nero-copy";

             //nero_log_if("balloon", boost::dll::fs::exists(copy_path));

             nero_log("loading scene library");

            m_CreateCppSceneFn = boost::dll::import_alias<CreateCppSceneFn>(             // type of imported symbol must be explicitly specified
                 library_path,                                            // path to library
                 "create_scene",                                                // symbol to import
                 boost::dll::load_mode::append_decorations                              // do append extensions and prefixes
             );

            nero_log("loading done");




            m_GameScene = m_CreateCppSceneFn(Scene::Context());

            //Scene boo = *m_GameScene.get();


            nero_log(m_GameScene->getName());
         }

     }
}
