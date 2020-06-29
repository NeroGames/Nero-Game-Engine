////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Adventure Scene
#include "LoadingScreen.h"
//NERO
#include <Nero/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace ng
{
    LoadingScreen::LoadingScreen(): nero::StartupScreen()
        ,mTimeCount(0)
    {

    }

    void LoadingScreen::init()
    {
        //The Textures(Images) for a Startup Screen should be located in the folder /startup
        //This folder is automatically created when the Engine start the first time

        //Build the title sprite
        mTitleTexture.loadFromFile(nero::STARTUP_FOLDER + "/adventure_scene.png");
        mTitleSprite.setTexture(mTitleTexture);
        nero::centerOrigin(mTitleSprite);
        mTitleSprite.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, 120.f));
        mTitleSprite.setScale(sf::Vector2f(1.1, 1.1f));
        //Build the logo sprite
        mLogoTexture.loadFromFile(nero::STARTUP_FOLDER + "/opp_aviator.png");
        mLogoSprite.setTexture(mLogoTexture);
        nero::centerOrigin(mLogoSprite);
        mLogoSprite.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, m_RenderWindow->getSize().y / 2.f));
        mLogoSprite.setScale(sf::Vector2f(1.5, 1.5f));
        //Build the loading sprite
        mLoadingTexture.loadFromFile(nero::STARTUP_FOLDER + "/loading.png");
        mLoadingSprite.setTexture(mLoadingTexture);
        nero::centerOrigin(mLoadingSprite);
        mLoadingSprite.setPosition(sf::Vector2f(m_RenderWindow->getSize().x / 2.f, 500.f));
    }

    void LoadingScreen::handleEvent(sf::Event& event)
    {
        //Empty
    }

    void LoadingScreen::update(const sf::Time& timeStep)
    {
        mTimeCount += timeStep.asSeconds();
        int alpha = std::abs(std::sin(mTimeCount))*255;
        mLoadingSprite.setColor(sf::Color(255, 255, 255, alpha));
    }

    void LoadingScreen::render()
    {
         m_RenderWindow->draw(mTitleSprite);
         m_RenderWindow->draw(mLogoSprite);
         m_RenderWindow->draw(mLoadingSprite);
    }

    const sf::Color LoadingScreen::getBackgroundColor() const
    {
        return sf::Color::White;
    }

    const float LoadingScreen::getMinTime() const
    {
        return 10.f; //second
    }
}

