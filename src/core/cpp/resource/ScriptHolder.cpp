////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/resource/ScriptHolder.h>
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ScriptHolder::ScriptHolder()
	{
		m_Configuration = loadJson("setting/resource")["script"];
    }

    void ScriptHolder::load()
    {

    }

	void ScriptHolder::clear()
	{
		//clear parent
		ResourceHolder::clear();

		//clear current
		m_ScriptMap.clear();
		m_ScriptTable.clear();
	}
}

