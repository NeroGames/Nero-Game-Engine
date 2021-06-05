////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef DATETIME_H
#define DATETIME_H
///////////////////////////HEADERS//////////////////////////
//Boost
#include <boost/date_time.hpp>
//Cpp
#include <string>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace datetime
	{
		const struct
		{
			const std::string DEFAULT = "%Y/%M/%D at %H:%M:%S";

		} DateFormat;

		boost::posix_time::ptime	getCurrentDateTime();
		std::string					formatDateTime(const boost::posix_time::ptime& time, const std::string& format = DateFormat.DEFAULT);
		std::wstring				toWideString(const std::string& input);
	}
}
#endif // DATETIME_H


