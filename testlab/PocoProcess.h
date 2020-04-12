#ifndef POCOPROCESS_H
#define POCOPROCESS_H

#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include <fstream>
#include <Nero/core/cpp/utility/Logging.h>

using Poco::Process;
using Poco::ProcessHandle;

void pocoProcess()
{
	nero::logging::Logger::init();

	std::string cmd("g++");
	std::vector<std::string> args;
	args.push_back("-v");

	Poco::Pipe outPipe;
	ProcessHandle ph = Process::launch(cmd, args, 0, &outPipe, 0);
	Poco::PipeInputStream istr(outPipe);

	Poco::StreamCopier::copyStream(istr, nero::logging::Logger::getStringStream());
}


#endif // POCOPROCESS_H
