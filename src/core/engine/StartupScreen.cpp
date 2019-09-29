////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/engine/StartupScreen.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    StartupScreen::StartupScreen():
        m_RenderWindow(nullptr)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    StartupScreen::~StartupScreen()
    {
        m_RenderWindow = nullptr;
    }

    ////////////////////////////////////////////////////////////
    void StartupScreen::setRenderWindow(sf::RenderWindow* renderWindow)
    {
        m_RenderWindow = renderWindow;
    }
}
