////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
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
#include <json/json.hpp>
#include <Nero/editor/LoggerApplication.h>

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
            void        createDockSpace();

            void        showProjectManagerWindow();

            template <typename T>
            void                    addScene(const std::string& projectName);

            template <typename T>
            void                    addLuaScene(const std::string& projectName);

            //Start Project
            void                    addScene(const std::string& projectName, std::function<Scene::Ptr(Scene::Context)> factory);
            void                    addLuaScene(const std::string& projectName, std::function<LuaScene::Ptr(Scene::Context)> factory);
            void                    createProject(const nlohmann::json& projectJson);
            void                    openProject(const std::string& path);
            void                    openProject(const nlohmann::json& projectJson);

            void                    loadAllProject();

            //
            void                    setEditorSetting(const nlohmann::json& setting);

            sf::Sprite             flipTexture(const sf::Texture& texture);

            void                    showLogWindow();

        private:
            friend class            Editor;
            sf::RenderWindow&       m_RenderWindow;
            bool        setup_dock = false;
            ImGuiID actionBarId;
            ImGuiID dock_id_right;
            bool            show_project_window = false;
            sf::Texture     project_manager_texture;

            Scene::Context      m_Context;

            ProjectManager::Ptr     m_ProjectManager;
            SceneManager::Ptr       m_SceneManager;

            //
             nlohmann::json m_EditorSetting;

             //project creation
             char project_name[100];
             char project_lead[100];
             char project_company[100];
             char project_description[400];

             std::string test_log;

             LoggerApplication m_LoggerApplication;

             bool   fristLoad;


    };

    template <typename T>
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
    }

}

#endif // INTERFACE_H
