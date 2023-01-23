////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMESCREENSCRIPTOBJECT_H
#define GAMESCREENSCRIPTOBJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/object/ScriptObject.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GameScreenScriptObject : public ScriptObject
    {
      public:
        GameScreenScriptObject(ScriptObject::Context context);
    };
} // namespace nero

#endif // GAMESCREENSCRIPTOBJECT_H
