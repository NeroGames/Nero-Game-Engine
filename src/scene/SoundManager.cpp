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
        ,m_CurrentMusic("")
        ,m_MusicVolume(50.f)
        ,m_SoundVolume(50.f)
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
        sound.setVolume(m_SoundVolume);
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
        music.setVolume(m_MusicVolume);
        music.play();

        m_CurrentMusic = name;
    }

    void SoundManager::stopMusic(std::string name)
    {
        if(m_CurrentMusic == "")
            return;

        auto& music = m_MusicHolder.getMusic(name);
        music.stop();

        m_CurrentMusic = "";
    }

    void SoundManager::setSoundVolume(float volume)
    {
        m_SoundVolume = volume;

        clampVolume(m_SoundVolume);
    }

    void SoundManager::setMusicVolume(float volume)
    {
        m_MusicVolume = volume;

        clampVolume(m_MusicVolume);

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

    float SoundManager::getSoundVolume()
    {
        return m_SoundVolume;
    }

    float SoundManager::getMusicVolume()
    {
        return m_MusicVolume;
    }

     nlohmann::json SoundManager::toJson()
     {
         nlohmann::json json;

        json["sound_volume"] = m_SoundVolume;
        json["music_volume"] = m_MusicVolume;

        return json;
     }

    void SoundManager::fromJson(nlohmann::json json)
    {
        m_SoundVolume = json["sound_volume"];
        m_MusicVolume = json["music_volume"];
    }

    void SoundManager::pauseMusic()
    {
        if(m_CurrentMusic == "")
            return;

        auto& music = m_MusicHolder.getMusic(m_CurrentMusic);
        music.pause();
    }

    void SoundManager::resumeMusic()
    {
        if(m_CurrentMusic == "")
            return;

        auto& music = m_MusicHolder.getMusic(m_CurrentMusic);
        music.play();
    }

    void SoundManager::stopMusic()
    {
        if(m_CurrentMusic == "")
            return;

        auto& music = m_MusicHolder.getMusic(m_CurrentMusic);
        music.stop();
    }

    float SoundManager::increaseMusicVolume(float offset)
    {
        m_MusicVolume += offset;

        clampVolume(m_MusicVolume);

        if(m_CurrentMusic != "")
        {
            auto& music = m_MusicHolder.getMusic(m_CurrentMusic);
            music.setVolume(m_MusicVolume);
        }

        return m_MusicVolume;
    }

    float SoundManager::decreaseMusicVolume(float offset)
    {
        m_MusicVolume -= offset;

        clampVolume(m_MusicVolume);

        if(m_CurrentMusic != "")
        {
            auto& music = m_MusicHolder.getMusic(m_CurrentMusic);
            music.setVolume(m_MusicVolume);
        }

        return m_MusicVolume;
    }

    float SoundManager::increaseSoundVolume(float offset)
    {
        m_SoundVolume += offset;

        clampVolume(m_SoundVolume);

        return m_SoundVolume;
    }

    float SoundManager::decreaseSoundVolume(float offset)
    {
        m_SoundVolume -= offset;

        clampVolume(m_SoundVolume);

        return m_SoundVolume;
    }

    void SoundManager::clampVolume(float& volume)
    {
        if (volume < 0)
        {
            volume = 0;
        }
        else if (volume > 100)
        {
            volume = 100;
        }
    }
}

