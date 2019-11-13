////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
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
			windowSetting["window_width"]		= 1245;
			windowSetting["window_height"]		= 700;
			windowSetting["window_position_x"]	= 20;
			windowSetting["window_position_y"]	= 20;
			windowSetting["window_icon"]		= "icon.png";
		}

		nlohmann::json windowSetting;
	};

	const _EditorSetting EditorSetting;
}

#endif // EDITORSETTING_H
