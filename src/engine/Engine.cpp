////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/Engine.h>
#include <Nero/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    Engine::Engine(const unsigned int& windowWidth, const unsigned int& windowHeight, const std::string& windowTitle):
         m_WindowTitle(windowTitle)
        ,m_WindowWidth(windowWidth)
        ,m_WindowHeight(windowHeight)
        ,m_Window(sf::VideoMode(windowWidth, windowHeight), windowTitle, sf::Style::Close)
        ,m_ElapsedTime()
        ,m_FrameCount(0)
        ,m_FramePerSecond(0.f)
        ,m_TimePerFrame(0.f)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    Engine::~Engine()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Engine::run()
    {
        //Start a clock
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;

        while(m_Window.isOpen())
        {
            //Accumulate the time elapsed at each loop
            sf::Time elapsedTime = clock.restart();
            timeSinceLastUpdate += elapsedTime;

            //When the time comes over the value of "TIME_PER_FRAME" do --> 1 --> 2 then do --> 2 --> 3
            //TIME_PER_FRAME is constant with a value of 1/60 second (the game is update 60 time per second)
            while(timeSinceLastUpdate > TIME_PER_FRAME)
            {
                //retrieve 1/60 second in the accumulated time
                timeSinceLastUpdate -= TIME_PER_FRAME;

                //1... handle user inputs
                handleEvent();
                //2... update the game
                update(TIME_PER_FRAME);
            }

            //3... Compute Frame rate
            computeFrameRate(elapsedTime);
            //4... render the game
            render();
        }
    }

    ////////////////////////////////////////////////////////////
    void Engine::computeFrameRate(sf::Time timeStep)
    {
        //Accumulate data for on 1 second
        m_ElapsedTime       += timeStep;
        m_FrameCount        += 1;

        //Then compute the frame rate
        if(m_ElapsedTime >= sf::seconds(1.0f))
        {
            m_FramePerSecond    = m_FrameCount;
            m_TimePerFrame      = m_ElapsedTime.asSeconds() / m_FrameCount;

            m_ElapsedTime      -= sf::seconds(1.0f);
            m_FrameCount        = 0;
        }
    }

    ////////////////////////////////////////////////////////////
    std::string Engine::getWindowTitle() const
    {
        return m_WindowTitle;
    }

    ////////////////////////////////////////////////////////////
    void Engine::setWindowTitle(const std::string& windowTitle)
    {
        m_Window.setTitle(windowTitle);
    }

    ////////////////////////////////////////////////////////////
    unsigned int Engine::getWindowWidth() const
    {
        return m_WindowWidth;
    }

    ////////////////////////////////////////////////////////////
    unsigned int Engine::getWindowHeight() const
    {
        return m_WindowHeight;
    }

    ////////////////////////////////////////////////////////////
    float Engine::getFrameRate() const
    {
        return m_FramePerSecond;
    }

    ////////////////////////////////////////////////////////////
    float Engine::getFrameTime() const
    {
        return m_TimePerFrame;
    }
}
////////////////////////////////////////////////////////////
