////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/engine/StartupScreen.h>
////////////////////////////////////////////////////////////
namespace nero
{
    StartupScreen::StartupScreen():
        m_RenderWindow(nullptr)
	   ,m_ResourceManager(nullptr)
    {
        //Empty
    }

    StartupScreen::~StartupScreen()
    {
		destroy();
    }

	void StartupScreen::destroy()
	{
		m_RenderWindow = nullptr;
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

	void StartupScreen::setFontHolder(FontHolder::Ptr fontHolder)
	{
		m_FontHolder = fontHolder;
	}

	void StartupScreen::setTextureHolder(TextureHolder::Ptr textureHolder)
	{
		m_TextureHolder = textureHolder;
	}

	void StartupScreen::setAnimationHolder(AnimationHolder::Ptr animationHolder)
	{
		m_AnimationHodler = animationHolder;
	}

	void StartupScreen::setSoundHolder(SoundHolder::Ptr soundHolder)
	{
		m_SoundHolder = soundHolder;
	}

	void StartupScreen::setMusicHolder(MusicHolder::Ptr musicHolder)
	{
		m_MusicHolder = musicHolder;
	}
}
