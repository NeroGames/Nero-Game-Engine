////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef COMMANDLINE_H
#define COMMANDLINE_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/Logging.h>
//Poco
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
//Cpp
#include <fstream>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace cmd
	{
		const int				NO_EXIT_CODE					= -9999999; //TODO
		const std::string		KILL_PROCESS_WINDOWS			= "taskkill /F /PID ";
		const std::string		KILL_PROCESS_LINUX				= "kill -9 ";
		const std::string		LAUNCH_APPLICATION_WINDOWS		= "START /B ";
		const std::string		FILTER_PROCESS_WINDOWS			= "START /B tasklist /fo csv | findstr /i ";

		class Process
		{
			public:
				Poco::Pipe&				getOutPipe();
				Poco::Pipe&				getErrorPipe();
				unsigned int			getProcessId();
				int						getExistCode();
				int						waitCompletion();
				void					killProcess();

			private:
										Process(Poco::ProcessHandle handle);
				friend Process			runCommand(const std::string& command, const std::vector<std::string>& argument, bool waitCompletion);

			private:
				Poco::Pipe				m_OutPipe;
				Poco::Pipe				m_ErrorPipe;
				Poco::ProcessHandle		m_Handle;
				int						m_ExitCode;
		};

		Process							runCommand(const std::string& command, const std::vector<std::string>& argument = {}, bool waitCompletion = true);
		std::string						launchApplication(const std::string& path, const std::string& processName = StringPool.BLANK);
		void							launchBrowser(const std::string& url);
		void							killProcess(const unsigned int& processId);
		std::string						findProcessId(const std::string& application);
		bool							processRunning(const std::string& processId);
	}
}
#endif // COMMANDLINE_H
