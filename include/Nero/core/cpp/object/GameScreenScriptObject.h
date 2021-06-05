////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMESCREENSCRIPTOBJECT_H
#define GAMESCREENSCRIPTOBJECT_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/object/ScriptObject.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	class GameScreenScriptObject : public ScriptObject
	{
		public:
			GameScreenScriptObject(ScriptObject::Context context);
	};
}

#endif // GAMESCREENSCRIPTOBJECT_H
