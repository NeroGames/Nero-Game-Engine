////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORSETTING_H
#define EDITORSETTING_H
///////////////////////////HEADERS///////////////////////////
//JSON
#include <json/json.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
	struct _EditorSetting
	{
		_EditorSetting()
		{
			//window setting
			windowSetting["width"]			= 1245;
			windowSetting["height"]			= 700;
			windowSetting["position_x"]		= 20;
			windowSetting["position_y"]		= 20;
			windowSetting["icon_file"]		= "icon.png";
		}

		nlohmann::json windowSetting;
	};

	const _EditorSetting EditorSetting;
}

#endif // EDITORSETTING_H
