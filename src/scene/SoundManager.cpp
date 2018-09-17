////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/SoundManager.h>
////////////////////////////////////////////////////////////
namespace nero
{
    SoundManager::SoundManager(MusicHolder& musicHolder, SoundHolder& soundHolder):
        m_MusicHolder(musicHolder)
        ,m_SoundHolder(soundHolder)
        ,m_SoundSetting()
        ,m_CurrentMusic("")
    {
        for(int i=0; i < MAX_SOUND_POOL; i++)
        {
            m_SoundPool.push_back(sf::Sound());
        }
    }

    void SoundManager::playSound(std::string name)
    {
        auto& sound =  getSound();
        sound.setBuffer(m_SoundHolder.getSoundBuffer(name));
        sound.setVolume(m_SoundSetting.soundVolume);
        sound.setRelativeToListener(true);
        sound.play();
    }

    void SoundManager::playMusic(std::string name)
    {
        if(m_CurrentMusic != "")
        {
            stopMusic(m_CurrentMusic);
        }

        auto& music = m_MusicHolder.getMusic(name);
        music.setVolume(m_SoundSetting.musicVolume);
        music.play();

        m_CurrentMusic = name;
    }

    void SoundManager::stopMusic(std::string name)
    {
        auto& music = m_MusicHolder.getMusic(name);
        music.stop();

        m_CurrentMusic = "";
    }

    void SoundManager::setSoundVolume(float volume)
    {
        m_SoundSetting.soundVolume = volume;
    }

    void SoundManager::setMusicVolume(float volume)
    {
        m_SoundSetting.musicVolume = volume;

        if(m_CurrentMusic != "")
        {
            auto& music = m_MusicHolder.getMusic(m_CurrentMusic);
            music.setVolume(volume);
        }
    }

    sf::Sound& SoundManager::getSound()
    {
        for(auto& sound : m_SoundPool)
        {
            if(sound.getStatus() !=  sf::SoundSource::Playing)
                return sound;
        }

        auto& sound = m_SoundPool.front();
        sound.stop();
        return sound;
    }
}

