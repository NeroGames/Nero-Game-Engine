////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef RENDERENGINE_H
#define RENDERENGINE_H

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/engine/Engine.h>
#include <NERO/scene/Scene.h>
#include <NERO/camera/Camera.h>
#include <NERO/resource/Resource.h>
#include <NERO/scene/ObjectManager.h>
//SFGUI
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Canvas.hpp>
////////////////////////////////////////////////////////////

namespace
{
    class ConfigLog
    {
        public :
            static bool config()
            {
                el::Configurations log_conf("config/log_config.nero");
                el::Loggers::reconfigureAllLoggers(log_conf);

                return true;
            }
    };

    bool result = ConfigLog::config();
}


namespace nero
{
    class RenderEngine : public Engine
    {
        public: //Methods
                                            RenderEngine(const sf::String& title = "Nero Render Engine", const float& windowWidth = 800.f, const float& windowHeight = 600.f);
            virtual                        ~RenderEngine();

            template <typename T>
            void                            setScene(const sf::String& scene_name);

        protected: //Methods
            void                            handleEvents();
            void                            update(const sf::Time& time_step);
            void                            render();

            void                            handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);


        //Private Attibuts
        private:
            sfg::SFGUI                      m_Sfgui;
            sfg::Desktop                    m_Desktop;

            sfg::Canvas::Ptr                m_RenderCanvas;
            sf::View                        m_CanvasDefaultView;

            Scene::Ptr                      m_Scene;
            std::function<Scene::Ptr()>     m_CreateScene;
            SceneSettings                   m_SceneSettings;

            Camera                          m_Camera;
            ResourceManager                 m_ResourceManager;
            ObjectManager                   m_ObjectManager;

            nlohmann::json                 m_SceneJson;
    };

    ////////////////////////////////////////////////////////////
    template <typename T>
    void RenderEngine::setScene(const sf::String& scene_name)
    {
        //Register a function that can return the Scene that has been set to the engine
        m_CreateScene =     [this] ()
                            {
                                return Scene::Ptr(new T(Scene::Context(m_RenderCanvas, m_CanvasDefaultView, m_Camera, m_ResourceManager)));
                            };

        //create the scene
        m_Scene = m_CreateScene();

        //update the engine window title
        this->setWinTitle(scene_name);

        std::string file = NERO_FOLDER + "/" + scene_name + "/" + scene_name +  ".json";

        using namespace boost::filesystem;

        if(exists(file))
        {
            std::ifstream stream(file);
            if(!stream.is_open())
                return;

            nlohmann::json scene;
            stream >> scene;
            stream.close();

            m_SceneJson = scene;

            m_ObjectManager.loadScene(scene);
            m_ObjectManager.setWorld(m_Scene->m_World);
            m_ObjectManager.buildScene(m_Scene->m_RootObject);
            m_Scene->init();
        }
    }
}


#endif // RENDERENGINE_H
