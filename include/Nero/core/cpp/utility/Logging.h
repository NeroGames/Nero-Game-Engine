////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LOGGING_H
#define LOGGING_H
///////////////////////////HEADERS//////////////////////////
// Poco
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/SplitterChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/StreamChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
// Nero
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/engine/Setting.h>
// Cpp
#include <string>
#include <sstream>
#include <vector>
/////////////////////////////TODO////////////////////////////
// TODO [Logging] manage logging settings
////////////////////////////////////////////////////////////
namespace nero
{
    ///\addtogroup Utility
    ///@{
    namespace logging
    {
        ///\brief List of log levels
        enum LEVEL
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

        ///\brief Log message wrapper, contains the level and the message logged
        struct Message
        {
            std::string message;
            LEVEL       level;
        };

        ///\brief The logger Settings
        struct SettingDocument
        {
        };

        ///\brief Static Class, Log message to Console, File and String
        ///
        /// Before using init() or init(setting) must be called once
        class Logger
        {
          public:
            static void init();
            static void init(const Setting& setting);
            static void log(const std::string& message, LEVEL level = LOG_INFO);
            static void logIf(const std::string& message, bool condition, LEVEL level = LOG_INFO);
            static std::string        getString();
            static void               clearStringStream();
            static std::stringstream& getStringStream();

          private:
            static void format(LEVEL level);

          private:
            Logger();

          private:
            static Poco::PatternFormatter*  m_PatternFormatter;
            static Poco::FormattingChannel* m_ConsoleFormating;
            static Poco::FormattingChannel* m_StringStreamFormating;
            static Poco::FormattingChannel* m_FileFormating;
            static std::stringstream        m_StringStream;
            static Setting                  m_LoggingSetting;
        };
    } // namespace logging
      ///@}
} // namespace nero
////////////////////////////MACRO///////////////////////////
#define EXPAND(x)                      x
#define nero_log_info(message)         nero::logging::Logger::log(nero::toString(message));
#define nero_log_level(message, level) nero::logging::Logger::log(nero::toString(message), level);
#define nero_log_info_if(message, condition)                                                       \
    nero::logging::Logger::logIf(nero::toString(message), condition);
#define nero_log_level_if(message, condition, level)                                               \
    nero::logging::Logger::logIf(nero::toString(message), condition, level);
#define GET_NERO_LOG_MACRO(_1, _2, NAME, ...)        NAME
#define _nero_log(...)                               GET_NERO_LOG_MACRO(__VA_ARGS__, nero_log_level, nero_log_info)(__VA_ARGS__)
#define GET_NERO_LOG_IF_MACRO(_1, _2, _3, NAME, ...) NAME
#define _nero_log_if(...)                                                                          \
    GET_NERO_LOG_IF_MACRO(__VA_ARGS__, nero_log_level_if, nero_log_info_if)(__VA_ARGS__)
#define nero_log(...)    EXPAND(_nero_log(__VA_ARGS__))
#define nero_log_if(...) EXPAND(_nero_log_if(__VA_ARGS__))
#define nero_s(value)    nero::toString(value)
#define nero_sp(value)   nero::toString(value) + nero::toString(" ")
#define nero_sv(value)   nero::toString(#value) + nero::toString(" = ") + nero::toString(value)
#define nero_sn(object)  nero::toString(object->toString())
////////////////////////////////////////////////////////////
#endif // LOGGING_H
