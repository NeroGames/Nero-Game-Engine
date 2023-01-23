////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/engine/StartupScreen.h>
////////////////////////////////////////////////////////////
namespace nero
{
    StartupScreen::StartupScreen()
        : m_RenderWindow(nullptr)
        , m_ResourceManager(nullptr)
    {
        // empty
    }

    StartupScreen::~StartupScreen()
    {
        destroy();
    }

    void StartupScreen::destroy()
    {
        m_RenderWindow    = nullptr;
        m_ResourceManager = nullptr;
    }

    void StartupScreen::setRenderWindow(sf::RenderWindow* renderWindow)
    {
        m_RenderWindow = renderWindow;
    }

    void StartupScreen::setResourceManager(ResourceManager::Ptr resourceManager)
    {
        m_ResourceManager = resourceManager;
    }
} // namespace nero
