////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
#include <Nero/core/cpp/utility/Logging.h>
////////////////////////////////////////////////////////////
namespace nero
{
	Poco::FormattingChannel* Logger::m_LoggingConsoleFormating		= new Poco::FormattingChannel(new Poco::PatternFormatter("%s: %p: %P:  %N: %u:  %t"));
	Poco::FormattingChannel* Logger::m_LoggingStringStreamFormating = new Poco::FormattingChannel(new Poco::PatternFormatter("%s: %p: %P:  %N: %u:  %t"));
	Poco::FormattingChannel* Logger::m_LoggingFileFormating			= new Poco::FormattingChannel(new  Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
	std::stringstream		 Logger::m_LoggingStringStream			= std::stringstream();
	Setting					 Logger::m_LoggingSetting				= Setting();
	//std::vector<LogMessage>  Logger::m_LoggingMessageTable			= std::vector<LogMessage>();


	Logger::Logger()
	{
		//empty
	}

	void Logger::init()
	{
		//initializing logging system
			//console channel
		Poco::AutoPtr<Poco::ConsoleChannel> console_channel(new Poco::ConsoleChannel);
			//string stream channel
		Poco::AutoPtr<Poco::StreamChannel> string_channel(new Poco::StreamChannel(m_LoggingStringStream));
			//file chinnel
		Poco::AutoPtr<Poco::FileChannel> file_channel(new Poco::FileChannel);
		file_channel->setProperty("path", "logging/nero_game_engine.log");
		file_channel->setProperty("rotation", "2 K");
		file_channel->setProperty("archive", "timestamp");
			//format channel
				//console channel
		m_LoggingConsoleFormating->setChannel(console_channel);
		m_LoggingConsoleFormating->open();
				//string stream channel
		m_LoggingStringStreamFormating->setChannel(string_channel);
		m_LoggingStringStreamFormating->open();
				//file channel
		m_LoggingFileFormating->setChannel(file_channel);
		m_LoggingFileFormating->open();
			//splitter channel
		Poco::AutoPtr<Poco::SplitterChannel> spliter_channel(new Poco::SplitterChannel);
			spliter_channel->addChannel(m_LoggingConsoleFormating);
			spliter_channel->addChannel(m_LoggingStringStreamFormating);
			spliter_channel->addChannel(m_LoggingFileFormating);
			//root logger
		Poco::Logger::root().setChannel(spliter_channel);
	}

	void Logger::init(const Setting& logSetting)
	{
		//TODO
	}

	void Logger::format(LOGLEVEL level)
	{
		//TODO

		/*if(!m_LoggingSetting.empty())
		{
			std::string pattern = m_LoggingSetting.getString("formating_" + std::to_string(int(level)));
			m_LoggingFileFormating->setProperty("pattern", pattern);
			m_LoggingStringStreamFormating->setProperty("pattern", pattern);
			m_LoggingFileFormating->setProperty("pattern", pattern);
		}*/
	}

	void Logger::log(const std::string& message, LOGLEVEL level)
	{
		//format output
		format(level);

		//log
		switch (level)
		{
			case LOG_INFO:
			{
				Poco::Logger::get("[Nero]").information(message);
			}break;

			case LOG_DEBUG:
			{
				Poco::Logger::get("[Nero]").debug(message);
			}break;

			case LOG_TRACE:
			{
				Poco::Logger::get("[Nero]").trace(message);
			}break;

			case LOG_WARNING:
			{
				Poco::Logger::get("[Nero]").warning(message);
			}break;

			case LOG_ERROR:
			{
				Poco::Logger::get("[Nero]").error(message);
			}break;

			case LOG_NOTICE:
			{
				Poco::Logger::get("[Nero]").notice(message);
			}break;

			case LOG_CRITICAL:
			{
				Poco::Logger::get("[Nero]").critical(message);
			}break;

			case LOG_FATAL:
			{
				Poco::Logger::get("[Nero]").fatal(message);
			}break;
		}

		//save log
		//saveLog(level);
	}

	void Logger::logIf(const std::string& message, bool condition, LOGLEVEL level)
	{
		if(condition)
		{
			log(message, level);
		}
	}

	/*void Logger::saveLog(LOGLEVEL level)
	{
		m_LoggingMessageTable.push_back(LogMessage{m_LoggingStringStream.str(), level});
		m_LoggingStringStream.str(StringPool.BLANK);
	}

	const std::vector<LogMessage>& Logger::getMessageTable()
	{
		return m_LoggingMessageTable;
	}

	void Logger::clearMessageTable()
	{
		m_LoggingMessageTable.clear();
	}*/

	std::string Logger::getString()
	{
		return m_LoggingStringStream.str();
	}
	void Logger::clearStringStream()
	{
		m_LoggingStringStream.str(StringPool.BLANK);
	}

}
