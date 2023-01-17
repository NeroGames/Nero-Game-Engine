#include "Poco/Process.h"
#include <gtest/gtest.h>

#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include <fstream>
#include <Nero/core/cpp/utility/Logging.h>

using Poco::Process;
using Poco::ProcessHandle;

TEST(Poco, Process)
{
  nero::logging::Logger::init();

  std::string               cmd("g++");
  std::vector<std::string>  args{"-v"};

  Poco::Pipe    outPipe;
  ProcessHandle ProcessHandle = Process::launch(cmd, args, 0, &outPipe, 0);
  Poco::PipeInputStream iStream(outPipe);

  Poco::StreamCopier::copyStream(iStream, nero::logging::Logger::getStringStream());
}
