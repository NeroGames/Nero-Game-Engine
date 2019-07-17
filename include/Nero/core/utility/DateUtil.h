#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <boost/date_time.hpp>
#include <string>

namespace nero
{
    struct _DateFormat
    {
        const std::string DEFAULT = "%Y/%M/%D at %H:%M:%S";
    };

    const _DateFormat DateFormat;

   boost::posix_time::ptime getCurrentDateTime();

   std::string formatDate();
   std::string formatDateTime(const boost::posix_time::ptime& time, const std::string& format = DateFormat.DEFAULT);

   std::wstring toWideString(const std::string& input);


}

#endif // DATEUTIL_H


