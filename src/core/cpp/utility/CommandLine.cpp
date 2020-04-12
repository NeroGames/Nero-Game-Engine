////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/core/cpp/utility/Platform.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/utility/String.h>
//Cpp
#include <iostream>
#ifdef NERO_OS_WINDOW
#include <shellapi.h>
#endif
////////////////////////////////////////////////////////////
namespace nero
{
	std::string formatPathWindows(const std::string& path)
	{
		std::string result = string::StringPool.BLANK;

		std::vector<std::string> table = string::splitString(path, '\\');
		for(std::string word : table)
		{
			if(string::splitString(word, ' ').size() > 1)
				result += "\"" + word + "\"\\";
			else
				result += word + "\\";
		}

		result.pop_back();

		return result;
	}
}
namespace nero
{
	namespace cmd
	{
		Process::Process(Poco::ProcessHandle handle):
			m_Handle(handle)
		{
			//emtpy
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

		bool Process::killProcess()
		{
			return nero::cmd::killProcess(m_Handle.id());
		}

		Process runCommand(const std::string& command, const std::vector<std::string>& argument, bool waitCompletion)
		{

			Poco::Pipe outPipe, errorPipe;
			Process process(Poco::Process::launch(command, argument, nullptr, &outPipe, &errorPipe));

			Poco::PipeInputStream outStream(outPipe);
			Poco::PipeInputStream errorStream(errorPipe);
			//string stream
			Poco::StreamCopier::copyStream(outStream, nero::logging::Logger::getStringStream());
			Poco::StreamCopier::copyStream(errorStream, nero::logging::Logger::getStringStream());
			//standard output
			Poco::StreamCopier::copyStream(outStream, std::cout);
			Poco::StreamCopier::copyStream(errorStream, std::cout);
			//logging file

			process.m_OutPipe	= outPipe;
			process.m_ErrorPipe = errorPipe;

			if(waitCompletion)
			{
				process.m_ExitCode = process.m_Handle.wait();
			}
			else
			{
				process.m_ExitCode = NO_EXIT_CODE;
			}


			return process;
		}

		bool killProcess(const unsigned int& processId)
		{
			#ifdef NERO_OS_WINDOW
				runCommand(KILL_PROCESS_WINDOWS, {string::toString(processId)});
				return true; //TODO

			#elif NERO_OS_LINUX
				runCommand(KILL_PROCESS_LINUX, {string::toString(processId)});
				return true //TODO

			#endif
		}

		void launchApplication(const std::string& application)
		{
			#ifdef NERO_OS_WINDOW
				//nero_log(formatPathWindows(file::getWindowsPath(application)))
				Poco::Pipe outPipe, errorPipe;
				std::string cmd = "START /B C:\\Users\\sk-landry\\Desktop\\Origin\\Project\\\"Nero Game Engine\"\\Tools\\Profiler\\profiler_gui.exe";
				try {
					nero_log(cmd);
					//Poco::Process::launch("start", {"/B", cmd});
					system(cmd.c_str());

				} catch (Poco::SystemException e) {
					nero_log(e.message());
				}
				//return runCommand(formatPathWindows(file::getWindowsPath(application)), {}, false);

			#elif NERO_OS_LINUX
				//TODO

			#endif
		}

		void launchBrowser(const std::string& url)
		{
			#ifdef NERO_OS_WINDOW
				ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
			#elif NERO_OS_LINUX
				system(url.c_str());
			#endif // _WIN32
		}
	}
}
