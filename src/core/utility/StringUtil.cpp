#include <Nero/core/utility/StringUtil.h>
#include <boost/algorithm/string.hpp>
namespace nero
{
    std::string formatString(const std::string& input, String_Format format)
    {
        using namespace boost::algorithm;

        std::string result = input;

        switch (format)
        {
            case String_Format::DEFAULT:
            {
                trim_left(result);
                trim_right(result);
                to_lower(result);
                replace_all(result, StringPool.SPACE, StringPool.UNDERSCORE);

            }break;
        }

        return result;
    }

    std::vector<std::string> &splitString(const std::string &s, char delim,std::vector<std::string> &elems)
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            if (item.length() > 0)
            {
                elems.push_back(item);
            }
        }

        return elems;
    }


    std::vector<std::string> splitString(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        splitString(s, delim, elems);

        return elems;
    }

}
