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
		const int				NO_EXIT_CODE			= -9999999; //TODO
		const std::string		KILL_PROCESS_WINDOWS	= "taskkill /F /PID ";
		const std::string		KILL_PROCESS_LINUX		= "kill -9 ";

		class Process
		{
			public:
				Poco::Pipe&				getOutPipe();
				Poco::Pipe&				getErrorPipe();
				unsigned int			getProcessId();
				int						getExistCode();
				int						waitCompletion();
				bool					killProcess();

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
		void							launchApplication(const std::string& program);
		void							launchBrowser(const std::string& url);
		bool							killProcess(const unsigned int& processId);
	}
}
#endif // COMMANDLINE_H
