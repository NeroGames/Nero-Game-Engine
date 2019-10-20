////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITOR_H
#define EDITOR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/engine/CoreEngine.h>
#include <Nero/editor/EditorInterface.h>
//STD
#include <future>
#include <Nero/editor/LoadingScreen.h>

////////////////////////////////////////////////////////////
namespace nero
{
    class Editor : public CoreEngine
    {
        public:
                                    Editor();
            virtual                ~Editor() override;

    private:
           //game loop
           void                     handleEvent()                    override;
           void                     update(const sf::Time& timeStep) override;
           void                     render()                         override;
           //startup
           int                      startEngine(bool& engineStarted, const int duration);
           void                     buildStartupScreen();
           void                     setupRenderWindow();

         private:
            //Startup Thread
            std::future<int>        m_StartEngineFuture;
            bool                    m_EngineStarted;
            //Startup Screen
            LoadingScreen::Ptr      m_LoadingScreen;
            //Editor Interface
            EditorInterface::Ptr    m_Interface;
            //Resource Manager
            //ResourceManager::Ptr    m_ResourceManager;
            //background tasks
            std::vector<std::function<void()>> m_BackgroundTaskTable;
    };

}

#endif // EDITOR_H
