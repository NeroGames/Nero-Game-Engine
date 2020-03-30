////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
#include <Nero/core/cpp/utility/Logging.h>
////////////////////////////////////////////////////////////
namespace nero
{
	Poco::PatternFormatter*		Logger::m_PatternFormatter				= new Poco::PatternFormatter("%s: %t");
	Poco::FormattingChannel*	Logger::m_LoggingConsoleFormating		= new Poco::FormattingChannel(Logger::m_PatternFormatter);
	Poco::FormattingChannel*	Logger::m_LoggingStringStreamFormating	= new Poco::FormattingChannel(Logger::m_PatternFormatter);
	Poco::FormattingChannel*	Logger::m_LoggingFileFormating			= new Poco::FormattingChannel(Logger::m_PatternFormatter);
	std::stringstream			Logger::m_LoggingStringStream			= std::stringstream();
	Setting						Logger::m_LoggingSetting				= Setting();

	Logger::Logger()
	{
		//empty
	}

	void Logger::init()
	{
		//setup setting
		/*if(m_LoggingSetting.empty())
		{
			//build default lodding setting
			m_LoggingSetting.setString("enable_color", "true");
			m_LoggingSetting.setString("information_color", "black");
			m_LoggingSetting.setString("trace_color", "black");
			m_LoggingSetting.setString("enable_color", "black");
			etc ...
		}*/

		//initializing logging system
			//console channel
		Poco::AutoPtr<Poco::ColorConsoleChannel> console_channel(new Poco::ColorConsoleChannel);
		console_channel->setProperty("enableColors", "true");
		console_channel->setProperty("informationColor", "black");
		console_channel->setProperty("traceColor", "black");
		console_channel->setProperty("debugColor", "black");
		console_channel->setProperty("noticeColor", "black");
		console_channel->setProperty("warningColor", "black");
		console_channel->setProperty("errorColor", "black");
		console_channel->setProperty("criticalColor", "black");
		console_channel->setProperty("fatalColor", "black");
			//string stream channel
		Poco::AutoPtr<Poco::StreamChannel> string_channel(new Poco::StreamChannel(m_LoggingStringStream));
			//file chinnel
		Poco::AutoPtr<Poco::FileChannel> file_channel(new Poco::FileChannel);
		file_channel->setProperty("path", "logging/nero_game_engine.log");
		file_channel->setProperty("rotation", "1 days");
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

	void Logger::init(const Setting& loggingSetting)
	{
		//save settings
		m_LoggingSetting = loggingSetting;

		//initialize logging
		init();
	}

	void Logger::format(LOGLEVEL level)
	{
		switch (level)
		{
			case LOG_INFO:
			{
				m_PatternFormatter->setProperty("pattern", "%s: %t");
			}break;

			/*case LOG_DEBUG:
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
			}break;*/

			default:
			{
				m_PatternFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t");
			}
		}
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
	}

	void Logger::logIf(const std::string& message, bool condition, LOGLEVEL level)
	{
		if(condition)
		{
			log(message, level);
		}
	}

	std::string Logger::getString()
	{
		return m_LoggingStringStream.str();
	}
	void Logger::clearStringStream()
	{
		m_LoggingStringStream.str(StringPool.BLANK);
	}
}
