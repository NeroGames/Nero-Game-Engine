#include <Nero/editor/ProjectManager.h>
#include <Nero/core/cpp/utility/FileUtil.h>
#include <Nero/core/cpp/utility/DateUtil.h>

#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
//STD
#include <windows.h>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <boost/dll/import.hpp>
#include <experimental/filesystem>

#include <boost/dll.hpp>

namespace nero
{
    ProjectManager::ProjectManager():
         m_ProjectTable()
        ,m_EditorPid(StringPool.BLANK)
        ,m_GameProject()
		,m_EditorSetting(nullptr)
    {

    }

	void ProjectManager::createProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask)
	{
		nero_log("creating project : " + parameter.getString("project_name"));

		std::string projectType = parameter.getString("project_type");

		if(projectType == "CPP Project")
		{
			createCppProject(parameter, backgroundTask);
		}
		else if(projectType == "Lua Project")
		{
			createLuaProject(parameter, backgroundTask);
		}
		else if(projectType == "CPP and Lua Project")
		{
			createCppLuaProject(parameter, backgroundTask);
		}

		/*if(backgroundTask->getErrorCode() != 0)
		{
			std::string workspaceDirectory	= findWorkspace(parameter.getString("workspace_name"))["workspace_directory"].get<std::string>();
			std::string projectDirectory	= getPath({workspaceDirectory, "Project", parameter.getString("project_name")});

			//std::this_thread::sleep_for(std::chrono::seconds(1));
			removeDirectory(projectDirectory, true);

			return;
		}*/

		updateRecentProject(getProjectDirectory(parameter));
	}

	std::string ProjectManager::getProjectDirectory(const Parameter& parameter)
	{
		std::string workspaceDirectory	= findWorkspace(parameter.getString("workspace_name"))["workspace_directory"].get<std::string>();
		return getPath({workspaceDirectory, "Project", parameter.getString("project_name")});
	}

	void ProjectManager::createCppProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask)
	{
		nero_log("creating cpp_project");

		//Step 1-1 : Create directory struture
		backgroundTask->setStatus(1);
		backgroundTask->addMessage("generating project directory ...");

		std::string project_name = parameter.getString("project_name");

		std::string workspaceDirectory	= findWorkspace(parameter.getString("workspace_name"))["workspace_directory"].get<std::string>();
		std::string projectDirectory	= getPath({workspaceDirectory, "Project", parameter.getString("project_name")});

		createDirectory(getPath({projectDirectory}));
		createDirectory(getPath({projectDirectory, "Source"}));
		createDirectory(getPath({projectDirectory, "Resource"}));
			createDirectory(getPath({projectDirectory, "Source", project_name}));
				createDirectory(getPath({projectDirectory, "Source", project_name, "cpp"}));
				createDirectory(getPath({projectDirectory, "Source", project_name, "cpp", "script"}));
					createDirectory(getPath({projectDirectory, "Source", project_name, "cpp", "gameobject"}));
					createDirectory(getPath({projectDirectory, "Source", project_name, "cpp", "level"}));
					createDirectory(getPath({projectDirectory, "Source", project_name, "cpp", "screen"}));
					createDirectory(getPath({projectDirectory, "Source", project_name, "cpp", "startupscreen"}));
		createDirectory(getPath({projectDirectory, "Build"}));
		createDirectory(getPath({projectDirectory, "Scene"}));
			createDirectory(getPath({projectDirectory, "Scene", "Level"}));
			createDirectory(getPath({projectDirectory, "Scene", "Screen"}));
			createDirectory(getPath({projectDirectory, "Scene", "Factory"}));
			createDirectory(getPath({projectDirectory, "Scene", "Class"}));
		createDirectory(getPath({projectDirectory, "Setting"}));

		ResourceManager::buildDirectory(getPath({projectDirectory, "Resource"}));

		//Step 1-2 : Create project document
		Setting document;
		document.setString("engine_version",	EngineConstant.ENGINE_VERSION);
		document.setString("document_type",		EngineConstant.DOCUMENT_TYPE_PROJECT);
		document.setString("creation_date",		formatDateTime(getCurrentDateTime()));
		document.setString("modification_date", formatDateTime(getCurrentDateTime()));
		document.setString("project_name",		parameter.getString("project_name"));
		document.setString("project_type",		parameter.getString("project_type"));
		document.setString("code_editor",		parameter.getString("code_editor"));
		document.setString("company_name",		parameter.getString("company_name"));
		document.setString("project_lead",		parameter.getString("project_lead"));
		document.setString("project_namespace", parameter.getString("project_namespace"));
		document.setString("description",		parameter.getString("description"));
		document.setString("workspace_name",	parameter.getString("workspace_name"));
		document.setBool("startup_pack",		parameter.getBool("startup_pack"));

		saveFile(getPath({projectDirectory, ".project"}), document.toString());

		//Step 2 : generate project source
		backgroundTask->setStatus(2);
		backgroundTask->addMessage("generating project file ...");

		//Step 2 : Generate file
		std::string cmake_template			= loadText("template/cpp-project/CMakeLists.txt");
		std::string cmake_setting_template	= loadText("template/cpp-project/CMakeSettings.json");
		std::string scene_header_template	= loadText("template/cpp-project/CppScene.h");
		std::string scene_source_template	= loadText("template/cpp-project/CppScene.cpp");

		auto wordTable = getWordTable(parameter.getString("project_name"));

		std::string scene_class		= formatSceneClassName(wordTable);
		std::string class_header	= formatHeaderGard(wordTable);

		//file 1 : header
		boost::algorithm::replace_all(scene_header_template, "::Scene_Class::",		scene_class);
		boost::algorithm::replace_all(scene_header_template, "::Header_Gard::",		class_header);
		boost::algorithm::replace_all(scene_header_template, "::Namespace::",		parameter.getString("project_namespace"));
		boost::algorithm::replace_all(scene_header_template, "::Project_Name::",	parameter.getString("project_name"));
		boost::algorithm::replace_all(scene_header_template, "::Project_Lead::",	parameter.getString("project_lead"));
		boost::algorithm::replace_all(scene_header_template, "::Coypright_Date::",	toString(getCurrentDateTime().date().year()));

		//file 2 : source
		boost::algorithm::replace_all(scene_source_template, "::Scene_Class::",		scene_class);
		boost::algorithm::replace_all(scene_source_template, "::Namespace::",		parameter.getString("project_namespace"));
		boost::algorithm::replace_all(scene_source_template, "::Project_Name::",	parameter.getString("project_name"));
		boost::algorithm::replace_all(scene_source_template, "::Project_Lead::",	parameter.getString("project_lead"));
		boost::algorithm::replace_all(scene_source_template, "::Coypright_Date::",	toString(getCurrentDateTime().date().year()));

		//file 3 : cmake text list
		boost::algorithm::replace_all(cmake_template, "::Project_Name::",					formatCmakeProjectName(wordTable));
		boost::algorithm::replace_all(cmake_template, "::Engine_Installation_Directory::",	getEngineDirectory());
		boost::algorithm::replace_all(cmake_template, "::Project_Library::",				formatCmakeProjectLibrary(wordTable));

		//file 4 : cmake setting
		nero_log(projectDirectory);
		nero_log(getPath({projectDirectory, "Build"}));
		boost::algorithm::replace_all(cmake_setting_template, "::Project_Build_Directory::", escapeAntiSlash(getPath({projectDirectory, "Build"})));

		saveFile(getPath({projectDirectory, "Source", project_name, "CMakeLists"}, StringPool.EXTENSION_TEXT), cmake_template);
		saveFile(getPath({projectDirectory, "Source", project_name, "CMakeSettings"}, StringPool.EXTENSION_JSON), cmake_setting_template);
		saveFile(getPath({projectDirectory, "Source", project_name, "cpp", scene_class}, StringPool.EXTENSION_CPP_HEADER), scene_header_template);
		saveFile(getPath({projectDirectory, "Source", project_name, "cpp", scene_class}, StringPool.EXTENSION_CPP_SOURCE), scene_source_template);

		//Step 3 : compile the project
		backgroundTask->setStatus(3);
		backgroundTask->addMessage("compiling project ...");

		compileProject(projectDirectory, backgroundTask);

		/*if(backgroundTask->getErrorCode() != 0)
		{
			backgroundTask->setStatus(-1);
			backgroundTask->addMessage("Failed to create project");
			return;
		}*/

		//compplet
		backgroundTask->setStatus(4);
		backgroundTask->addMessage("project creation completed ...");
		backgroundTask->setCompleted(true);
	}

	void ProjectManager::createLuaProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask)
	{
		nero_log("not_implemented");
	}

	void ProjectManager::createCppLuaProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask)
	{
		nero_log("not_implemented");
	}

	/*bool ProjectManager::isProjectExist(const std::string& projectName)
    {
        //check if project director exist
        if(!directoryExist(getPath({m_EditorSetting["workspace_folder"].get<std::string>(), formatString(projectName)})))
        {
            return false;
        }

        //check if project file exist
        std::string project_file = getPath({m_EditorSetting["workspace_folder"].get<std::string>(), formatString(projectName), "nero_project"}, StringPool.EXTENSION_JSON);
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
	}*/

	void ProjectManager::setSetting(const Setting::Ptr& setting)
    {
		m_EditorSetting = setting;
    }

	/*void ProjectManager::loadAllProject()
    {
        using namespace  std::experimental::filesystem;
        path folder_path(m_EditorSetting["workspace_folder"].get<std::string>());

        directory_iterator it{folder_path};
        while (it != directory_iterator{})
        {
            std::string project_folder = it->path().string();
            std::string project_file = getPath({project_folder, "nero_project"}, StringPool.EXTENSION_JSON);

            if(fileExist(project_file))
            {
                m_ProjectTable.push_back(loadJson(project_file, true));
            }

            it++;
        }
	}*/

    const std::vector<nlohmann::json>& ProjectManager::getProjectTable() const
    {
        return m_ProjectTable;
    }

    const std::vector<nlohmann::json> ProjectManager::getWorkspaceProjectTable(const std::string& workspace_name)
    {
        std::vector<nlohmann::json> result;

        nlohmann::json projectWorkpsace = findWorkspace(workspace_name);

		std::experimental::filesystem::path folder_path(getPath({projectWorkpsace["workspace_directory"].get<std::string>(), "Project"}));

		std::experimental::filesystem::directory_iterator it{folder_path};
		while (it != std::experimental::filesystem::directory_iterator{})
        {
            std::string project_folder = it->path().string();
			std::string project_file = getPath({project_folder, ".project"});

            if(fileExist(project_file))
            {
                result.push_back(loadJson(project_file, true));
            }

            it++;
        }

        return result;
    }

	void ProjectManager::createWorkspace(const Parameter& parameter)
    {
		nero_log("creating new project worksapce");

		nero_log(parameter.getString("workspace_location"));
		nero_log(parameter.getString("workspace_name"));

		std::string directory = getPath({parameter.getString("workspace_location"), parameter.getString("workspace_name")});
		createDirectory(directory);
		createDirectory(getPath({directory, "Project"}));

		//create workspace document
		Parameter document;
		document.setString("engine_version", EngineConstant.ENGINE_VERSION);
		document.setString("document_type", EngineConstant.DOCUMENT_TYPE_WORKSPACE);
		document.setString("workspace_name", parameter.getString("workspace_name"));
		document.setString("project_lead", parameter.getString("project_lead"));
		document.setString("project_namespace", parameter.getString("project_namespace"));
		document.setString("company_name", parameter.getString("company_name"));
		saveFile(getPath({directory, ".workspace"}), document.toString());

        //update workspace setting
		auto worksapceSetting =  loadJson(getPath({"setting", "workspace"}));

		nlohmann::json workspace;
		workspace["order"]					= worksapceSetting.size() + 1;
		workspace["workspace_id"]           = formatString(parameter.getString("workspace_name"));
		workspace["workspace_name"]         = parameter.getString("workspace_name");
		workspace["workspace_directory"]    = directory;
		workspace["project_lead"]			= parameter.getString("project_lead");
		workspace["company_name"]			= parameter.getString("company_name");
		workspace["project_namespace"]      = parameter.getString("project_namespace");

		worksapceSetting.push_back(workspace);

		saveFile(getPath({"setting", "workspace"}, StringPool.EXTENSION_JSON), worksapceSetting.dump(3), true);
    }

	void ProjectManager::importWorkspace(const std::string& directory)
	{
		auto worksapceSetting =  loadJson(getPath({"setting", "workspace"}));

		Parameter parameter;
		parameter.loadJson(loadJson(getPath({directory, ".workspace"}), true));

		nlohmann::json workspace;
		workspace["order"]					= worksapceSetting.size() + 1;
		workspace["workspace_id"]           = formatString(parameter.getString("workspace_name"));
		workspace["workspace_name"]         = parameter.getString("workspace_name");
		workspace["workspace_directory"]    = directory;
		workspace["project_lead"]			= parameter.getString("project_lead");
		workspace["company_name"]			= parameter.getString("company_name");
		workspace["project_namespace"]      = parameter.getString("project_namespace");

		worksapceSetting.push_back(workspace);

		saveFile(getPath({"setting", "workspace"}, StringPool.EXTENSION_JSON), worksapceSetting.dump(3), true);
	}


    const nlohmann::json ProjectManager::getWorkspaceTable() const
    {
		return loadJson(getPath({"setting", "workspace"}));
    }

    const std::vector<std::string> ProjectManager::getWorkspaceNameTable() const
    {
		auto workspaceTable =  loadJson(getPath({"setting", "workspace"}));

         std::vector<std::string> result;

         for(auto workspace : workspaceTable)
         {
			 result.push_back(workspace["workspace_name"].get<std::string>());
         }

         return result;
    }

    const nlohmann::json ProjectManager::findWorkspace(const std::string& name) const
    {
		auto workspaceTable =  loadJson(getPath({"setting", "workspace"}));

        nlohmann::json result;

        for(auto workspace : workspaceTable)
        {
            if(workspace["workspace_name"] == name)
            {
                result = workspace;
				break;
            }
        }

        return result;
    }

	void ProjectManager::compileProject(const std::string& projectDirectory, BackgroundTask::Ptr backgroundTask)
	{
		Parameter parameter;
		parameter.loadJson(loadJson(getPath({projectDirectory, ".project"}), true));
		//remote
		std::string sourcePath  = getPath({projectDirectory, "Source", parameter.getString("project_name")});
		std::string buildPath   = getPath({projectDirectory, "Build"});

		std::string clean       = "mingw32-make -C \"" + buildPath + "\" -k clean";
		std::string run_cmake   = "cmake -G \"MinGW Makefiles\" -S \"" + sourcePath + "\" -B \"" + buildPath + "\"";
		std::string build       = "mingw32-make -C \"" + buildPath + "\"";

		int error_code;

		error_code =  system(clean.c_str());
		nero_log("cleaning project error code : " + nero_ss(error_code));
		if(error_code != 0 && false)
		{
			backgroundTask->addMessage("Failed to clean project");
			backgroundTask->setErrorCode(error_code);
		}

		error_code = system(run_cmake.c_str());
		nero_log("configuring project error code : " + nero_ss(error_code));
		if(error_code != 0 && false)
		{
			backgroundTask->addMessage("Failed to configure project");
			backgroundTask->setErrorCode(error_code);

			return;
		}

		error_code = system(build.c_str());
		nero_log("building project error code : " + nero_ss(error_code));
		if(error_code != 0 && false)
		{
			backgroundTask->addMessage("Failed to build project");
			backgroundTask->setErrorCode(error_code);

			return;
		}
	}



	void ProjectManager::compileProject(const std::string& projectDirectory)
    {
		//
		Parameter parameter;
		parameter.loadJson(loadJson(getPath({projectDirectory, ".project"}), true));
        //remote
		std::string sourcePath  = getPath({projectDirectory, "Source", parameter.getString("project_name")});
        std::string buildPath   = getPath({projectDirectory, "Build"});

        std::string clean       = "mingw32-make -C \"" + buildPath + "\" -k clean";
		std::string run_cmake   = "cmake -G \"MinGW Makefiles\" -S \"" + sourcePath + "\" -B \"" + buildPath + "\"";
        std::string build       = "mingw32-make -C \"" + buildPath + "\"";

        system(clean.c_str());
        system(run_cmake.c_str());
        system(build.c_str());
    }

	void ProjectManager::editProject()
    {

    }

	std::string ProjectManager::formatSceneClassName(std::vector<std::string> wordTable)
	{
		 std::string result = StringPool.BLANK;

		 for(std::string s : wordTable)
		 {
			 if(s != StringPool.BLANK)
			 {
				 boost::algorithm::to_lower(s);
				 s[0] = std::toupper(s[0]);
				 result += s;
			 }
		 }

		 result += "Scene";

		 return result;
	}

	std::string ProjectManager::formatHeaderGard(std::vector<std::string> wordTable)
	{

		 std::string result = StringPool.BLANK;

		 for(std::string s : wordTable)
		 {
			 if(s != StringPool.BLANK)
			 {
				 boost::algorithm::to_upper(s);
				 result += s + "_";
			 }
		 }

		 result += "H";

		 return result;
	}

	std::string ProjectManager::formatCmakeProjectName(std::vector<std::string> wordTable)
	{

		 std::string result = StringPool.BLANK;

		 for(std::string s : wordTable)
		 {
			 if(s != StringPool.BLANK)
			 {
				 boost::algorithm::to_lower(s);
				 s[0] = std::toupper(s[0]);
				 result += s + " ";
			 }
		 }

		 result.pop_back();

		 return result;
	}


     std::string ProjectManager::formatCmakeProjectLibrary(std::vector<std::string> wordTable)
     {

         std::string result = StringPool.BLANK;

         for(std::string s : wordTable)
         {
             if(s != StringPool.BLANK)
             {
                 boost::algorithm::to_lower(s);
                 result += s;
             }
         }

         result = "nerogame-" + result;

         return result;
     }

     std::string ProjectManager::getEngineDirectory() const
     {
        //return getCurrentPath();

         return "C:/Program Files (x86)/Nero Game Engine";
     }

	 GameProject::Ptr ProjectManager::openProject(const std::string& projectDirectory)
     {
		nero_log("openning project " + projectDirectory);

		Parameter parameter;
		parameter.loadJson(loadJson(getPath({projectDirectory, ".project"}), true));
		parameter.setString("project_directory", projectDirectory);
		parameter.setString("workspace_directory", getParentDirectory(projectDirectory, 2));

		m_GameProject = GameProject::Ptr(new GameProject());
		m_GameProject->setSetting(m_EditorSetting);
		m_GameProject->setRenderTexture(m_RenderTexture);
		m_GameProject->setRenderContext(m_RenderContext);
		m_GameProject->setCamera(m_Camera);

		m_GameProject->loadResource(parameter);
		nero_log("initializing project");
		m_GameProject->init(parameter);
		nero_log("loading project");
		//m_GameProject->loadProject();
		nero_log("loading project library");
		m_GameProject->loadLibraryDemo();
		nero_log("openning editor");
		m_GameProject->openEditor();

		updateRecentProject(projectDirectory);


        return m_GameProject;
     }

	 void ProjectManager::closeProject()
	 {
		m_GameProject->close();
		m_GameProject = nullptr;
	 }

	 void ProjectManager::updateRecentProject(const std::string& projectDirectory)
	 {
		 auto recentProject =  loadJson(getPath({"setting", "recent_project"}));

		 Parameter projectDocument;
		 projectDocument.loadJson(loadJson(getPath({projectDirectory, ".project"}), true));

		for(int i = 0; i < recentProject.size(); i++)
		{
			if(recentProject[i]["project_name"].get<std::string>() == projectDocument.getString("project_name"))
			{
				recentProject.erase(i);
				break;
			}
		}

		Setting recent;
		recent.setString("project_directory", projectDirectory);
		recent.setString("project_name", projectDocument.getString("project_name"));
		recent.setString("workspace_name", projectDocument.getString("workspace_name"));

		recentProject.push_back(recent.toJson());

		saveFile(getPath({"setting", "recent_project"}, StringPool.EXTENSION_JSON), recentProject.dump(3), true);


	 }

     nlohmann::json ProjectManager::findProject(const std::string& workspace_name, const std::string& project_name)
     {
         nlohmann::json result;

        auto projectTable = getWorkspaceProjectTable(workspace_name);


        for(auto project : projectTable)
        {
            if(project["project_name"] == project_name)
            {
                result =  project;
            }
        }

        return result;
     }

	 bool ProjectManager::isWorksapceExist(const std::string& projectName)
	 {
		 return true;
	 }

	 void ProjectManager::setRenderTexture(const RenderTexturePtr& renderTexture)
	 {
		m_RenderTexture = renderTexture;
	 }

	 void ProjectManager::setCamera(const Camera::Ptr& camera)
	 {
		m_Camera = camera;
	 }

	 void ProjectManager::setRenderContext(const RenderContextPtr& renderContext)
	 {
		 m_RenderContext = renderContext;
	 }
}
