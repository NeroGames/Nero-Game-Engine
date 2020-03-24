////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LOGGING_H
#define LOGGING_H
///////////////////////////HEADERS//////////////////////////
//Poco
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/SplitterChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/StreamChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
//Nero
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/utility/StringUtil.h>
//Cpp
#include <string>
#include <sstream>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
	enum LOGLEVEL
	{
		LOG_FATAL,
		LOG_CRITICAL,
		LOG_ERROR,
		LOG_WARNING,
		LOG_NOTICE,
		LOG_INFO,
		LOG_DEBUG,
		LOG_TRACE
	};

	struct LogMessage
	{
		std::string message;
		LOGLEVEL	level;
	};

	class Logger
	{
		public:
			static void			init();
			static void			init(const Setting& logSetting);
			static void			log(const std::string& message, LOGLEVEL level = LOG_INFO);
			static void			logIf(const std::string& message, bool condition, LOGLEVEL level = LOG_INFO);

			//string stream logging
			//static void								clearMessageTable();
			//static const std::vector<LogMessage>&	getMessageTable();
			static std::string		getString();
			static void				clearStringStream();

		private:
			static void			format(LOGLEVEL level);
			//static void			saveLog(LOGLEVEL level);

		private:
			static Poco::FormattingChannel*	m_LoggingConsoleFormating;
			static Poco::FormattingChannel*	m_LoggingStringStreamFormating;
			static Poco::FormattingChannel*	m_LoggingFileFormating;
			static std::stringstream		m_LoggingStringStream;
			static Setting					m_LoggingSetting;
			//static std::vector<LogMessage>	m_LoggingMessageTable;

		private:
			Logger();
	};
}
////////////////////////////MACRO///////////////////////////
#define EXPAND(x) x
#define nero_log_info(message) nero::Logger::log(nero::toString(message));
#define nero_log_level(message, level) nero::Logger::log(nero::toString(message), level);
#define nero_log_info_if(message, condition) nero::Logger::logIf(nero::toString(message), condition);
#define nero_log_level_if(message, condition, level) nero::Logger::logIf(nero::toString(message), condition, level);
#define GET_NERO_LOG_MACRO(_1,_2,NAME,...) NAME
#define _nero_log(...) GET_NERO_LOG_MACRO(__VA_ARGS__, nero_log_level, nero_log_info)(__VA_ARGS__)
#define GET_NERO_LOG_IF_MACRO(_1,_2,_3,NAME,...) NAME
#define _nero_log_if(...) GET_NERO_LOG_IF_MACRO(__VA_ARGS__, nero_log_level_if, nero_log_info_if)(__VA_ARGS__)
#define nero_log(...) EXPAND(_nero_log(__VA_ARGS__))
#define nero_log_if(...) EXPAND(_nero_log_if(__VA_ARGS__))
#define nero_ss(value) nero::toString(value)
#define nero_ssp(value) nero::toString(value) + nero::toString(" ")
#define nero_ssv(value) nero::toString(#value) + nero::toString(" = ") + nero::toString(value)
#define nero_ssn(object)  nero::toString(object->toString())
////////////////////////////////////////////////////////////

#endif // LOGGING_H
