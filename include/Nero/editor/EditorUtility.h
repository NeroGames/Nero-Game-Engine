////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACEUTILITY_H
#define EDITORINTERFACEUTILITY_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/utility/String.h>
//Json
#include <json/json.hpp>
//Cpp
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

	struct WorkspaceInput
	{
		//create
		char                        location[256];
		char                        name[100];
		char                        company[100];
		char                        projectLead[100];
		char                        projectNamespace[10];
		//import
		char						locationImport[256];

		void clear()
		{
			string::fillCharArray(location,			sizeof(location),			string::StringPool.BLANK);
			string::fillCharArray(name,				sizeof(name),				string::StringPool.BLANK);
			string::fillCharArray(projectLead,		sizeof(projectLead),		string::StringPool.BLANK);
			string::fillCharArray(company,			sizeof(company),			string::StringPool.BLANK);
			string::fillCharArray(projectNamespace,	sizeof(projectNamespace),	string::StringPool.BLANK);
			string::fillCharArray(locationImport,	sizeof(locationImport),		string::StringPool.BLANK);
		}
	};

	struct ProjectInput
	{
		char                        name[100];
		char                        projectLead[100];
		char                        company[100];
		char                        projectNamespace[10];
		char                        description[512];
		const char*                 projectType;
		const char*                 codeEditor;
		const char*					workspace;
		int                         projectTypeIndex;
		int                         codeEditorIndex;
		int                         workspaceIndex;

		bool						startupPack;
		std::string                 lastCreatedProject;

		void clear()
		{
			string::fillCharArray(name,					sizeof(name),				string::StringPool.BLANK);
			string::fillCharArray(projectLead,			sizeof(projectLead),		string::StringPool.BLANK);
			string::fillCharArray(company,				sizeof(company),			string::StringPool.BLANK);
			string::fillCharArray(projectNamespace,		sizeof(projectNamespace),	string::StringPool.BLANK);
			string::fillCharArray(description,			sizeof(description),		string::StringPool.BLANK);

			startupPack = true;
		}

		void update(const nlohmann::json workspaceTable)
		{
			if(!workspaceTable.empty())
			{
				auto workspace = workspaceTable.front();

				string::fillCharArray(company,				sizeof(company),			workspace["company_name"].get<std::string>());
				string::fillCharArray(projectLead,			sizeof(projectLead),		workspace["project_lead"].get<std::string>());
				string::fillCharArray(projectNamespace,		sizeof(projectNamespace),	workspace["project_namespace"].get<std::string>());

				workspaceIndex = 0;
			}
			else
			{
				clear();
			}
		}
	};

	struct MenuBarInput
	{
		bool newProject			= false;
		bool openProject		= false;
		bool newWorkspace		= false;
	};

	void			popToolbarStyle();
	void			pushToolbarStyle();
}


#endif // EDITORINTERFACEUTILITY_H
