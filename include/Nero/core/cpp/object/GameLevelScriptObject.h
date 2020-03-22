////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMELEVELSCRIPTOBJECT_H
#define GAMELEVELSCRIPTOBJECT_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/object/ScriptObject.h>
#include <Nero/core/cpp/object/GameLevelObject.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	class GameLevelScriptObject : public ScriptObject
	{
		public:
			GameLevelScriptObject(ScriptObject::Context context);
	};

}


#endif // GAMELEVELSCRIPTOBJECT_H
