////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACEUTILITY_H
#define EDITORINTERFACEUTILITY_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/utility/StringUtil.h>
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
			fillCharArray(location,			sizeof(location),			StringPool.BLANK);
			fillCharArray(name,				sizeof(name),				StringPool.BLANK);
			fillCharArray(projectLead,		sizeof(projectLead),		StringPool.BLANK);
			fillCharArray(company,			sizeof(company),			StringPool.BLANK);
			fillCharArray(projectNamespace,	sizeof(projectNamespace),	StringPool.BLANK);
			fillCharArray(locationImport,	sizeof(locationImport),		StringPool.BLANK);
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
			fillCharArray(name,					sizeof(name),				StringPool.BLANK);
			fillCharArray(projectLead,			sizeof(projectLead),		StringPool.BLANK);
			fillCharArray(company,				sizeof(company),			StringPool.BLANK);
			fillCharArray(projectNamespace,		sizeof(projectNamespace),	StringPool.BLANK);
			fillCharArray(description,			sizeof(description),		StringPool.BLANK);

			startupPack = true;
		}

		void update(const nlohmann::json workspaceTable)
		{
			if(!workspaceTable.empty())
			{
				auto workspace = workspaceTable.front();

				fillCharArray(company,				sizeof(company),			workspace["company_name"].get<std::string>());
				fillCharArray(projectLead,			sizeof(projectLead),		workspace["project_lead"].get<std::string>());
				fillCharArray(projectNamespace,		sizeof(projectNamespace),	workspace["project_namespace"].get<std::string>());

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
