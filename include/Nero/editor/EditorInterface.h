////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACE_H
#define EDITORINTERFACE_H
///////////////////////////HEADERS//////////////////////////
//Poco
#include <Poco/Logger.h>
//IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
//Nero
#include <Nero/editor/ProjectManager.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/core/lua/scene/LuaScene.h>
#include <Nero/editor/GameProject.h>
#include <Nero/editor/LoggerApplication.h>
#include <Nero/core/cpp/utility/String.h>
#include <Nero/editor/AdvancedScene.h>
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/editor/AdvancedCamera.h>
#include <Nero/editor/EditorUtility.h>
#include <Nero/editor/ConsoleApplication.h>
#include <Nero/editor/GameLevelBuilder.h>
#include <Nero/editor/GameScreenBuilder.h>
//Json
#include <json/json.hpp>
//SFML
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
//File Diaglog
#include <nativefiledialog/include/nfd.h>
//Node Editor
# include <nodeeditor/imgui_node_editor.h>
//STD
#include <memory>
#include <functional>
#include <future>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorInterface
    {

        public:
            typedef std::unique_ptr<EditorInterface> Ptr;
			typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;

		public:
											EditorInterface(sf::RenderWindow& window);
										   ~EditorInterface();
			void							destroy();

        private:
			//initialization
			void							init();
			//game loop
			void							handleEvent(const sf::Event& event);
			void							update(const sf::Time& timeStep);
			void							render();
			//input
			void							handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
			//retrieve frame rate from core engine
			void							updateFrameRate(const float& frameRate, const float& frameTime);
			//let editor provide some objects
			void							setEditorSetting(Setting::Ptr setting);
			void							setEditorCamera(const AdvancedCamera::Ptr& camera);
			void							setEditorTextureHolder(TextureHolder::Ptr textureHolder);
			void							setEditorSoundHolder(SoundHolder::Ptr soundHolder);
			void							setEditorFontHolder(FontHolder::Ptr soundHolder);
			void							setCallbackWindowTitle(std::function<void (const std::string&)> callback);
			void							updateWindowTitle(const std::string& title);
			//close the editor
			void							closeEditor();
			//signal handling
			void							registerSignalHandler();
			static void						handleSignalAbnormalTermination(int signal);
			static void						handleSignalArithmeticError(int signal);
			static void						handleSignalIllegalInstruction(int signal);
			static void						handleSignalInteractiveAttention(int signal);
			static void						handleSignalInvalidStorageAccess(int signal);
			static void						handleSignalTerminsationRequest(int signal);

		private:
			//////////////main attributes
			friend class									EngineEditor;
			sf::RenderWindow&								m_RenderWindow;
			float											m_FrameRate;
			float											m_FrameTime;
			Setting::Ptr									m_EditorSetting;
			TextureHolder::Ptr								m_EditorTextureHolder;
			SoundHolder::Ptr								m_EditorSoundHolder;
			FontHolder::Ptr									m_EditorFontHolder;
			std::function<void (const std::string&)>		m_UpdateWindowTile;
			//environment
			EnvironmentSetup								m_EnvironmentSetup;
			//game project
			ProjectManager::Ptr								m_ProjectManager;
			GameProject::Ptr								m_GameProject;
			AdvancedScene::Ptr								m_AdvancedScene;
			GameLevelBuilder::Ptr							m_GameLevelBuilder;
			GameScreenBuilder::Ptr							m_GameScreenBuilder;
			ResourceManager::Ptr							m_ResourceManager;
			ResourceManager::Ptr							m_EditorResourceManager;

		private:
			//////////////docksapce
			ImGuiID											m_DockspaceID;
			bool											m_BuildDockspaceLayout;
			bool											m_SetupDockspaceLayout;
			void											createDockSpace();
			void											interfaceFirstDraw();
			//////////////starter window
			void											showConfigurationWindow();
			void											showConfigurationWelcome();
			void											showConfigurationFinish();
			void											showConfigurationEditor();
			void											showConfigurationTexturePacker();
			void											showConfigurationWorksapce();
			//////////////main menu bar
			MenuBarInput									m_MenuBarInput;
			void											showEditorMenuBar();
			void											showAboutEngineWindow();
			//////////////Tool Bar
			ImGuiWindow*									mToolbarWindow;
			void											showToolbarWindow();
			void											showToolbarScrollLeft();
			void											showToolbarScrollRight();
			void											showToolbarLeft(bool scrollToolbar);
			void											showToolbarMiddle(bool scrollToolbar);
			void											showToolbarRight(bool scrollToolbar);
			void											handleMenuBarFileAction();
			void											showToolbarWorldButton();
			void											showToolbarScreenButton();
			void											showToolbarFactoryButton();
			//////////////Workspace
			WorkspaceInput									m_WorkspaceInput;
			void											createWorkspace(const Parameter& parameter);
			void											importWorkspace(const std::string& directory);
			void											showWorkspaceWindow();
			void											showCreateWorkspace();
			void											showImportWorkspace();
			void											showWorkspaceList();
			//////////////Project
			ProjectInput									m_ProjectInput;
			int												m_CountCreateProject;
			void											createProject(const Parameter& parameter);
			void											openProject(const std::string& projectDirectory);
			void											compileProject();
			void											editProject();
			void											reloadProject();
			void											closeProject();
			void											showProjectManagerWindow();
			void											showCreateProjectWindow();
			void											showOpenProjectWindow();
			void											showRecentProjectWindow();
			NewGameLevelInput								m_NewGameLevelInput;
			NewGameLevelInput								m_NewGameScreenInput;
			void											createGameLevel(const Parameter& parameter);
			void											createGameScreen(const Parameter& parameter);
			void											openGameLevel();
			void											closeGameLevel();
			std::string										m_SelectedGameLevel;
			std::string										m_OpenedGameLevel;
            //
			sf::Sprite										flipTexture(const sf::Texture& texture);
			//editor view
				//upper left
			void											showUtilityWindow();
			void											showGameLevelWindow();
			void											showWorldChunckWindow();
				//bottom left
			void											showObjectLayerWindow();
			void											showGameScreenWindow();
				//right
			void											showExplorerWindow();
			void											showResourceBrowserWindow();
			void											showHelpWindow();
				//bottom
			void											showResourceWindow();
			void											showLoggingWindow();
			//utility
			void											showToggleButton(bool toggle, const std::string& label, std::function<void()> callback);
			//Docksapce
			bool											setup_dock = false;
			ImGuiID											actionBarId;
			ImGuiID											dock_id_right;
			ImGuiID											dock_id_upper_left;
			ImGuiID											dock_id_left_bottom;
			bool											show_project_window = false;
            //project creation
			LoggerApplication								m_LoggerApplication;
			bool											m_InterfaceFirstDraw;
			ImGuiIO											baseio;
			std::stringstream								buffer;
			std::streambuf*									old;
            //
			void											showGameProjectWindow();
			void											showGameSettingWindow();
			void											showVisualScriptWindow();
			void											showSceneWindow();
			void											showBackgroundTaskWindow();
            ////////////////////////Project and Workspace////////////////////////
            //General
			//Script Wizard
			char											m_InputClassName[100];
			char											m_InputParentClass[100];
			const char*										m_SelectedScriptType;
			int												m_SelectedScriptTypeIndex;
			const char*										m_InputSelectedGameLevel;
			int												m_SelectedGameLevelIndex;
			const char*										m_SelectedGameScreen;
			int												m_SelectedGameScreenIndex;
			//World chunk
			void											addWorldChunk();
			void											removeWorldChunk();
			int												m_InputSelectedWorldChunkId;
			//Object Layer
			void											addObjectLayer();
			void											removeObjectLayer();
			int												m_InputSelectedObjectLayerId;
			//
			void											removeGameLevel();
			void											editGameLevel();
			void											showNewGameLevelPopup();
			void											showNewGameScreenPopup();
			int												m_InputSelectedGameLevelId;
			//
			void											removeGameScreen();
			int												m_InputSelectedGameScreenId;
			//Tabs
			TabBarSwitch									m_ProjectManagerTabBarSwitch;
			TabBarSwitch									m_BottomDockspaceTabBarSwitch;
			//Banner
			//show view
			//
			void											showScriptCreationWindow();
            //function
			void											playScene();
			void											pauseScene();
			void											stepScene();
			void											resetScene();
			void											renderScene();
			//
			void											buildRenderContext();
			void											prepareRenderTexture();
			bool											mouseOnCanvas();
			//
			ax::NodeEditor::EditorContext*					g_Context;
			//
			ResourceType									m_ResourceBrowserType;
			//
			void											showSpriteResource();
			void											showAnimationResource();
			void											showFontResource();
			void											showLightmapResource();
			//
			sf::Texture&									getFontTexture(const std::string& fontName);
			std::map<std::string, sf::Texture>				m_FontTextureMap;
			//
			RenderTexturePtr								m_RenderTexture;
			EditorMode										m_EditorMode;
			BuilderMode										m_BuilderMode;
			//
			sf::RectangleShape								m_CameraXAxis;
			sf::RectangleShape								m_CameraYAxis;
			//
			AdvancedCamera::Ptr								m_EditorCamera;
			//
			void renderCamera();
			sf::View										m_CanvasFrontView;
			sf::Text										m_GameModeInfo;
			sf::Text										m_GameBuilderInfo;
			//
			void											renderGameModeInfo();
			RenderContext::Ptr								m_RenderContext;
			//
			std::string										getString(const EditorMode& editorMode);
			//
			void											selectDirectory(std::function<void(std::string)> callback);
			void											selectFile(std::function<void(std::string)> callback);
			void											selectFile(std::function<void(std::vector<std::string>)> callback);
			//
			void											clearScriptWizardInput();
			void											showMeshResource();
			sf::Vector2f									getAddObjectPosition();
			//
			void											switchBuilderMode();
			void											showCanvasMenu();
			//
			std::string										m_SelectedChunkNode;
			void											saveResourceFile(ResourceType type, const std::vector<std::string> loadedFileTable);
			//
			void											onSaveProject();
			void											onLoadProject();
			void											autoSaveProject();
			sf::Clock										m_AutoSaveClock;
			std::tuple<ImVec4, ImVec4>						getLayerColor(Object::Type type);
			//
			ImVec4											ambient_light;
			//
			void											createScriptObject(const Parameter& parameter);
			//
			ImVec4											getLoggingColor(logging::LEVEL level);
			std::string										m_MouseInformation;
			ConsoleApplication								m_ConsoleApplication;
			void											showConsoleWindow();
	};

}

#endif // EDITORINTERFACE_H
