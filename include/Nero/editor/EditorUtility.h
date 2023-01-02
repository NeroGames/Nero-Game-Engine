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
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <imgui/imgui.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class TabSelectionHandler
	{
        public:
            using Ptr = std::shared_ptr<TabSelectionHandler>;

		public:
            void                            selectTab(const std::string& tabName);
            void                            registerTab(const std::vector<std::string>& tabNameTable);
            ImGuiTabItemFlags               getTabStatus(const std::string& tabName);
            void                            reset();

		private:
            std::map<std::string, bool>     m_TabMap;
	};

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

    sf::Vector2f    formatSize(sf::Vector2f original, float size);
    void			popToolbarStyle();
    void			pushToolbarStyle(bool selected = false);
    void			pushGameLevelStyle(bool selected = false, bool opened = false);
    void			popGameLevelStyle();
    void			popResourceStyle(bool selected = false);
    void			pushResourceStyle(bool selected = false);

    namespace file
    {
        void selectDirectory(std::function<void(std::string)> callback);
        void selectFile(std::function<void(std::string)> callback);
        void selectFile(std::function<void(std::vector<std::string>)> callback);
    }
}


#endif // EDITORINTERFACEUTILITY_H
