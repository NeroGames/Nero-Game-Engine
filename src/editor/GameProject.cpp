////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/editor/GameProject.h>
#include <Nero/core/utility/FileUtil.h>
#include <boost/dll.hpp>
#include <boost/algorithm/string.hpp>
/////////////////////////////////////////////////////////////

namespace nero
{
    GameProject::GameProject():
        m_AdvancedScene(nullptr)
        ,m_Scene(nullptr)
        ,m_ProjectCompilationStatus(0)
    {

    }

    void GameProject::init(const nlohmann::json& project, const nlohmann::json& project_workpsace)
    {
        std::string project_id = project["project_id"].get<std::string>();
        m_ProjectDirectory = getPath({project_workpsace["workspace_directory"].get<std::string>(), project_id});

        m_ProjectSourceDirectory  = getPath({m_ProjectDirectory, "Source"});
        m_ProjectBuildDirectory   = getPath({m_ProjectDirectory, "Build"});

        m_CmakeListFile = getPath({project_workpsace["workspace_directory"].get<std::string>(), project["project_id"].get<std::string>(), "Source", "CMakeLists"}, StringPool.EXTANSION_TEXT);


        m_CleanProjectCommand       = "mingw32-make -C \"" + m_ProjectBuildDirectory + "\" -k clean";
        m_ConfigureProjectCommand   = "cmake -G \"MinGW Makefiles\" -S \"" + m_ProjectSourceDirectory + "\" -B \"" + m_ProjectBuildDirectory + "\"";
        m_BuildProjectCommand       = "mingw32-make -C \"" + m_ProjectBuildDirectory + "\"";


        boost::algorithm::replace_all(project_id, StringPool.UNDERSCORE, StringPool.BLANK);

        m_ProjectLibraryFile = getPath({m_ProjectBuildDirectory, "libnerogame-" + project_id}, StringPool.EXTANSION_DLL);
        m_ProjectLibraryCopyFile = getPath({m_ProjectBuildDirectory, "libnerogame-" + project_id + "-copy"}, StringPool.EXTANSION_DLL);

        m_AdvancedScene = AdvancedScene::Ptr(new AdvancedScene());
    }

    void GameProject::loadProject()
    {

    }

    void GameProject::loadProjectLibrary()
    {

        nero_log(m_ProjectLibraryFile);

        if(!boost::dll::fs::exists(m_ProjectLibraryFile))
        {
            return;
        }


        if(boost::dll::fs::exists(m_ProjectLibraryCopyFile))
        {

            m_Scene = nullptr;
            m_CreateCppSceneFn.clear();

           boost::dll::fs::remove(m_ProjectLibraryCopyFile);
        }

        std::string boo = boost::replace_all_copy(m_ProjectLibraryFile, "/", "\\");
        boo = boost::replace_all_copy(boo, "\\", "\\\\");

        std::string zoo = boost::replace_all_copy(m_ProjectLibraryCopyFile, "/", "\\");
        zoo = boost::replace_all_copy(zoo, "\\", "\\\\");


        std::string copy_command = "copy \"" + boo + "\" \"" + zoo + "\"";

        nero_log(copy_command);

        system(copy_command.c_str());

        nero_log("copy done");


        nero_log(removeFileExtension(m_ProjectLibraryCopyFile));

            boost::dll::fs::path library_path(removeFileExtension(m_ProjectLibraryCopyFile));

            nero_log("loading scene library");

           m_CreateCppSceneFn = boost::dll::import_alias<CreateCppSceneFn>(library_path, "createScene", boost::dll::load_mode::append_decorations);

           nero_log("loading done");

           m_AdvancedScene->setScene(m_CreateCppSceneFn(Scene::Context()));
    }

    void GameProject::openEditor()
    {
        nero_log("cmake project : " + m_CmakeListFile);

        //get the list of qtcreator process
        std::string list_process    = "tasklist \/fo csv\| findstr \/i \"qtcreator\"";
        std::string processCSV      = exec(list_process.c_str());

        nero_log(processCSV);

        //If the Editor has been opened and are still available (has not been closed)
        if(m_EditorProcessId != StringPool.BLANK && processCSV.find(m_EditorProcessId) != std::string::npos)
        {
            //open it
            std::string cmd = "START \"\" qtcreator -pid " + m_EditorProcessId;
            nero_log(cmd);
            system(cmd.c_str());
        }
        else
        {
            //open a new process
            std::string open_editor = "START \"\" qtcreator \"" + m_CmakeListFile +"\"";
            system(open_editor.c_str());

            //save the process id
                 processCSV  = exec(list_process.c_str());
            auto processTab  = splitString(processCSV, '\r\n');

            nero_log(processTab.size());


            m_EditorProcessId = splitString(processTab.back(), ',').at(1);
            nero_log(m_EditorProcessId);

        }
    }

    std::string GameProject::exec(const char* cmd)
    {
        std::stringstream string_buffer;
        std::streambuf * cout_buffer = std::cout.rdbuf(string_buffer.rdbuf());

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

        std::cout.rdbuf(cout_buffer);

        return result;
    }

    void GameProject::compileProject()
    {
        m_ProjectCompilationStatus = 1;

        m_CleanProjectResult = system(m_CleanProjectCommand.c_str());

        m_ProjectCompilationStatus = 2;

        m_ConfigureProjectResult = system(m_ConfigureProjectCommand.c_str());

        m_ProjectCompilationStatus = 3;

        m_BuildProjectResult = system(m_BuildProjectCommand.c_str());

        m_ProjectCompilationStatus = 4;

    }

    AdvancedScene::Ptr GameProject::getAdvancedScene()
    {
        return m_AdvancedScene;
    }

}

