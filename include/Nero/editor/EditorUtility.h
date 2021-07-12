////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACEUTILITY_H
#define EDITORINTERFACEUTILITY_H
///////////////////////////HEADERS///////////////////////////
//Poco
#include <Nero/core/cpp/utility/CommandLine.h>
//Nero
#include <Nero/core/cpp/utility/String.h>
//Json
#include <json/json.hpp>
//Cpp
#include <map>
#include <vector>
#include <memory>
#include <functional>
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
		char                        projectNamespace[11];
		//import
		char						locationImport[256];

		std::string errorMessage	= StringPool.BLANK;
		std::string redirectLink	= StringPool.BLANK;
		bool error					= true;

		void clear()
		{
			string::fillCharArray(location,			sizeof(location),			StringPool.BLANK);
			string::fillCharArray(name,				sizeof(name),				StringPool.BLANK);
			string::fillCharArray(projectLead,		sizeof(projectLead),		StringPool.BLANK);
			string::fillCharArray(company,			sizeof(company),			StringPool.BLANK);
			string::fillCharArray(projectNamespace,	sizeof(projectNamespace),	StringPool.BLANK);
			string::fillCharArray(locationImport,	sizeof(locationImport),		StringPool.BLANK);
		}
	};

	struct ProjectInput
	{
		char                        name[100];
		char                        projectLead[100];
		char                        company[100];
		char                        projectNamespace[11];
		char                        description[512];
		const char*                 projectType;
		const char*                 codeEditor;
		const char*					workspace;
		int                         projectTypeIndex;
		int                         codeEditorIndex;
		int                         workspaceIndex;

		bool						startupPack;
		std::string                 lastCreatedProject;

		std::string errorMessage	= StringPool.BLANK;
		std::string redirectLink	= StringPool.BLANK;
		bool error					= true;


		void clear()
		{
			string::fillCharArray(name,					sizeof(name),				StringPool.BLANK);
			string::fillCharArray(projectLead,			sizeof(projectLead),		StringPool.BLANK);
			string::fillCharArray(company,				sizeof(company),			StringPool.BLANK);
			string::fillCharArray(projectNamespace,		sizeof(projectNamespace),	StringPool.BLANK);
			string::fillCharArray(description,			sizeof(description),		StringPool.BLANK);

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
		bool newProject				= false;
		bool openProject			= false;
		bool newWorkspace			= false;
		bool aboutEngine			= false;
	};

	void			popToolbarStyle();
	void			pushToolbarStyle(bool selected = false);
	void			pushGameLevelStyle(bool selected = false, bool opened = false);
	void			popGameLevelStyle();
	void			popResourceStyle(bool selected = false);
	void			pushResourceStyle(bool selected = false);

	struct AppLauncher
	{
		//let the Interface only access
		//friend class			EditorInterface;

		static std::string		NERO_GAME_HOME;
		static std::string		TEXTURE_PACKER;
		static std::string		qtCreatorProcessId;
		static std::string		visaulStudioProcessId;
		static std::string		profilerProcessId;
		static std::string		texturePackerProcessId;

		static void				launchTexturePacker();

		static void				launchProfiler();
	};

	struct EnvironmentSetup
	{
		bool setupCodeEditor					= false;
		bool setupTexturePacker					= false;
		bool setupWorkspace						= false;
		bool readyNext							= false;

		bool selectQtCreator					= true;
		bool selectVisualStudio					= false;

		bool createWorkspace					= true;
		bool importWorkspace					= false;

		char qtCreatorPath[256];
		char visualStudioPath[256];
		char texturePackerPath[256];

		bool									configure();
		bool									restart();
		std::vector<std::function<void()>>		viewTable;
		unsigned int							currentView = 0;
		void									showView();
		void									nextView();
		bool									finish();
		bool									finishNext();
		void									clearInput();
		bool									hasPrevious();
		void									previousView();
	};

	struct NewGameLevelInput
	{
		char                        name[100];
		bool						enablePhysics;
		bool						enableLight;
		char                        prototype[100]; //template
	};

	struct NewGameScrenInput
	{
		char                        name[100];
		char                        parent[100];
		char                        prototype[100]; //template
	};
}


#endif // EDITORINTERFACEUTILITY_H
