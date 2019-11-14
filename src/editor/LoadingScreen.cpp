////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/editor/LoadingScreen.h>
#include <Nero/core/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    LoadingScreen::LoadingScreen(): StartupScreen()
    {

    }

    ////////////////////////////////////////////////////////////
    void LoadingScreen::handleEvent(sf::Event& event)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void LoadingScreen::update(const sf::Time& timeStep)
    {
        //move right left
        if(m_LoadingBall.getPosition().x < m_MaxRight && m_Direction > 0)
        {
            float boo = m_MaxRight/m_LoadingBall.getPosition().x;

             m_LoadingBall.move(sf::Vector2f(2.f*boo*boo*boo, 0.f));

             if(m_LoadingBall.getPosition().x >= m_MaxRight)
                m_Direction = -m_Direction;
        }

        else if (m_LoadingBall.getPosition().x > m_MaxLeft && m_Direction < 0)
        {
            float boo = m_LoadingBall.getPosition().x/m_MaxLeft;

             m_LoadingBall.move(sf::Vector2f(-2.f*boo*boo*boo, 0.f));

             if(m_LoadingBall.getPosition().x <= m_MaxLeft)
                m_Direction = -m_Direction;
        }
    }

    ////////////////////////////////////////////////////////////
    void LoadingScreen::render()
    {
        m_RenderWindow->draw(m_LogoSprite);
        m_RenderWindow->draw(m_LoadingRectRight);
        m_RenderWindow->draw(m_LoadingRectLeft);
        m_RenderWindow->draw(m_LoadingBall);
        m_RenderWindow->draw(m_CopyrightsSprite);
    }

    ////////////////////////////////////////////////////////////
	const sf::Color LoadingScreen::getCanvasColor() const
    {
        return sf::Color::White;
    }

    ////////////////////////////////////////////////////////////
	const float LoadingScreen::getDuration() const
    {
        return 3.f;
    }

    ////////////////////////////////////////////////////////////
    void LoadingScreen::init()
    {
        //Build the logo sprite
        m_LogoTexture.loadFromFile("startup/nero_logo.png");
        m_LogoSprite.setTexture(m_LogoTexture);
        centerOrigin(m_LogoSprite);
        m_LogoSprite.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, m_RenderWindow->getSize().y / 3.f));
        m_LogoSprite.setScale(sf::Vector2f(1.001f, 1.001f));
        //Build the copyrights sprite
        m_CopyrightsTexture.loadFromFile("startup/copyrights.png");
        m_CopyrightsSprite.setTexture(m_CopyrightsTexture);
        centerOrigin(m_CopyrightsSprite);
        m_CopyrightsSprite.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, m_RenderWindow->getSize().y - 50.f));
        m_CopyrightsSprite.setScale(sf::Vector2f(1.001f, 1.001f));
        //Build the left loading rectangle
        m_LoadingRectLeft.setSize(sf::Vector2f(15.f, 15.f));
        m_LoadingRectLeft.setOutlineThickness(2.f);
        m_LoadingRectLeft.setOutlineColor(sf::Color::Black);
        m_LoadingRectLeft.setFillColor(sf::Color::White);
        m_LoadingRectLeft.setOrigin(sf::Vector2f(10.f, 10.f));
        m_LoadingRectLeft.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f - 250.f, (m_RenderWindow->getSize().y / 3.f) + 300.f));
        m_LoadingRectLeft.setRotation(45.f);
        //Build the right loading rectangle
        m_LoadingRectRight = m_LoadingRectLeft;
        m_LoadingRectRight.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f + 250.f, (m_RenderWindow->getSize().y / 3.f) + 300.f));
        //Build the loading ball
        m_LoadingBall.setRadius(5.f);
        m_LoadingBall.setOrigin(sf::Vector2f(5.f, 5.f));
        m_LoadingBall.setFillColor(sf::Color::Black);
        m_LoadingBall.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, (m_RenderWindow->getSize().y / 3.f) + 300.f));
        //Set default parameters for the ball movement
        m_MaxLeft = m_LoadingRectLeft.getPosition().x + 30.f;
        m_MaxRight = m_LoadingRectRight.getPosition().x - 30.f;
        m_Direction = 1;
    }
}
