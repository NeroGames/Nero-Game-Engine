////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACE_H
#define EDITORINTERFACE_H
///////////////////////////HEADERS//////////////////////////
//IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Nero/editor/ProjectManager.h>
#include <functional>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/cpp/luascene/LuaScene.h>
#include <Nero/editor/GameProject.h>
#include <json/json.hpp>
#include <Nero/editor/LoggerApplication.h>
#include <Nero/core/cpp/utility/StringUtil.h>
#include <Nero/core/cpp/utility/LogUtil.h>
#include <Nero/editor/AdvancedScene.h>
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/editor/AdvancedCamera.h>
#include <Nero/editor/EditorUtility.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <nativefiledialog/include/nfd.h>

#include <future>
#include <map>
# include <nodeeditor/imgui_node_editor.h>

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

        private:
            void        handleEvent(const sf::Event& event);
            void        update(const sf::Time& timeStep);
            void        render();

            void        updateFrameRate(const float& frameRate, const float& frameTime);

            void        quitEditor();

            void                    loadAllProject();

            //
            void                    setEditorSetting(const nlohmann::json& setting);

            sf::Sprite             flipTexture(const sf::Texture& texture);

			//editor view
				//upper left
			void                    showUtilityWindow();
			void                    showSceneLevelWindow();
			void                    showWorldChunckWindow();
				//bottom left
			void					showObjectLayerWindow();
            void                    showSceneScreenWindow();
                //right
			void                    showExplorerWindow();
			void                    showResourceBrowserWindow();
			void					showHelpWindow();
				//bottom
            void                    showResourceWindow();
			void                    showLoggingWindow();
                //terminate
            void                    interfaceFirstDraw();

            //utility
            void                    showToggleButton(bool toggle, const std::string& label, std::function<void()> callback);

        private:
			friend class						EngineEditor;
			sf::RenderWindow&					m_RenderWindow;
			//Docksapce
			ImGuiID								m_DockspaceID;
			bool								m_BuildDockspaceLayout;
			bool								m_SetupDockspaceLayout;
			void								createDockSpace();
			void								createEditorMenuBar();

            bool        setup_dock = false;
			ImGuiID actionBarId;
			ImGuiID dock_id_right;
            ImGuiID dock_id_upper_left;
			ImGuiID dock_id_left_bottom;

            bool            show_project_window = false;

            //
            nlohmann::json m_EditorSetting;

            //project creation

            std::string test_log;

            LoggerApplication m_LoggerApplication;
            bool   m_InterfaceFirstDraw;
            ImGuiIO baseio;
            std::stringstream buffer;
            std::streambuf * old;

			void setCallbackWindowTitle(std::function<void (const std::string&)> fn);

            std::function<void (const std::string&)> m_UpdateWindowTile;

            //
            void                        showGameProjectWindow();
            void                        showGameSettingWindow();
            void                        showSceneWindow();
			void						showBackgroundTaskWindow();



            ////////////////////////Tool Bar////////////////////////
            void                        showToolbarWindow();

            ////////////////////////Project and Workspace////////////////////////
            //General
            ProjectManager::Ptr         m_ProjectManager;
            GameProject::Ptr            m_CurrentProject;
            //Project Workspace
            nlohmann::json              m_WorkspaceTable;               //list of available workspaces
            int                         m_WorksapceStatus;              //0 : no_worksapce, 1 : redirect_user, 2 worksapce_available
			char                        m_InputWorksapceLocation[256];    //read workspace path
			char                        m_InputWorksapceLocationImport[256];    //read workspace path
            char                        m_InputWorkspaceName[100];
            char                        m_InputWorkspaceCompany[100];
			char                        m_InputWorkspaceLead[100];
            char                        m_InputWorkspaceNamespace[10];
            const char*                 m_SelectedWorkpsapce;
			int                         m_SelectedWorkpsapceIdex;
			int                         m_SelectedProjectTypeIdex;
			int                         m_SelectedCodeEditorIdex;

            //Game Project
            char                        m_InputProjectName[100];        //read project name
            char                        m_InputProjectLead[100];        //read project lead
            char                        m_InputProjectCompany[100];     //read project company
            char                        m_InputProjectNamespace[10];
            char                        m_InputProjectDescription[512]; //read project description
			const char*                 m_SelectedProjectType;
			const char*                 m_SelectedCodeEditor;
            std::future<int>            m_CreateProjectFuture;
			std::future<int>            m_CompileProjectFuture;

            int                         m_ProjectCreationStatus;
            std::string                 m_LastCreatedProject;

			//World chunk
			void						addWorldChunk();
			void						removeWorldChunk();
			int							m_InputSelectedWorldChunkId;
			//Object Layer
			void						addObjectLayer();
			void						removeObjectLayer();
			int							m_InputSelectedObjectLayerId;
			//
			void						addGameLevel();
			void						removeGameLevel();
			int							m_InputSelectedGameLevelId;


            //Tabs
			TabBarSwitch				m_ProjectManagerTabBarSwitch;
			TabBarSwitch				m_BottomDockspaceTabBarSwitch;
            //Banner
            //show view
            void                        showProjectManagerWindow();
            void                        showCreateProjectWindow();
			void                        showOpenProjectWindow();
            void                        showRecentProjectWindow();
            void                        showWorkspaceWindow();
            //function
			int                         createProject(const Setting& parameter, int& status);
			void                        createWorkspace(const Setting& parameter);
			void						importWorkspace(const std::string& directory);
			void                        openProject(const std::string& projectDirectory);
            void                        compileProject();
            void                        editProject();
            void                        reloadProject();
			void						playScene();
			void						pauseScene();
			void						stepScene();
			void						resetScene();
			void						renderScene();

			void						setSetting(Setting::Ptr setting);

            //
            GameProject::Ptr            m_GameProject;
            AdvancedScene::Ptr          m_AdvancedScene;
			void				buildRenderContext();
			void						prepareRenderTexture();
			bool				isMouseOnCanvas();

			ax::NodeEditor::EditorContext*	g_Context;

			TextureHolder::Ptr			m_EditorTextureHolder;
			SoundHolder::Ptr			m_EditorSoundHolder;
			FontHolder::Ptr				m_EditorFontHolder;
			ResourceManager::Ptr		m_ResourceManager;

			Setting::Ptr				m_Setting;

			void		setEditorTextureHolder(TextureHolder::Ptr textureHolder);
			void		setEditorSoundHolder(SoundHolder::Ptr soundHolder);
			void		setEditorFontHolder(FontHolder::Ptr soundHolder);
			void		setResourceManager(ResourceManager::Ptr resourceManager);

			ResourceType				m_ResourceBrowserType;

			void		showSpriteResource();
			void		showAnimationResource();
			void		showFontResource();
			void		showLightmapResource();

			sf::Texture& getFontTexture(const std::string& fontName);
			std::map<std::string, sf::Texture> m_FontTextureMap;

			RenderTexturePtr			m_RenderTexture;
			EditorMode					m_EditorMode;
			BuilderMode					m_BuilderMode;

			sf::RectangleShape      m_CameraXAxis;
			sf::RectangleShape      m_CameraYAxis;

			AdvancedCamera::Ptr		m_EditorCamera;

			void setCamera(const AdvancedCamera::Ptr& camera);
			void renderCamera();
			sf::View				m_CanvasFrontView;
			sf::Text				m_GameModeInfo;

			void			renderGameModeInfo();
			float	m_FrameRate;
			float	m_FrameTime;

			RenderContextPtr m_RenderContext;

			void init();
			std::string getString(const EditorMode& editorMode);

			void selectDirectory(std::function<void(nfdchar_t *outPath)> callback);
			void clearWorkspaceInput();
			void clearProjectInput();
			void updateProjectInput();
			void showMeshResource();
			sf::Vector2f getAddObjectPosition();

			void handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
			void switchBuilderMode();
			void showCanvasMenu();

			void updateWindowTitle(const std::string& title);


			std::string m_SelectedChunkNode;
			void saveResourceFile(ResourceType type, const std::vector<std::string> fileTable);

			void onSaveProject();
			void onLoadProject();
			void autoSaveProject();
			sf::Clock	m_AutoSaveClock;
			std::tuple<ImVec4, ImVec4> getLayerColor(Object::Type type);

			ImVec4 ambient_light;

	};

}

#endif // EDITORINTERFACE_H
