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
#include <NERO/engine/Engine.h>
#include <NERO/Utility.h>
////////////////////////////////////////////////////////////

namespace nero
{
    ////////////////////////////////////////////////////////////
    Engine::Engine(const sf::String& winTitle, const float& winWidth, const float& winHeight):
        m_WinTitle(winTitle),
        m_WinWidth(winWidth),
        m_WinHeight(winHeight),
        m_Window(sf::VideoMode(winWidth, winHeight), winTitle, sf::Style::Close)
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

        while (m_Window.isOpen())
        {
            //Accumulate the time passed at each loop
            timeSinceLastUpdate += clock.restart();

            //When the the time come over the value of "TIME_PER_FRAME" then ... 1 2 3
            //TIME_PER_FRAME is constant with a value of 1/60 second (the game is update 60 time per second)
            while(timeSinceLastUpdate > TIME_PER_FRAME)
            {
                //retrieve 1/60 second in the accumulated time
                timeSinceLastUpdate -= TIME_PER_FRAME;

                //1... handle user input
                handleEvents();
                //2... update the game
                update(TIME_PER_FRAME);
                //3... render the game
                render();
            }
        }
    }

    ////////////////////////////////////////////////////////////
    sf::String Engine::getWinTitle() const
    {
        return m_WinTitle;
    }

    ////////////////////////////////////////////////////////////
    void Engine::setWinTitle(const sf::String& winTitle)
    {
        m_Window.setTitle(winTitle);
    }

    ////////////////////////////////////////////////////////////
    float Engine::getWinWidth() const
    {
        return m_WinWidth;
    }

    ////////////////////////////////////////////////////////////
    float Engine::getWinHeight() const
    {
        return m_WinHeight;
    }
}
