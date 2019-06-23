////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITOR_H
#define EDITOR_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/engine/CoreEngine.h>
#include <Nero/core/utility/LogUtil.h>
#include <Nero/editor/EditorInterface.h>
//STD
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
    class Editor : public CoreEngine
    {
        public:
                                    Editor();
            virtual                ~Editor() override;

            template <typename T>
            void                    addScene(const std::string& sceneName);

            template <typename T>
            void                    addLuaScene(const std::string& sceneName);

    private:
           //game loop
           void                     handleEvent()                    override;
           void                     update(const sf::Time& timeStep) override;
           void                     render()                         override;
           //startup
           int                      startEngine(bool& engineStarted, const float minTime);
           void                     buildStartupScreen();
           void                     setupRenderWindow();

         private:
            //Startup Thread
            std::future<int>        m_StartEngineFuture;
            bool                    m_EngineStarted;
            //Startup Screen
            //LoadingScreen::Ptr      m_LoadingScreen;
            //Editor Interface
            EditorInterface::Ptr    m_EditorInterface;
            //Resource Manager
            //ResourceManager::Ptr    m_ResourceManager;
            //Scene Setting
            //Task
            std::vector<std::function<void()>> m_BackgroundTaskTable;
    };

    template <typename T>
    void Editor::addScene(const std::string& sceneName)
    {
        nero_log(sceneName);
    }

    template <typename T>
    void Editor::addLuaScene(const std::string& sceneName)
    {
        nero_log(sceneName);
    }

}

#endif // EDITOR_H
