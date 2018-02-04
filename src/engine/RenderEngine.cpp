////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/engine/RenderEngine.h>
#include <NERO/Utility.h>
//SFGUI
#include <SFGUI/Window.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    ////////////////////////////////////////////////////////////
    RenderEngine::RenderEngine(const sf::String& title, const float& windowWidth, const float& windowHeight):
        Engine(title, windowWidth, windowHeight),
        m_Sfgui(),
        m_Camera(sf::Vector2f(windowWidth, windowHeight)),
        m_ObjectManager(m_RenderCanvas, m_ResourceManager.Texture)

    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates(); //if not, SFGUI will have some undetermined behaviour

        m_ResourceManager.Texture.load("Resource/Texture");
        m_ResourceManager.Font.load("Resource/Font");

        // Create our SFML canvas window
        m_RenderCanvas = sfg::Canvas::Create();
        auto canvas_window = sfg::Window::Create();
            canvas_window->SetStyle(sfg::Window::Style::BACKGROUND);
            canvas_window->SetRequisition(sf::Vector2f(windowWidth, windowHeight));
            canvas_window->Add(m_RenderCanvas);
        m_Desktop.Add(canvas_window);

        m_CanvasDefaultView = m_RenderCanvas->GetView();
        m_CanvasDefaultView.setCenter((windowWidth-22.f)/2.f, (windowHeight-22.f)/2.f);
        m_CanvasDefaultView.setSize(sf::Vector2f(windowWidth-22.f, windowHeight-22.f));



        //Render a empty scene
        setScene<Scene>("Nero Render Engine");
    }

    ////////////////////////////////////////////////////////////
    RenderEngine::~RenderEngine()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::handleEvents()
    {
        sf::Event event;

        while (m_Window.pollEvent(event))
        {
            m_Desktop.HandleEvent(event);
            m_Camera.handleEvent(event);
            m_Scene->handleEvent(event);

            switch(event.type)
            {
                case sf::Event::Closed:
                    m_Window.close();
                    break;
                //Keyboard events
                case sf::Event::KeyPressed:
                    handleKeyboardInput(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    handleKeyboardInput(event.key.code, false);
                    break;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::update(const sf::Time& time_step)
    {
        m_Desktop.Update(time_step.asSeconds());

        m_Camera.update(time_step);
        m_RenderCanvas->SetView(m_Camera.getView());

        m_Scene->update(&m_SceneSettings);
    }

    ////////////////////////////////////////////////////////////
    void RenderEngine::render()
    {
        m_Window.clear();

            m_RenderCanvas->Clear(sf::Color::Black);

                m_Scene->render();
                m_Scene->renderShape();

                m_RenderCanvas->SetView(m_CanvasDefaultView);
                    m_Scene->renderOnFrontScreen();
                m_RenderCanvas->SetView(m_Camera.getView());

                m_RenderCanvas->Display();

            m_Sfgui.Display(m_Window);

        m_Window.display();
    }


    ////////////////////////////////////////////////////////////
    void RenderEngine::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //Handle only key pressed inputs
        if(isPressed)
        {
            // R to reset the scene
            if(key == sf::Keyboard::R && CTRL())
            {
                m_Camera.reinitialize();
                m_Scene = m_CreateScene();
                m_ObjectManager.loadScene(m_SceneJson);
                m_ObjectManager.setWorld(m_Scene->m_World);
                m_ObjectManager.buildScene(m_Scene->m_RootObject);
                m_Scene->init();

            }

            // P to pause the scene
            else if(key == sf::Keyboard::P && CTRL())
                m_SceneSettings.pause = !m_SceneSettings.pause;
        }
    }
}
