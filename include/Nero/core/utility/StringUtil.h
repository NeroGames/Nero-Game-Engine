////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef STRINGUTIL_H
#define STRINGUTIL_H
///////////////////////////HEADERS//////////////////////////
#include <string>
#include <sstream>
#include <vector>
#include <functional>
////////////////////////////////////////////////////////////

namespace nero
{
    struct _StringPool
    {
		const std::string BLANK = "";
        const std::string SPACE = " ";
		const std::string NEW_LINE = "\n";


        //file extension
		const std::string EXTENSION_JSON			= ".json";
		const std::string EXTENSION_TEXT			= ".txt";
		const std::string EXTENSION_LUA				= ".lua";
		const std::string EXTENSION_DLL				= ".dll";
		const std::string EXTENSION_CPP_HEADER		= ".h";
		const std::string EXTENSION_CPP_SOURCE		= ".cpp";
		const std::string EXTENSION_NERO			= ".nero";
		const std::string EXTENSION_INI				= ".ini";

        //
		const std::string UNDERSCORE				= "_";
		const std::string SLASH						= "/";
		const std::string ANTI_SLASH				= "\\";

		//suffix
		const std::string SUFFIX_SETTING			= "_setting";

    };

    const _StringPool StringPool;

    enum class String_Format
    {
        DEFAULT,
    };

    std::string formatString(const std::string& input, String_Format format = String_Format::DEFAULT);

    template <typename T>
    std::string         toString(T const& value);

    std::vector<std::string> &splitString(const std::string &s, char delim,std::vector<std::string> &elems);
    std::vector<std::string> splitString(const std::string &s, char delim);



    template <typename T>
    std::string toString(T const& value)
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

   void fillCharTable(const char** charTab, const std::vector<std::string>& stringTab);
   void fillCharArray(char* charArray, int arraySize, const std::string& string);

   std::vector<std::string> getWordTable(std::string string);

   std::string wrapString(const std::string& message, int maxLetter);
}

#endif // STRINGUTIL_H
