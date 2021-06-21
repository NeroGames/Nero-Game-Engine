////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/editor/ProjectManager.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/utility/DateTime.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
//Boost
#include <boost/algorithm/string.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll.hpp>
//Std
#include <windows.h>
#include <stdio.h>
#include <experimental/filesystem>
////////////////////////////////////////////////////////////
namespace nero
{
	ProjectManager::ProjectManager():
		 m_GameProject(nullptr)
		,m_EditorSetting(nullptr)
	{

	}

	ProjectManager::~ProjectManager()
	{

	}

	void ProjectManager::createWorkspace(const Parameter& parameter)
	{
		nero_log("creating new project worksapce");

		nero_log(parameter.getString("workspace_location"));
		nero_log(parameter.getString("workspace_name"));

		std::string directory = file::getPath({parameter.getString("workspace_location"), parameter.getString("workspace_name")});
		file::createDirectory(directory);
		file::createDirectory(file::getPath({directory, "Project"}));

		//create workspace document
		Parameter document;
		document.setString("engine_version", EngineConstant.ENGINE_VERSION);
		document.setString("document_type", EngineConstant.DOCUMENT_TYPE_WORKSPACE);
		document.setString("workspace_name", parameter.getString("workspace_name"));
		document.setString("project_lead", parameter.getString("project_lead"));
		document.setString("project_namespace", parameter.getString("project_namespace"));
		document.setString("company_name", parameter.getString("company_name"));
		file::saveFile(file::getPath({directory, ".workspace"}), document.toString());

		//update workspace setting
		auto worksapceSetting =  file::loadJson(file::getPath({"setting", "workspace"}));

		nlohmann::json workspace;
		workspace["order"]					= worksapceSetting.size() + 1;
		workspace["workspace_id"]           = string::formatString(parameter.getString("workspace_name"));
		workspace["workspace_name"]         = parameter.getString("workspace_name");
		workspace["workspace_directory"]    = directory;
		workspace["project_lead"]			= parameter.getString("project_lead");
		workspace["company_name"]			= parameter.getString("company_name");
		workspace["project_namespace"]      = parameter.getString("project_namespace");

		worksapceSetting.push_back(workspace);

		file::saveFile(file::getPath({"setting", "workspace"}, StringPool.EXT_JSON), worksapceSetting.dump(3), true);
	}

	void ProjectManager::importWorkspace(const std::string& directory)
	{
		auto worksapceSetting =  file::loadJson(file::getPath({"setting", "workspace"}));

		Parameter parameter;
		parameter.loadJson(file::loadJson(file::getPath({directory, ".workspace"}), true));

		//update workspace to match directory
		parameter.setString("workspace_name", file::getFileName(directory));

		nlohmann::json workspace;
		workspace["order"]					= worksapceSetting.size() + 1;
		workspace["workspace_id"]           = string::formatString(parameter.getString("workspace_name"));
		workspace["workspace_name"]         = parameter.getString("workspace_name");
		workspace["workspace_directory"]    = directory;
		workspace["project_lead"]			= parameter.getString("project_lead");
		workspace["company_name"]			= parameter.getString("company_name");
		workspace["project_namespace"]      = parameter.getString("project_namespace");

		worksapceSetting.push_back(workspace);

		file::saveFile(file::getPath({"setting", "workspace"}, StringPool.EXT_JSON), worksapceSetting.dump(3), true);
		file::saveFile(file::getPath({directory, ".workspace"}), parameter.toString());
	}

	void ProjectManager::deleteWorksapce(const std::string &directory)
	{
		//TODO delete workspace
	}

	const nlohmann::json ProjectManager::getWorkspaceTable() const
	{
		return file::loadJson(file::getPath({"setting", "workspace"}));
	}

	bool ProjectManager::workspaceExist(const std::string& workspaceName)
	{
		std::string workspace_id = string::formatString(workspaceName);

		for(auto workspace : getWorkspaceTable())
		{
		   if(workspace["workspace_id"].get<std::string>() == workspace_id)
		   {
			   return true;
		   }
		}

		return false;
	}

	const std::vector<std::string> ProjectManager::getWorkspaceNameTable() const
	{
		auto workspaceTable =  file::loadJson(file::getPath({"setting", "workspace"}));

		std::vector<std::string> result;

		for(auto workspace : workspaceTable)
		{
			result.push_back(workspace["workspace_name"].get<std::string>());
		}

		return result;
	}

	const nlohmann::json ProjectManager::findWorkspace(const std::string& name) const
	{
		auto workspaceTable =  file::loadJson(file::getPath({"setting", "workspace"}));

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

		if(backgroundTask->getErrorCode() == 0)
		{
			updateRecentProject(getProjectDirectory(parameter));
		}
	}

	std::string ProjectManager::getProjectDirectory(const Parameter& parameter)
	{
		std::string workspaceDirectory	= findWorkspace(parameter.getString("workspace_name"))["workspace_directory"].get<std::string>();
		return file::getPath({workspaceDirectory, "Project", parameter.getString("project_name")});
	}

	void ProjectManager::createCppProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask)
	{
		nero_log("creating a new cpp project");

		//step 1-1 : Create directory struture
		backgroundTask->setStatus(1);
		backgroundTask->addMessage("generating project directory ...");

		std::string project_name = parameter.getString("project_name");

		std::string workspaceDirectory	= findWorkspace(parameter.getString("workspace_name"))["workspace_directory"].get<std::string>();
		std::string projectDirectory	= file::getPath({workspaceDirectory, "Project", parameter.getString("project_name")});

		file::createDirectory(file::getPath({projectDirectory}));
		file::createDirectory(file::getPath({projectDirectory, "Source"}));
		file::createDirectory(file::getPath({projectDirectory, "Resource"}));
			file::createDirectory(file::getPath({projectDirectory, "Source", project_name}));
				file::createDirectory(file::getPath({projectDirectory, "Source", project_name, "cpp"}));
					file::createDirectory(file::getPath({projectDirectory, "Source", project_name, "cpp", "level"}));
					file::createDirectory(file::getPath({projectDirectory, "Source", project_name, "cpp", "screen"}));
					file::createDirectory(file::getPath({projectDirectory, "Source", project_name, "cpp", "script"}));
		file::createDirectory(file::getPath({projectDirectory, "Build"}));
		file::createDirectory(file::getPath({projectDirectory, "Scene"}));
			file::createDirectory(file::getPath({projectDirectory, "Scene", "Level"}));
			file::createDirectory(file::getPath({projectDirectory, "Scene", "Screen"}));
			file::createDirectory(file::getPath({projectDirectory, "Scene", "Factory"}));
			file::createDirectory(file::getPath({projectDirectory, "Scene", "Class"}));
		file::createDirectory(file::getPath({projectDirectory, "Setting"}));

		ResourceManager::buildDirectory(file::getPath({projectDirectory, "Resource"}));

		//Step 1-2 : Create project document
		Setting document;
		document.setString("engine_version",	EngineConstant.ENGINE_VERSION);
		document.setString("document_type",		EngineConstant.DOCUMENT_TYPE_PROJECT);
		document.setString("creation_date",		datetime::formatDateTime(datetime::getCurrentDateTime()));
		document.setString("modification_date", datetime::formatDateTime(datetime::getCurrentDateTime()));
		document.setString("project_name",		parameter.getString("project_name"));
		document.setString("project_id",		string::formatString(parameter.getString("project_name")));
		document.setString("project_type",		parameter.getString("project_type"));
		document.setString("code_editor",		parameter.getString("code_editor"));
		document.setString("company_name",		parameter.getString("company_name"));
		document.setString("project_lead",		parameter.getString("project_lead"));
		document.setString("project_namespace", parameter.getString("project_namespace"));
		document.setString("description",		parameter.getString("description"));
		document.setString("workspace_name",	parameter.getString("workspace_name"));
		document.setBool("startup_pack",		parameter.getBool("startup_pack"));

		file::saveFile(file::getPath({projectDirectory, ".project"}), document.toString());

		//Step 2 : generate project source
		backgroundTask->setStatus(2);
		backgroundTask->addMessage("generating project files ...");

		//Step 2 : Generate file
		std::string cmake_template			= file::loadText("template/cpp_project/CMakeLists.txt");
		std::string cmake_setting_template	= file::loadText("template/cpp_project/CMakeSettings.json");
		std::string scene_header_template	= file::loadText("template/cpp_project/CppScene.h");
		std::string scene_source_template	= file::loadText("template/cpp_project/CppScene.cpp");
		std::string screen_header_template	= file::loadText("template/cpp_project/CppStartupScreen.h");
		std::string screen_source_template	= file::loadText("template/cpp_project/CppStartupScreen.cpp");

		auto wordTable = string::getWordTable(parameter.getString("project_name"));

		std::string scene_class		= formatSceneClassName(wordTable);
		std::string class_header	= formatHeaderGard(wordTable);

		//file 1 : header
		boost::algorithm::replace_all(scene_header_template, "::SceneClass::",		scene_class);
		boost::algorithm::replace_all(scene_header_template, "::HeaderGard::",		class_header);
		boost::algorithm::replace_all(scene_header_template, "::Namespace::",		parameter.getString("project_namespace"));
		boost::algorithm::replace_all(scene_header_template, "::ProjectName::",	parameter.getString("project_name"));
		boost::algorithm::replace_all(scene_header_template, "::ProjectLead::",	parameter.getString("project_lead"));
		boost::algorithm::replace_all(scene_header_template, "::CoyprightDate::",	toString(datetime::getCurrentDateTime().date().year()));

		//file 2 : source
		boost::algorithm::replace_all(scene_source_template, "::SceneClass::",		scene_class);
		boost::algorithm::replace_all(scene_source_template, "::Namespace::",		parameter.getString("project_namespace"));
		boost::algorithm::replace_all(scene_source_template, "::ProjectName::",	parameter.getString("project_name"));
		boost::algorithm::replace_all(scene_source_template, "::ProjectLead::",	parameter.getString("project_lead"));
		boost::algorithm::replace_all(scene_source_template, "::CoyprightDate::",	toString(datetime::getCurrentDateTime().date().year()));

		//file 3 : cmake text list
		boost::algorithm::replace_all(cmake_template, "::Project_Name::",					formatCmakeProjectName(wordTable));
		boost::algorithm::replace_all(cmake_template, "::Project_Library::",				formatCmakeProjectLibrary(wordTable));

		//file 4 : cmake setting
		nero_log(projectDirectory);
		nero_log(file::getPath({projectDirectory, "Build"}));
		boost::algorithm::replace_all(cmake_setting_template, "::Project_Build_Directory::", file::escapeBackslash(file::getPath({projectDirectory, "Build"})));

		//file 5 : startup screen header
		boost::algorithm::replace_all(screen_header_template, "::StartupScreenClass::", "StartupScreen");
		boost::algorithm::replace_all(screen_header_template, "::HeaderGard::",		"STARTUPSCREEN_H");
		boost::algorithm::replace_all(screen_header_template, "::Namespace::",		parameter.getString("project_namespace"));
		boost::algorithm::replace_all(screen_header_template, "::ProjectName::",	parameter.getString("project_name"));
		boost::algorithm::replace_all(screen_header_template, "::ProjectLead::",	parameter.getString("project_lead"));
		boost::algorithm::replace_all(screen_header_template, "::CoyprightDate::",	toString(datetime::getCurrentDateTime().date().year()));

		//file 6 : startup screen header
		boost::algorithm::replace_all(screen_source_template, "::StartupScreenClass::", "StartupScreen");
		boost::algorithm::replace_all(screen_source_template, "::Namespace::",		parameter.getString("project_namespace"));
		boost::algorithm::replace_all(screen_source_template, "::ProjectName::",	parameter.getString("project_name"));
		boost::algorithm::replace_all(screen_source_template, "::ProjectLead::",	parameter.getString("project_lead"));
		boost::algorithm::replace_all(screen_source_template, "::CoyprightDate::",	toString(datetime::getCurrentDateTime().date().year()));

		//save file
		file::saveFile(file::getPath({projectDirectory, "Source", project_name, "CMakeLists"}, StringPool.EXT_TEXT), cmake_template);
		file::saveFile(file::getPath({projectDirectory, "Source", project_name, "CMakeSettings"}, StringPool.EXT_JSON), cmake_setting_template);
		file::saveFile(file::getPath({projectDirectory, "Source", project_name, "cpp", scene_class}, StringPool.EXT_H), scene_header_template);
		file::saveFile(file::getPath({projectDirectory, "Source", project_name, "cpp", scene_class}, StringPool.EXT_CPP), scene_source_template);
		file::saveFile(file::getPath({projectDirectory, "Source", project_name, "cpp", "StartupScreen"}, StringPool.EXT_H), screen_header_template);
		file::saveFile(file::getPath({projectDirectory, "Source", project_name, "cpp", "StartupScreen"}, StringPool.EXT_CPP), screen_source_template);

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
		backgroundTask->addMessage("project creation completed");
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

	bool ProjectManager::projectExist(const std::string& projectName, const std::string& workspaceName)
    {
		//check workspace exist
		if(!workspaceExist(workspaceName))
		{
			return false;
		}

		//check project exist
		std::string project_id = string::formatString(projectName);
		for(auto project : getWorkspaceProjectTable(workspaceName))
		{
			if(project["project_id"].get<std::string>() == project_id)
			{
				return true;
			}
		}

		return false;
	}

	void ProjectManager::setSetting(const Setting::Ptr& setting)
    {
		m_EditorSetting = setting;
    }


    const std::vector<nlohmann::json> ProjectManager::getWorkspaceProjectTable(const std::string& workspace_name)
	{
        std::vector<nlohmann::json> result;

		nlohmann::json projectWorkpsace = findWorkspace(workspace_name);

		std::experimental::filesystem::path folder_path(file::getPath({projectWorkpsace["workspace_directory"].get<std::string>(), "Project"}));

		std::experimental::filesystem::directory_iterator it{folder_path};
		while (it != std::experimental::filesystem::directory_iterator{})
        {
            std::string project_folder = it->path().string();
			std::string project_file = file::getPath({project_folder, ".project"});

			if(file::fileExist(project_file))
			{
				result.push_back(file::loadJson(project_file, true));
            }

            it++;
        }

        return result;
    }



	void ProjectManager::compileProject(const std::string& projectDirectory, BackgroundTask::Ptr backgroundTask)
	{
		Parameter parameter;
		parameter.loadJson(file::loadJson(file::getPath({projectDirectory, ".project"}), true));
		//remote
		std::string sourcePath  = file::getPath({projectDirectory, "Source", parameter.getString("project_name")});
		std::string buildPath   = file::getPath({projectDirectory, "Build"});

		std::string nero_game_home	= m_EditorSetting->getSetting("environment").getString("nero_game_home");

		std::string mingw32			= nero_game_home + "/Compiler/bin/mingw32-make.exe";
		std::string gxx				= nero_game_home + "/Compiler/bin/g++.exe";
		std::string gcc				= nero_game_home + "/Compiler/bin/gcc.exe";
		std::string cmake			= nero_game_home + "/Tools/Cmake/bin/cmake.exe";

		backgroundTask->addMessage("  -> (1/3) cleaning project");
		cmd::Process cleanProcess	=  cmd::runCommand(mingw32, {"-C", buildPath, "-k", "clean"});
		nero_log("clean project exit code = " + toString(cleanProcess.getExistCode()));

		backgroundTask->addMessage("  -> (2/3) configuring project");
		cmd::Process configProcess	=  cmd::runCommand(cmake, {"-G", "MinGW Makefiles", "-S", sourcePath, "-B", buildPath,
															   "-D", "CMAKE_CXX_COMPILER=" + file::getPath(gxx), "-D", "CMAKE_C_COMPILER="	+ file::getPath(gcc),
																"-D", "CMAKE_MAKE_PROGRAM=" + file::getPath(mingw32)});
		nero_log("configure project exit code = " + toString(configProcess.getExistCode()));

		backgroundTask->addMessage("  -> (3/3) building project");
		cmd::Process buildProcess	=  cmd::runCommand(mingw32, {"-C", buildPath});
		nero_log("build project exit code = " + toString(buildProcess.getExistCode()));
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
		return file::getCurrentPath();
     }

	 GameProject::Ptr ProjectManager::openProject(const std::string& projectDirectory)
     {
		nero_log("openning project " + projectDirectory);

		Setting::Ptr setting = std::make_shared<Setting>();
		setting->loadSetting(file::getPath({projectDirectory, ".project"}), true, true);
		setting->setString("project_directory", projectDirectory);
		setting->setString("workspace_directory", file::getParentDirectory(projectDirectory, 2));

		m_GameProject = GameProject::Ptr(new GameProject());
		m_GameProject->init(setting);

		/*m_GameProject->setSetting(m_EditorSetting);
		m_GameProject->setRenderTexture(m_RenderTexture);
		m_GameProject->setRenderContext(m_RenderContext);
		m_GameProject->setCamera(m_Camera);*/

		/*m_GameProject->loadResource(parameter);
		nero_log("initializing project");

		nero_log("loading project");
		m_GameProject->loadProject();
		nero_log("loading project library");
		m_GameProject->loadLibrary();
		//m_GameProject->loadLibraryDemo();
		nero_log("openning editor");
		m_GameProject->openEditor();*/

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
		 auto recentProject =  file::loadJson(file::getPath({"setting", "recent_project"}));

		 Parameter projectDocument;
		 projectDocument.loadJson(file::loadJson(file::getPath({projectDirectory, ".project"}), true));

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

		file::saveFile(file::getPath({"setting", "recent_project"}, StringPool.EXT_JSON), recentProject.dump(3), true);


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
}
