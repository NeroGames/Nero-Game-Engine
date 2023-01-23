////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/core/cpp/utility/Platform.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/utility/String.h>
// Cpp
#include <iostream>
#include <sstream>
#include <unistd.h>
#ifdef NERO_OS_WINDOW
#include <shellapi.h>
#endif
////////////////////////////////////////////////////////////
namespace nero
{
    namespace cmd
    {
        Process::Process(Poco::ProcessHandle handle)
            : m_Handle(handle)
        {
            // emtpy
        }

        Poco::Pipe& Process::getOutPipe()
        {
            return m_OutPipe;
        }

        Poco::Pipe& Process::getErrorPipe()
        {
            return m_ErrorPipe;
        }

        unsigned int Process::getProcessId()
        {
            return m_Handle.id();
        }

        int Process::waitCompletion()
        {
            return m_Handle.wait();
        }

        int Process::getExistCode()
        {
            return m_ExitCode;
        }

        void Process::killProcess()
        {
            Poco::Process::kill(m_Handle);
        }

        Process runCommand(const std::string&              command,
                           const std::vector<std::string>& argument,
                           bool                            waitCompletion)
        {

            Poco::Pipe outPipe, errorPipe;
            Process    process(
                Poco::Process::launch(command, argument, nullptr, &outPipe, &errorPipe));

            Poco::PipeInputStream outStream(outPipe);
            Poco::PipeInputStream errorStream(errorPipe);
            // string stream
            Poco::StreamCopier::copyStream(outStream, nero::logging::Logger::getStringStream());
            Poco::StreamCopier::copyStream(errorStream, nero::logging::Logger::getStringStream());
            // standard output
            Poco::StreamCopier::copyStream(outStream, std::cout);
            Poco::StreamCopier::copyStream(errorStream, std::cout);
            // logging file

            process.m_OutPipe   = outPipe;
            process.m_ErrorPipe = errorPipe;

            if(waitCompletion)
            {
                process.m_ExitCode = process.m_Handle.wait();
            }
            else
            {
                process.m_ExitCode = 0; // NO_EXIT_CODE;
            }

            return process;
        }

        void killProcess(const unsigned int& processId)
        {
            Poco::Process::kill(processId);
        }

        std::string launchApplication(const std::string& path, const std::string& processName)
        {
#ifdef NERO_OS_WINDOW

            std::string command =
                LAUNCH_APPLICATION_WINDOWS + file::escapeSpace(file::getWindowsPath(path));
            system(command.c_str());

            return findProcessId(processName == StringPool.BLANK ? file::getFileName(path)
                                                                 : processName);

#elif NERO_OS_LINUX

            // TODO

#endif
        }

        void launchBrowser(const std::string& url)
        {
#ifdef NERO_OS_WINDOW

            ShellExecute(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

#elif NERO_OS_LINUX

            system(url.c_str());

#endif
        }

        std::string findProcessId(const std::string& application)
        {
#ifdef NERO_OS_WINDOW

            Poco::Pipe          outPipe;
            Poco::Pipe          outPipe2;
            Poco::ProcessHandle handle =
                Poco::Process::launch("tasklist", {"/fo", "csv"}, nullptr, &outPipe, &outPipe);
            Poco::ProcessHandle   handle2 = Poco::Process::launch("findstr",
                                                                  {"/i", application},
                                                                &outPipe,
                                                                &outPipe2,
                                                                &outPipe2);

            Poco::PipeInputStream outStream(outPipe2);
            std::stringstream     stringStream;
            Poco::StreamCopier::copyStream(outStream, stringStream);

            auto        processTableSplit = string::splitString(stringStream.str(), '\r\n');

            std::string result = string::splitString(processTableSplit.back(), ',').at(1);
            result.pop_back();
            result.erase(result.begin());

            return result;

#elif NERO_OS_LINUX

            // TODO

#endif
        }

        bool processRunning(const std::string& processId)
        {
            if(processId == StringPool.BLANK)
                return false;

#ifdef NERO_OS_WINDOW

            Poco::Pipe          outPipe;
            Poco::ProcessHandle handle =
                Poco::Process::launch("tasklist", {"/fo", "csv"}, nullptr, &outPipe, &outPipe);

            Poco::PipeInputStream outStream(outPipe);
            std::stringstream     stringStream;
            Poco::StreamCopier::copyStream(outStream, stringStream);
            handle.wait();

            return stringStream.str().find(processId) != std::string::npos;

#elif NERO_OS_LINUX

                // TODO

#endif
        }

        void setEnvironmnentVariable(const std::string& name, const std::string& value)
        {
#ifdef NERO_OS_WINDOW

            Poco::Pipe          outPipe;
            Poco::ProcessHandle handle =
                Poco::Process::launch("setx", {name, value}, nullptr, &outPipe, &outPipe);
            Poco::PipeInputStream outStream(outPipe);
            Poco::StreamCopier::copyStream(outStream, nero::logging::Logger::getStringStream());

#elif NERO_OS_LINUX

            // TODO

#endif
        }

        void showApplication(const std::string& name)
        {
            std::string NERO_GAME_HOME =
                getenv("NERO_GAME_HOME") ? std::string(getenv("NERO_GAME_HOME")) : StringPool.BLANK;

            std::string windowmode = file::escapeSpace(
                file::getWindowsPath(NERO_GAME_HOME + "/Tools/Script/windowmode.bat"));
            std::string sendkeys = file::escapeSpace(
                file::getWindowsPath(NERO_GAME_HOME + "/Tools/Script/sendkeys.bat"));

            // std::string cmd1 = windowmode + " -pid " + proccessId + " -mode restore";
            std::string cmd1 = "call " + windowmode + " -title " + name + " -mode maximized";
            std::string cmd2 = "call " + sendkeys + " " + name + "  \"\"";

            // cmd::runCommand("call", {windowmode, "-pid", proccessId, "-mode", "restore"}, false);
            // usleep(100);
            // cmd::runCommand("call", {sendkeys, " ", name, "\"\""}, false);

            // WinExec(cmd1.c_str(), SW_HIDE);
            // usleep(100);
            // WinExec(cmd2.c_str(), SW_HIDE);

            system(cmd1.c_str());
            usleep(100);
            system(cmd2.c_str());
        }

    } // namespace cmd
} // namespace nero
