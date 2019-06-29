#include <Nero/editor/Editor.h>
#include <Nero/editor/Interface.h>
#include <memory>
#include <Nero/core/utility/FileUtil.h>

namespace  nero
{
    Editor::Editor() : CoreEngine()
      ,m_EngineStarted(true)
    {
        setupRenderWindow();

        m_Interface = std::make_unique<Interface>(m_Window);
    }

    Editor::~Editor()
    {
       ImGui::SFML::Shutdown();
    }

    void Editor::handleEvent()
    {
        sf::Event event;
        while(m_Window.pollEvent(event))
        {
            if(!m_EngineStarted)
            {
                //StartupScreen Event
                //m_LoadingScreen->handleEvent(event);
            }
            else
            {
                //EngineUI Event
                m_Interface->handleEvent(event);
            }
        }
    }

    void Editor::update(const sf::Time& timeStep)
    {
        if(!m_EngineStarted)
        {
            //Update StartupScreen
             //m_LoadingScreen->update(timeStep);
        }
        else
        {
            //Render EngineUI
            m_Interface->update(timeStep);
            m_Interface->updateFrameRate(getFrameRate(), getFrameTime());
        }
    }

    void Editor::render()
    {
        //Render StartupScreen
        if(!m_EngineStarted)
        {
            /*m_Window.clear(m_LoadingScreen->getBackgroundColor());

                m_LoadingScreen->render();

            m_Window.display();*/
        }
        //Render EngineUI
        else
        {
            m_Window.clear(EngineConstant.CANVAS_CLEAR_COLOR);

                m_Interface->render();

            m_Window.display();
        }
    }

    void Editor::setupRenderWindow()
    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();
        m_Window.setPosition(sf::Vector2i(m_Window.getPosition().x, 15));

        removeFile(getPath({"imgui"}, ".ini"));

        //setup IMGUI
        ImGui::SFML::Init(m_Window);
    }

    void Editor::buildStartupScreen()
    {
        /*m_LoadingScreen = make_unique<LoadingScreen>();
        m_LoadingScreen->setRenderWindow(&m_Window);
        m_LoadingScreen->init();*/
    }
}
