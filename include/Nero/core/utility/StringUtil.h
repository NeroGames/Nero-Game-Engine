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

        //file extension
        const std::string EXTANSION_JSON    = ".json";
        const std::string EXTANSION_TEXT    = ".txt";
        const std::string EXTANSION_LUA     = ".lua";

    };

    const _StringPool StringPool;
}

#endif // STRINGUTIL_H
