////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/editor/project/GameProject.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/utility/DateTime.h>
// Boost
#include <boost/dll.hpp>
#include <boost/algorithm/string.hpp>
// Std
#include <thread>
/////////////////////////////////////////////////////////////

namespace nero
{
    GameProject::GameProject(Setting::Ptr projectSetting)
        : m_ProjectSetting(std::move(projectSetting))
    {
    }

    void GameProject::init()
    {
        m_ProjectSetting->setString(
            "source_directory",
            file::getPath(
                {getProjectDirectory(), "Source", m_ProjectSetting->getString("project_name")}));
        m_ProjectSetting->setString("build_directory",
                                    file::getPath({getProjectDirectory(), "Build"}));
        m_ProjectSetting->setString(
            "cmake_file",
            file::getPath({m_ProjectSetting->getString("source_directory"), "CMakeLists"},
                          StringPool.EXT_TEXT));

        std::string library     = string::formatString(m_ProjectSetting->getString("project_name"),
                                                   string::Format::COMPACT_LOWER);
        std::string libraryFile = file::getPath(
            {m_ProjectSetting->getString("build_directory"), "libnerogame-" + library},
            StringPool.EXT_DLL);
        std::string libraryFileCopy = file::getPath(
            {m_ProjectSetting->getString("build_directory"), "libnerogame-" + library + "-copy"},
            StringPool.EXT_DLL);

        m_ProjectSetting->setString("library_file", libraryFile);
        m_ProjectSetting->setString("library_file_copy", libraryFileCopy);

        // create advanced scene
        m_AdvancedScene = std::make_shared<AdvancedScene>(m_ProjectSetting);
    }

    bool GameProject::loadLibrary()
    {
        const std::string libraryFile     = m_ProjectSetting->getString("library_file");
        const std::string libraryFileCopy = m_ProjectSetting->getString("library_file_copy");

        if(!file::fileExist(libraryFile))
        {
            nero_log("No library to load");
            return false;
        }

        if(file::fileExist(libraryFileCopy))
        {
            m_AdvancedScene->clearLoadedObject();

            file::removeFile(libraryFileCopy);
        }

        file::copyFile(libraryFile, libraryFileCopy);

        boost::dll::fs::path libraryFilePath(file::removeFileExtension(libraryFileCopy));

        try
        {
            boost::dll::shared_library sharedLibrary(libraryFileCopy);

            if(!sharedLibrary.is_loaded())
                return false;

            nero_log("Project DLL Loaded");

            AdvancedScene::CreateCppGameSceneCallback createSceneCallback =
                boost::dll::import_alias<AdvancedScene::CreateCppGameScene>(
                    libraryFilePath,
                    "createScene",
                    boost::dll::load_mode::append_decorations);

            if(createSceneCallback.empty())
                return false;

            m_AdvancedScene->setCreateSceneCallback(createSceneCallback);
            createSceneCallback.clear();

            nero_log("Scene Class Loaded");

            // Load Game Level
            for(const std::string levelName : m_AdvancedScene->getRegisteredLevelTable())
            {
                const std::string createFunctionName =
                    "create" + string::formatString(levelName, string::Format::CAMEL_CASE_UPPER) +
                    "GameLevel";

                AdvancedScene::CreateCppGameLevelCallback createCppGameLevelCallback =
                    boost::dll::import_alias<AdvancedScene::CreateCppGameLevel>(
                        libraryFilePath,
                        createFunctionName,
                        boost::dll::load_mode::append_decorations);

                m_AdvancedScene->registerCreateLevelCallback(levelName, createCppGameLevelCallback);
                createCppGameLevelCallback.clear();

                nero_log("Game Level Class Loaded - " + levelName);
            }

            // TODO Loag Game Screen

            nero_log("DLL loaded successfully");
        }
        catch(std::exception e)
        {
            nero_log("Failed to load DLL");
            return false;
        }

        return true;
    }

    void GameProject::openEditor()
    {
        std::string codeEditor = m_ProjectSetting->getString("code_editor");

        if(codeEditor == "Qt Creator")
        {
            openQtCreator();
        }
        else if(codeEditor == "Visual Studio")
        {
            openVisualStudio();
        }
    }

    void GameProject::openQtCreator(const std::string& file)
    {
        char*             qtCreator  = getenv("NERO_GAME_QT");
        const std::string QT_CREATOR = qtCreator ? std::string(qtCreator) : StringPool.BLANK;

        if(QT_CREATOR == StringPool.BLANK)
            return;

        if(cmd::processRunning(m_CodeEditorProcessId))
        {
            std::string cmd = "START \"\" " + file::escapeSpace(file::getWindowsPath(QT_CREATOR)) +
                              " -pid " + m_CodeEditorProcessId;
            system(cmd.c_str());
        }
        else
        {

            std::string qt_creator = file::escapeSpace(file::getWindowsPath(QT_CREATOR));
            std::string cmake_file = file::escapeSpace(m_ProjectSetting->getString("cmake_file"));
            std::string cmd        = "START /B \"\" " + qt_creator + " " + cmake_file;
            system(cmd.c_str());

            m_CodeEditorProcessId = cmd::findProcessId("qtcreator");

            usleep(100);
            cmd::showApplication(m_ProjectSetting->getString("project_name"));
        }
    }

    void GameProject::openVisualStudio(const std::string& file)
    {
        char*             visualStudio = getenv("NERO_GAME_VS");
        const std::string VISUAL_STUDIO =
            visualStudio ? std::string(visualStudio) : StringPool.BLANK;

        if(VISUAL_STUDIO == StringPool.BLANK)
            return;

        if(file == StringPool.BLANK)
        {
            if(cmd::processRunning(m_CodeEditorProcessId))
            {
                cmd::showApplication("devenv");
            }
            else
            {
                std::string cmd =
                    "START \"\" " + file::escapeSpace(file::getWindowsPath(VISUAL_STUDIO));
                cmd += " \"" +
                       file::getWindowsPath(m_ProjectSetting->getString("source_directory")) +
                       "\"" + " /Edit";
                system(cmd.c_str());

                m_CodeEditorProcessId = cmd::findProcessId("devenv");
            }
        }
        else
        {
            std::string cmd = "START \"\" " +
                              file::escapeSpace(file::getWindowsPath(VISUAL_STUDIO)) + " \"" +
                              file + "\" " + " /Edit";
            system(cmd.c_str());
        }
    }

    void GameProject::compileProject(const std::string&        projectDirectory,
                                     const BackgroundTask::Ptr backgroundTask)
    {
        // TODO : prevent multiple compilation at the same time

        Parameter parameter;
        parameter.loadJson(file::loadJson(file::getPath({projectDirectory, ".project"}), true));
        const std::string projectName    = parameter.getString("project_name");
        const std::string sourcePath     = file::getPath({projectDirectory, "Source", projectName});
        const std::string buildPath      = file::getPath({projectDirectory, "Build"});

        char*             home           = getenv("NERO_GAME_HOME");
        const std::string NERO_GAME_HOME = home ? std::string(home) : StringPool.BLANK;

        if(NERO_GAME_HOME == StringPool.BLANK)
        {
            // TODO error, stop compilation process
        }

        const std::string mingw32 = NERO_GAME_HOME + "/Compiler/bin/mingw32-make.exe";
        const std::string gxx     = NERO_GAME_HOME + "/Compiler/bin/g++.exe";
        const std::string gcc     = NERO_GAME_HOME + "/Compiler/bin/gcc.exe";
        const std::string cmake   = NERO_GAME_HOME + "/Tools/Cmake/bin/cmake.exe";

        backgroundTask->addMessage("Compiling Project - " + projectName);
        nero_log("compiling project " + projectName);

        backgroundTask->nextStep();
        backgroundTask->addMessage("Step 1/3 - Cleaning Project");
        cmd::Process cleanProcess = cmd::runCommand(mingw32, {"-C", buildPath, "-k", "clean"});
        backgroundTask->setErrorCode(cleanProcess.getExistCode());
        nero_log("clean project exit code = " + toString(cleanProcess.getExistCode()));

        backgroundTask->nextStep();
        backgroundTask->addMessage("Step 2/3 - Configuring Project");
        cmd::Process configProcess =
            cmd::runCommand(cmake,
                            {"-G",
                             "MinGW Makefiles",
                             "-S",
                             sourcePath,
                             "-B",
                             buildPath,
                             "-D",
                             "CMAKE_CXX_COMPILER=" + file::getPath(gxx),
                             "-D",
                             "CMAKE_C_COMPILER=" + file::getPath(gcc),
                             "-D",
                             "CMAKE_MAKE_PROGRAM=" + file::getPath(mingw32)});
        backgroundTask->setErrorCode(configProcess.getExistCode());
        nero_log("configure project exit code = " + toString(configProcess.getExistCode()));

        backgroundTask->nextStep();
        backgroundTask->addMessage("Step 3/3 - Building Project");
        cmd::Process buildProcess = cmd::runCommand(mingw32, {"-C", buildPath});
        backgroundTask->setErrorCode(buildProcess.getExistCode());
        nero_log("build project exit code = " + toString(buildProcess.getExistCode()));

        backgroundTask->nextStep();
        backgroundTask->addMessage("Finished Compiling Project - " + projectName);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        backgroundTask->setCompleted(true);
    }

    AdvancedScene::Ptr GameProject::getAdvancedScene()
    {
        return m_AdvancedScene;
    }

    std::string GameProject::getProjectName() const
    {
        return m_ProjectSetting->getString("project_name");
    }

    std::string GameProject::getProjectDirectory() const
    {
        return m_ProjectSetting->getString("project_directory");
    }

    void GameProject::closeProject()
    {
        if(cmd::processRunning(m_CodeEditorProcessId))
        {
            std::string cmd = "taskkill /F /PID " + m_CodeEditorProcessId;
            system(cmd.c_str());
        }

        m_AdvancedScene->clearLoadedObject();
    }
} // namespace nero
