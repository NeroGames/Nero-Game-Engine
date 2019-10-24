////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORCONSTANT_H
#define EDITORCONSTANT_H
///////////////////////////HEADERS///////////////////////////
#include <string>
/////////////////////////////////////////////////////////////
namespace nero
{
	struct _EditorConstant
	{
			const std::string SCENE_LEVEL					= "Level";
			const std::string SCENE_CHUNCK					= "Chunck";
			const std::string SCENE_SCREEN					= "Screen";
			const std::string SCENE_LAYER					= "Layer";
			const std::string UTILITY						= "Utility";
			const std::string Music							= "Music";
			const std::string SCENE							= "Scene     ";
			const std::string SCENE_EXPLORER				= "Explorer";

			const std::string PROJECT_MANAGER				= "Project Manager";
			const std::string CREATE_PROJECT				= "Create Project";
			const std::string OPEN_PROJECT					= "Open Project";
			const std::string RECENT_PROJECT				= "Recent Project";
			const std::string WORKSPACE						= "Workspace";
			const std::string ERROR_CREATING_WORKSPACE		= "Error Creating Workspace";
			const std::string ERROR_CREATING_PROJECT		= "Error Creating Project";


			const std::string GAME_SETTING					= "Game Settings";
			const std::string GAME_PROJECT					= "Game Project";


			const std::string Toolbar						= "Toolbar";

	};

	const _EditorConstant EditorConstant;
}


#endif // EDITORCONSTANT_H

