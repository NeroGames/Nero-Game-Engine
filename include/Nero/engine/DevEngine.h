////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef DEVENGINE_H
#define DEVENGINE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/ResourceManager.h>
#include <Nero/camera/AdvancedCamera.h>
#include <Nero/engine/EngineSetting.h>
#include <Nero/engine/LoadingScreen.h>
#include <Nero/engine/DevEngineUI.h>
#include <Nero/scene/SceneManager.h>
#include <Nero/engine/Engine.h>
#include <Nero/utility/data/log.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
//BOOST
#include <boost/filesystem.hpp>
//EASYLOG
#include <easyloggingpp/easylogging++.h>
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
    class DevEngine : public Engine
    {
        public:
            //Method
                                    DevEngine(const unsigned int& windowWidth, const unsigned int& windowHeight);
                                    DevEngine(const unsigned int& windowWidth);
            virtual                ~DevEngine();

            template <typename T>
            void                    addScene(std::string sceneName);

        private:
            //Main Method
            void                    handleEvent() override;
            void                    update(const sf::Time& timeStep) override ;
            void                    render() override ;
            //Startup thread function
            int                     startEngine(bool& engineStarted, const float minTime);
            //
            void                    loadEngineSetting();
            void                    setupRenderWindow();
            void                    buildStartupScreen();

        private:
            //Startup Thread
            std::future<int>        m_StartEngineFuture;
            bool                    m_EngineStarted;
            //Startup Screen
            LoadingScreen::Ptr      m_LoadingScreen;
            //Engine UI
            DevEngineUI::Ptr        m_EngineUI;
            //Resource Manager
            ResourceManager::Ptr    m_ResourceManager;
            //Scene Setting
            EngineSetting           m_EngineSetting;
            //Task
            std::vector<std::function<void()>> m_TaskTable;
    };

    template <typename T>
    void DevEngine::addScene(std::string sceneName)
    {
        //Check Max Scene allowed
        if(m_TaskTable.size() > m_EngineSetting.maxScene)
        {
            nero_log("failed to add scene : " + sceneName);
            nero_log("only " + _s(m_EngineSetting.maxScene) + "scenes can be added");
            return;
        }

        m_TaskTable.push_back([this, sceneName]()
        {
            nero_log("adding scene : " + sceneName);
            m_EngineUI->addScene<T>(sceneName);

        });
    }
}
////////////////////////////////////////////////////////////
#endif // DEVENGINE_H
