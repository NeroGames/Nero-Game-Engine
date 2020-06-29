////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
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
#ifndef NERO_ENGINE_DEVELOPMENT
namespace
{
    class EarlyInit
    {
        public :
            static bool initLog()
            {
                //Create Log configuration if not exit
                if(!nero::fileExist(nero::CONFIGURATION_FOLDER + "/" + nero::LOG_CONFIGURATION))
                {
                    nero::saveFile(nero::CONFIGURATION_FOLDER + "/" + nero::LOG_CONFIGURATION, log_config_txt, log_config_txt_len);
                }

                //Load Log configuration
                el::Configurations logSetting(nero::CONFIGURATION_FOLDER + "/" + nero::LOG_CONFIGURATION);
                //el::Loggers::reconfigureAllLoggers(logSetting);

                return true;
            }

            static bool initDirectory()
            {
                nero::buildEngineDirectory();
                return true;
            }
    };

    bool result1 = EarlyInit::initDirectory();
    bool result2 = EarlyInit::initLog();
}
#endif // NERO_ENGINE_DEVELOPMENT
////////////////////////////////////////////////////////////
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
            void                            resetScene();

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
            SceneSetting                    m_SceneSetting;
            StartupScreen::Ptr              m_StartupScreen;

            std::future<int>                m_StartEngineFuture;
            bool                            m_EngineStarted;
            bool                            m_RestartScene;

            std::function<void()>           m_Task;
    };

    ////////////////////////////////////////////////////////////
    template <typename T>
    void RenderEngine::setScene(const sf::String& sceneName)
    {
        m_Task = [this, sceneName]()
        {
            //Register a function that can return the Scene that has been set to the engine
            m_CreateScene =     [this] ()
                                {
                                    return Scene::Ptr(new T(Scene::Context(m_RenderCanvas, m_FrontView, m_Camera, m_ResourceManager, true)));
                                };

            //create the scene
            m_Scene = m_CreateScene();

            //update the engine window title
            setWindowTitle(sceneName);

            std::string file = WORKSPACE_FOLDER + "/" + sceneName + "/" + sceneName +  ".json";

            if(fileExist(file))
            {
                m_SceneJson =  loadJson(file);

                //Scene
                m_Scene->m_SceneSetting = SceneSetting::fromJson(m_SceneJson["scene_setting"]);
                m_Scene->m_CameraSetting = CameraSetting::fromJson(m_SceneJson["world_view"]["camera_setting"]);
                m_Scene->m_SoundManager->fromJson(m_SceneJson["sound_setting"]);
                m_Scene->m_CanvasColor = colorFromJson(m_SceneJson["world_view"]["canvas_color"]);
                //World
                SceneBuilder::Ptr sceneBuilder = SceneBuilder::Ptr(new SceneBuilder(m_RenderCanvas, m_ResourceManager, m_Scene->m_SceneSetting));
                sceneBuilder->loadScene(m_SceneJson["world_view"]);
                sceneBuilder->setPhysicWorld(m_Scene->m_PhysicWorld);
                sceneBuilder->buildScene(m_Scene->m_World);
                //Camera
                m_Camera->setPosition(m_Scene->m_CameraSetting.defaultPosition);
                m_Camera->setRotation(m_Scene->m_CameraSetting.defaultRotation);
                m_Camera->setZoom(m_Scene->m_CameraSetting.defaultZoom);
                //Screen
                nlohmann::json screen_table = m_SceneJson["screen_table"];
                for(auto screenJson : screen_table)
                {
                    SceneBuilder::Ptr sceneBuilder = SceneBuilder::Ptr(new SceneBuilder(m_RenderCanvas, m_ResourceManager, m_Scene->m_SceneSetting));
                    Screen::Ptr screen = Screen::Ptr(new Screen);

                    //FrontScreen
                    screen->screen  = Object::Ptr(new Object());
                    screen->screenUI = UIObject::Ptr(new UIObject());

                    sceneBuilder->loadScene(screenJson);
                    sceneBuilder->buildScene(screen->screen);
                    sceneBuilder->buildUI(screen->screenUI);

                    screen->name = screenJson["name"];
                    screen->hide = true;
                    screen->canvasColor = colorFromJson(screenJson["canvas_color"]);

                    m_Scene->m_ScreenTable.push_back(screen);
                }

                //Finish
                m_Scene->m_ObjectManager->setObjectCount(sceneBuilder->getObjectCount());
                m_Scene->m_ObjectManager->setPhysicWorld(m_Scene->m_PhysicWorld);
                m_Scene->checkSceneObject();
                m_Scene->init();

                m_Scene->m_ResetScene = [this]()
                {
                    this->resetScene();
                };

                m_Scene->m_QuitEngine = [this](){ m_Window.close();};

            }
        };
    }
}
#endif // RENDERENGINE_H
