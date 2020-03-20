#include <Nero/core/cpp/utility/DateUtil.h>
#include <sstream>

namespace nero
{
	boost::posix_time::ptime getCurrentDateTime()
    {
        using namespace boost::posix_time;

        return second_clock::local_time();
    }

    std::string formatDateTime(const boost::posix_time::ptime& time, const std::string& format)
    {
        using namespace boost::posix_time;

        wtime_facet *facet = new wtime_facet(toWideString(format).c_str());

        std::stringstream stream;

        stream.imbue(std::locale(stream.getloc(), facet));
        stream << time;

        return stream.str();
    }

    std::wstring toWideString(const std::string& input)
    {
        std::wstring result;
        result.assign(input.begin(), input.end());

        return result;
    }

    /*// Get year Field from time object
    timeLocal.date().year();
    // Get Month Field from time Object
    timeLocal.date().month();
    // Get Day Field from time Object
    timeLocal.date().day();
    // Get Hours Field from time Object
    timeLocal.time_of_day().hours();
    // Get Minutes Field from time Object
    timeLocal.time_of_day().minutes();
    // Get Seconds Field from time Object
    timeLocal.time_of_day().seconds();*/

}
