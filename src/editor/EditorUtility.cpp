////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include <Nero/editor/EditorUtility.h>
/////////////////////////////////////////////////////////////

namespace nero
{
	void TabBarSwitch::registerTabTable(const std::vector<std::string>& tabNameTable)
	{
		if(tabNameTable.size() <= 0)
		{
			return;
		}

		for(const std::string& tab : tabNameTable)
		{
			m_SwitchMap[tab] = false;
		}

		//select the first tab
		m_SwitchMap[tabNameTable.front()] = true;
	}

	void TabBarSwitch::selectTab(const std::string& tabName)
	{
		resetSwith();

		m_SwitchMap[tabName] = true;
	}

	ImGuiTabItemFlags TabBarSwitch::getTabStatus(const std::string& tabName)
	{
		if(m_SwitchMap[tabName])
		{
			return ImGuiTabItemFlags_SetSelected;
		}

		return ImGuiTabItemFlags_None;
	}

	void TabBarSwitch::resetSwith()
	{
		for(auto it = m_SwitchMap.begin(); it != m_SwitchMap.end(); it++)
		{
			it->second = false;
		}
	}

	sf::Vector2f formatSize(sf::Vector2f original, float size)
	{
		if(original.x > size)
		{
			float scale = size / original.x;
			original.x = size;
			original.y = original.y * scale;
		}
		if(original.y > size)
		{
			float scale = size / original.y;
			original.y = size;
			original.x = original.x * scale;
		}

		return original;
	};

}
