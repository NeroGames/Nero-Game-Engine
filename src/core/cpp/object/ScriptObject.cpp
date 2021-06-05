////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/object/ScriptObject.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	ScriptObject::ScriptObject(Context context):
		 Object ()
		,m_ScriptContext(context)
	{

	}

	void ScriptObject::setTargetObject(Object::Ptr	target)
	{
		m_TargetObject = target;
	}

	Object::Ptr ScriptObject::getTargetObject()
	{
		return m_TargetObject;
	}
}

