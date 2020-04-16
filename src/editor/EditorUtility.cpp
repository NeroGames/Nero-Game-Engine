////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include <Nero/editor/EditorUtility.h>
#include <Nero/core/cpp/utility/File.h>
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

	void pushToolbarStyle(bool selected)
	{
		ImVec4 color = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 7.f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.000f, 1.000f, 1.000f, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.000f, 1.000f, 1.000f, .950f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.000f, 1.000f, 1.000f, .900f));
		ImGui::PushStyleColor(ImGuiCol_Text, color);

		if(selected)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.f);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.595f, 0.308f, 1.000f));
		}
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
			ImGui::PushStyleColor(ImGuiCol_Border, color);
		}


		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	}

	void popToolbarStyle()
	{
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(5);
		ImGui::PopFont();
	}

	std::string AppLauncher::qtCreatorProcessId		= StringPool.BLANK;
	std::string AppLauncher::visaulStudioProcessId	= StringPool.BLANK;
	std::string AppLauncher::profilerProcessId		= StringPool.BLANK;
	std::string AppLauncher::texturePackerProcessId	= StringPool.BLANK;
	std::string AppLauncher::NERO_GAME_HOME			= getenv("NERO_GAME_HOME") ? std::string(getenv("NERO_GAME_HOME")) : StringPool.BLANK;

	void AppLauncher::launchTexturePacker()
	{
		if(cmd::processRunning(AppLauncher::texturePackerProcessId))
		{
			showApplication("TexturePacker", AppLauncher::texturePackerProcessId);
		}
		else
		{
			//TODO
			std::string texturePacker = "C:/Program Files/CodeAndWeb/TexturePacker/bin/TexturePackerGUI.exe";
			AppLauncher::texturePackerProcessId = cmd::launchApplication(texturePacker);

			showApplication("TexturePacker", AppLauncher::texturePackerProcessId);
		}
	}

	void AppLauncher::launchProfiler()
	{
		if(cmd::processRunning(AppLauncher::profilerProcessId))
		{
			showApplication("EasyProfiler", AppLauncher::profilerProcessId);
		}
		else
		{
			std::string profiler = AppLauncher::NERO_GAME_HOME + "/Tools/Profiler/profiler_gui.exe";
			AppLauncher::profilerProcessId = cmd::launchApplication(profiler);

			showApplication("EasyProfiler", AppLauncher::profilerProcessId);
		}
	}

	void AppLauncher::showApplication(const std::string& name, const std::string& proccessId)
	{
		std::string windowmode	= file::getWindowsPath(AppLauncher::NERO_GAME_HOME + "/Tools/Script/windowmode.bat");
		std::string sendkeys	= file::getWindowsPath(AppLauncher::NERO_GAME_HOME + "/Tools/Script/sendkeys.bat");

		std::string cmd1 = windowmode + " -pid " + proccessId + " -mode restore";
		std::string cmd2 = sendkeys + " " + name + "  \"\"";

		system(cmd1.c_str());
		system(cmd2.c_str());
	}

	bool EnvironmentSetup::configure()
	{
		return setupCodeEditor || setupTexturePacker || setupWorkspace;
	}

	void EnvironmentSetup::showView()
	{
		nero_log(nero_s(currentView));
		viewTable.at(currentView)();
	}

	void EnvironmentSetup::nextView()
	{
		if(currentView < viewTable.size() - 1)
		{
			currentView += 1;
		}
	}

	bool EnvironmentSetup::finish()
	{
		return currentView == (viewTable.size() - 1);
	}

	bool EnvironmentSetup::finishNext()
	{
		return currentView == (viewTable.size() - 2);
	}

	void EnvironmentSetup::clearInput()
	{
		string::fillCharArray(qtCreatorPath,	 sizeof(qtCreatorPath),		StringPool.BLANK);
		string::fillCharArray(visualStudioPath,	 sizeof(visualStudioPath),	StringPool.BLANK);
		string::fillCharArray(texturePackerPath, sizeof(texturePackerPath),	StringPool.BLANK);
	}

	bool EnvironmentSetup::hasPrevious()
	{
		return currentView >= 1;
	}

	void EnvironmentSetup::previousView()
	{
		if(currentView > 0)
		{
			currentView -= 1;
		}
	}

}
