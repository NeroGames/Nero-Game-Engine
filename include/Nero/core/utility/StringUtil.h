////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef STRINGUTIL_H
#define STRINGUTIL_H
///////////////////////////HEADERS//////////////////////////
#include <string>
////////////////////////////////////////////////////////////

namespace nero
{
    struct _StringPool
    {
        const std::string BLANK = "";
        const std::string SPACE = " ";

        //file extension
        const std::string EXTANSION_JSON    = ".json";
        const std::string EXTANSION_TEXT    = ".txt";
        const std::string EXTANSION_LUA     = ".lua";

        //
        const std::string UNDERSCORE        = "_";
        const std::string SLASH             = "/";
        const std::string ANTI_SLASH        = "\\";

    };

    const _StringPool StringPool;

    enum class String_Format
    {
        DEFAULT,
    };

    std::string formatString(const std::string& input, String_Format format = String_Format::DEFAULT);
}

#endif // STRINGUTIL_H
