////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef INTERFACE_H
#define INTERFACE_H
///////////////////////////HEADERS//////////////////////////
//IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Nero/editor/ProjectManager.h>
#include <Nero/editor/SceneManager.h>
#include <functional>
#include <Nero/core/scene/Scene.h>
#include <Nero/core/luascene/LuaScene.h>
#include <Nero/core/engine/GameProject.h>
#include <json/json.hpp>
#include <Nero/editor/LoggerApplication.h>
#include <Nero/core/utility/StringUtil.h>

////////////////////////////////////////////////////////////

namespace nero
{
    class Interface
    {

        public:
            typedef std::unique_ptr<Interface> Ptr;

                        Interface(sf::RenderWindow& window);
                       ~Interface();

        private:
            void        handleEvent(const sf::Event& event);
            void        update(const sf::Time& timeStep);
            void        render();

            void        updateFrameRate(const float& frameRate, const float& frameTime);

            void        quitEditor();


            /*template <typename T>
            void                    addScene(const std::string& projectName);

            template <typename T>
            void                    addLuaScene(const std::string& projectName);*/

            //Start Project
            void                    addScene(const std::string& projectName, std::function<Scene::Ptr(Scene::Context)> factory);
            void                    addLuaScene(const std::string& projectName, std::function<LuaScene::Ptr(Scene::Context)> factory);
            void                    openProject(const std::string& path);
            void                    openProject(const nlohmann::json& projectJson);

            void                    loadAllProject();

            //
            void                    setEditorSetting(const nlohmann::json& setting);

            sf::Sprite             flipTexture(const sf::Texture& texture);

            //editor view
            void                    createDockSpace();
                //upper left
            void                    showUtilityWindow();
            void                    showSceneChunckWindow();
            void                    showSceneScreenWindow();
                //right
            void                    showCurrentSceneWindow();

            void                    showLogWindow();
            void                    showResourceCategoryWindow();
            void                    showResourceWindow();
                //terminate
            void                    interfaceFirstDraw();

            //utility
            void                    showToggleButton(bool toggle, const std::string& label, std::function<void()> callback);

        private:
            friend class            Editor;
            sf::RenderWindow&       m_RenderWindow;
            bool        setup_dock = false;
            ImGuiID actionBarId;
            ImGuiID dock_id_right;
            ImGuiID dock_id_upper_left;

            bool            show_project_window = false;
            sf::Texture     project_manager_texture;

            Scene::Context      m_Context;

            SceneManager::Ptr       m_SceneManager;
            //
            nlohmann::json m_EditorSetting;

            //project creation

            std::string test_log;

            LoggerApplication m_LoggerApplication;
            bool   m_InterfaceFirstDraw;
            bool open_sprite_browser = false;
            ImGuiIO baseio;
            ImGuiID dockspace_id;
            ////////////////////////Tool Bar////////////////////////
            void                        showToolbarWindow();
            sf::Texture                 m_ProjectButtonTexture;
            sf::Texture                 m_CompileButtonTexture;
            sf::Texture                 m_ReloadButtonTexture;
            sf::Texture                 m_EditButtonTexture;
            sf::Texture                 m_BlankButtonTexture;

            ////////////////////////Project and Workspace////////////////////////
            //General
            ProjectManager::Ptr         m_ProjectManager;
            GameProject::Ptr            m_CurrentProject;
            ImVec2                      m_ProjectManagerWindowSize;
            //Project Workspace
            nlohmann::json              m_WorkspaceTable;               //list of available workspaces
            int                         m_WorksapceStatus;              //0 : no_worksapce, 1 : redirect_user, 2 worksapce_available
            char                        m_InputWorksapceFolder[256];    //read workspace path
            char                        m_InputWorkspaceName[100];
            char                        m_InputWorkspaceCompany[100];
            char                        m_InputWorkspaceLead[100];
            const char*                 m_SelectedWorkpsapce;
            int                         m_SelectedWorkpsapceIdex;
            //Game Project
            char                        m_InputProjectName[100];        //read project name
            char                        m_InputProjectLead[100];        //read project lead
            char                        m_InputProjectCompany[100];     //read project company
            char                        m_InputProjectDescription[512]; //read project description
            //Tabs
            int                         m_ProjectManagerSelectedTab;    //0 : Create Project, 1 : Open Project, 2 : Recent Project, 3 : Worksapce
            //Banner
            sf::Texture                 m_ProjectManagerBannerTexture;
            //show view
            void                        showProjectManagerWindow();
            void                        showCreateProjectWindow();
            void                        showOpenPorjectWindow();
            void                        showRecentProjectWindow();
            void                        showWorkspaceWindow();
            //function
            void                        createProject(const nlohmann::json& projectJson);
            void                        createWorkspace(const nlohmann::json& workspaceJson);
            void                        compileProject();
            void                        editProject();
            void                        reloadProject();
    };

    /*template <typename T>
    void Interface::addScene(const std::string& projectName)
    {
        addScene(projectName, [this] (Scene::Context context)
        {
            return Scene::Ptr(new T(context));
        });
    }

    template <typename T>
    void Interface::addLuaScene(const std::string& projectName)
    {
        addLuaScene(projectName, [this] (Scene::Context context)
        {
            return LuaScene::Ptr(new T(context));
        });
    }*/

}

#endif // INTERFACE_H
