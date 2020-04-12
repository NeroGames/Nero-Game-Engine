////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
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

	void pushToolbarStyle()
	{
		ImVec4 color = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 7.f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.000f, 1.000f, 1.000f, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.000f, 1.000f, 1.000f, .950f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.000f, 1.000f, 1.000f, .900f));
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::PushStyleColor(ImGuiCol_Border, color);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	}

	void popToolbarStyle()
	{
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(5);
		ImGui::PopFont();
	}

}
