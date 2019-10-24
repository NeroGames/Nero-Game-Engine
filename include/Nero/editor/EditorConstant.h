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
		//ImGUI ID
		const std::string DOCKSPACE_ID					= "editor-dockspace";

		//ImGUI layout file
		const std::string FILE_INTERFACE_LAYOUT			= "imgui";

		//Window
			//toolbar
		const std::string WINDOW_TOOLBAR				= "Toolbar";
			//upper left
		const std::string WINDOW_UTILITY				= "Utility";
		const std::string WINDOW_MUSIC					= "Music";
		const std::string WINDOW_Grid					= "Gid";
			//lower left
		const std::string WINDOW_LEVEL					= "Level";
		const std::string WINDOW_CHUNCK					= "Chunck";
		const std::string WINDOW_SCREEN					= "Screen";
		const std::string WINDOW_LAYER					= "Layer";
			//right
		const std::string WINDOW_EXPLORER				= "Explorer";
		const std::string WINDOW_HELP					= "Engine Help";
		const std::string WINDOW_RESOURCE_BROWSER		= "Resource Browser";
			//bottom
		const std::string WINDOW_RESOURCE				= "Resource";
		const std::string WINDOW_LOGGING				= "Logging";
			//center
		const std::string WINDOW_GAME_SCENE				= "Scene     ";
		const std::string WINDOW_GAME_SETTING			= "Game Settings";
		const std::string WINDOW_GAME_PROJECT			= "Game Project";
		const std::string WINDOW_FACTORY				= "Factory";
		const std::string WINDOW_IMGUI_DEMO				= "Dear ImGui Demo";

		//Window and View

		const std::string PROJECT_MANAGER				= "Project Manager";
		const std::string CREATE_PROJECT				= "Create Project";
		const std::string OPEN_PROJECT					= "Open Project";
		const std::string RECENT_PROJECT				= "Recent Project";
		const std::string WORKSPACE						= "Workspace";
		const std::string ERROR_CREATING_WORKSPACE		= "Error Creating Workspace";
		const std::string ERROR_CREATING_PROJECT		= "Error Creating Project";






	};

	const _EditorConstant EditorConstant;
}


#endif // EDITORCONSTANT_H

