#include <Nero/core/utility/StringUtil.h>
#include <boost/algorithm/string.hpp>
#include <fstream>

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

    void fillCharTable(const char** charTab, const std::vector<std::string>& stringTab)
    {
        for(std::size_t i = 0; i < stringTab.size(); i++)
        {
            charTab[i] = const_cast<const char*>(stringTab[i].c_str());
        }
    };

    void fillCharArray(char* charArray, int arraySize, const std::string& string)
    {
        strncpy(charArray, string.c_str(), arraySize - 1);
        charArray[arraySize - 1] = 0;
    }

    std::vector<std::string> getWordTable(std::string string)
    {
        boost::algorithm::trim_left(string);
        boost::algorithm::trim_right(string);

        auto result = splitString(string, ' ');

        std::remove_if(result.begin(), result.end(), [](std::string word){return word == StringPool.BLANK;});

        return result;
    }
}
