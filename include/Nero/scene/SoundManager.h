////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/MusicHolder.h>
#include <Nero/resource/SoundHolder.h>
#include <Nero/scene/SceneUtil.h>
//SFML
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Listener.hpp>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
   class SoundManager
    {
        public:
            typedef std::shared_ptr<SoundManager>   Ptr;
        public:
                                SoundManager(MusicHolder& musicHolder, SoundHolder& soundHolder);

            void                playSound(std::string name);
            void                playMusic(std::string name);
            void                stopMusic(std::string name);
            void                setSoundVolume(float volume);
            void                setMusicVolume(float volume);
            void                pauseMusic();
            void                resumeMusic();
            void                stopMusic();
            float               getSoundVolume();
            float               getMusicVolume();

            float               increaseMusicVolume(float offset = 1.f);
            float               decreaseMusicVolume(float offset = 1.f);
            float               increaseSoundVolume(float offset = 1.f);
            float               decreaseSoundVolume(float offset = 1.f);

            /*//Listener
            void                setGlobalVolume(float volume);
            void                setListenerPosition(const sf::Vector3f& position);
            void                setListenerDirection(const sf::Vector3f& direction);
            void                setListenerUpVector(const sf::Vector3f& upVector);*/

            nlohmann::json      toJson();
            void                fromJson(nlohmann::json json);

        private:
            sf::Sound&          getSound();
            void                clampVolume(float& volume);

        private:
            MusicHolder&                    m_MusicHolder;
            SoundHolder&                    m_SoundHolder;
            std::string                     m_CurrentMusic;
            std::vector<sf::Sound>          m_SoundPool;
            float                           m_MusicVolume;
            float                           m_SoundVolume;
    };
}
#endif // SOUNDMANAGER_H
