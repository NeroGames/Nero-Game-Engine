////////////////////////////////////////////////////////////
// Project :ProjectName:
// Copyright (c) :Date: :ProjectLead:
////////////////////////////////////////////////////////////
#include ":StartupScreenClass:.h"
#include <Nero/utility/Utility.h>

namespace :NameSpace:
{
    :StartupScreenClass::::StartupScreenClass:(): nero::StartupScreen()
    {

    }

    void :StartupScreenClass:::init()
    {
        /*
        //Build the Logo sprite
        m_LogoTexture.loadFromFile(nero::STARTUP_FOLDER + "/logo.png");
        m_LogoSprite.setTexture(m_LogoTexture);
        nero::centerOrigin(m_LogoSprite);
        m_LogoSprite.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, m_RenderWindow->getSize().x / 3.f));
        //*/
    }

    void :StartupScreenClass:::handleEvent(sf::Event& event)
    {
        //handle events
    }

    void :StartupScreenClass:::update(const sf::Time& timeStep)
    {
        //update
    }

    void :StartupScreenClass:::render()
    {
         //m_RenderWindow->draw(m_LogoSprite);
    }

    const sf::Color :StartupScreenClass:::getBackgroundColor() const
    {
        return sf::Color::White;
    }

    const float :StartupScreenClass:::getMinTime() const
    {
        return 10.f; //seconds
    }
}

