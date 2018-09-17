////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef DEVENGINE_H
#define DEVENGINE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/Engine.h>
#include <Nero/engine/DevEngineUI.h>
#include <Nero/engine/EngineSetting.h>
#include <Nero/camera/AdvancedCamera.h>
#include <Nero/engine/LoadingScreen.h>
#include <Nero/resource/ResourceManager.h>
#include <Nero/scene/SceneManager.h>
#include <Nero/scene/SoundManager.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
//BOOST
#include <boost/filesystem.hpp>
//EASY_LOGGING
#include <easyloggingpp/easylogging++.h>
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
    class DevEngine : public Engine
    {
        public: //Method
                                    DevEngine(const float& winWidth = 1305.f, const float& winHeight = 660.f, const sf::String& title = "Nero Game Engine");
            virtual                ~DevEngine();

            template <typename T>
            void                    addScene(std::string sceneName);

        private: //Main Methods
            void                    handleEvent();
            void                    update(const sf::Time& timeStep);
            void                    render();

        private: //Start thread function
            int                     startEngine(bool& engineStarted, const float minTime);

        private:
            //Startup Thread
            std::future<int>        m_StartEngineFuture;
            bool                    m_EngineStarted;
            //Loading Screen
            LoadingScreen::Ptr      m_LoadingScreen;
            //Engine UI
            DevEngineUI::Ptr        m_EngineUI;
            //Camera
            AdvancedCamera::Ptr     m_Camera;
            //Manager
            ResourceManager::Ptr    m_ResourceManager;
            SceneManager::Ptr       m_SceneManager;
            SoundManager::Ptr       m_SoundManager;
            //Setting
            EngineSetting           m_EngineSetting;
            //Grid
            Grid::Ptr               m_Grid;
            //Task
            std::vector<std::function<void()>> m_TaskTable;
    };

    template <typename T>
    void DevEngine::addScene(std::string sceneName)
    {
        if(m_TaskTable.size() > MAX_SCENE)
        {
            nero_log("failed to add scene :" + sceneName + " only " + _s(MAX_SCENE) + "can be added");
            return;
        }

        m_TaskTable.push_back([this, sceneName]()
        {
            nero_log("adding scene : " + sceneName);
            //Add a new Scene
            m_SceneManager->addScene<T>(sceneName);
            m_EngineUI->addScene(sceneName);

            //Create the scene directory if not exist
            using namespace boost::filesystem;
            std::string directory = WORKSPACE_FOLDER + "/" + sceneName;
            create_directories(path(directory));

            //Provide the update_engine function to the Scene
            SceneBuilder::Ptr  sceneBuilder     = m_SceneManager->getSceneBuilder(sceneName);
            UndoManager::Ptr   undoManager      = m_SceneManager->getUndoManager(sceneName);

            sceneBuilder->setUpdateEngineFunction(std::bind(&DevEngineUI::updateUI, m_EngineUI.get()));
            sceneBuilder->setEngineUndoFunction(std::bind(&DevEngineUI::updateUndo, m_EngineUI.get()));

            //Load the Scene if possible
            std::string file = WORKSPACE_FOLDER + "/" + sceneName + "/" + sceneName +  ".json";

            nlohmann::json scene;

            if(exists(file))
            {
                scene = loadJson(file);
                sceneBuilder->loadScene(scene);
            }
            else
            {
                scene = sceneBuilder->saveScene();
            }

            nlohmann::json undo;
            undo["scene"] = scene;
            undoManager->add(undo);

        });
    }
}

#endif // DEVENGINE_Hr
