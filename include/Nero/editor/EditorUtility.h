////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACEUTILITY_H
#define EDITORINTERFACEUTILITY_H
///////////////////////////HEADERS///////////////////////////
#include <map>
#include <vector>
#include <memory>
#include <imgui/imgui.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	class TabBarSwitch
	{
		public:
			void					selectTab(const std::string& tabName);
			void					registerTabTable(const std::vector<std::string>& tabNameTable);
			ImGuiTabItemFlags		getTabStatus(const std::string& tabName);
			void					resetSwith();

		private:
			std::map<std::string, bool> m_SwitchMap;
	};

	sf::Vector2f formatSize(sf::Vector2f original, float size);

	enum class EditorMode
	{
		WORLD_BUILDER,
		SCREEN_BUILDER,
		OBJECT_BUILDER,
		PLAY_GAME,
		RENDER_GAME
	};

	enum class BuilderMode
	{
		OBJECT,
		MESH,
		JOINT
	};

}


#endif // EDITORINTERFACEUTILITY_H
