////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMELEVELSCRIPTOBJECT_H
#define GAMELEVELSCRIPTOBJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
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

} // namespace nero

#endif // GAMELEVELSCRIPTOBJECT_H
