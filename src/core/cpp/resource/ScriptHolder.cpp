////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ScriptHolder.h>
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ScriptHolder::ScriptHolder()
    {
        m_Configuration = file::loadJson("setting/resource")["script"];
    }

    void ScriptHolder::load()
    {
    }

    void ScriptHolder::clear()
    {
        // clear parent
        ResourceHolder::clear();

        // clear current
        m_ScriptMap.clear();
        m_ScriptTable.clear();
    }
} // namespace nero
