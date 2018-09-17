////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RENDERENGINE_H
#define RENDERENGINE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/Engine.h>
#include <Nero/scene/Scene.h>
#include <Nero/camera/Camera.h>
#include <Nero/resource/ResourceManager.h>
#include <Nero/scene/SceneBuilder.h>
#include <Nero/engine/StartupScreen.h>
#include <Nero/utility/ConstantPool.h>
//SFGUI
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Canvas.hpp>
//BOOST
#include <boost/filesystem.hpp>
//STD
#include <future>
////////////////////////////////////////////////////////////

//*
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
}//*/

namespace nero
{
    class RenderEngine : public Engine
    {
        public: //Methods
                                            RenderEngine(const float& windowWidth = 800.f, const float& windowHeight = 600.f, const sf::String& title = "Nero Render Engine");
                                            RenderEngine(StartupScreen::Ptr startupScreen, const float& windowWidth = 800.f, const float& windowHeight = 600.f, const sf::String& title = "Nero Render Engine");
            virtual                        ~RenderEngine();

            template <typename T>
            void                            setScene(const sf::String& sceneName);

        protected: //Methods
            void                            handleEvent();
            void                            update(const sf::Time& timeStep);
            void                            render();

            void                            handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);

        private: //Start thread function
            int                             startEngine(bool& engineStarted, const float minTime);

        //Private Attibuts
        private:
            sfg::SFGUI                      m_Sfgui;
            sfg::Desktop                    m_Desktop;

            sfg::Canvas::Ptr                m_RenderCanvas;
            sf::View                        m_FrontView;

            Scene::Ptr                      m_Scene;
            std::function<Scene::Ptr()>     m_CreateScene;
            nlohmann::json                  m_SceneJson;

            Camera::Ptr                     m_Camera;
            ResourceManager::Ptr            m_ResourceManager;
            SceneBuilder::Ptr               m_SceneBuilder;
            StartupScreen::Ptr              m_StartupScreen;

            std::future<int>                m_StartEngineFuture;
            bool                            m_EngineStarted;
    };

    ////////////////////////////////////////////////////////////
    template <typename T>
    void RenderEngine::setScene(const sf::String& sceneName)
    {
        //Register a function that can return the Scene that has been set to the engine
        m_CreateScene =     [this] ()
                            {
                                return Scene::Ptr(new T(Context(m_RenderCanvas, m_FrontView, m_Camera, m_ResourceManager)));
                            };

        //create the scene
        m_Scene = m_CreateScene();

        //update the engine window title
        setWinTitle(sceneName);

        std::string file = WORKSPACE_FOLDER + "/" + sceneName + "/" + sceneName +  ".json";

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

            m_SceneBuilder->loadScene(scene);
            m_SceneBuilder->setPhysicWorld(m_Scene->m_PhysicWorld);
            m_SceneBuilder->buildScene(m_Scene->m_RootObject);
            m_Scene->init();
        }
    }
}


#endif // RENDERENGINE_H
