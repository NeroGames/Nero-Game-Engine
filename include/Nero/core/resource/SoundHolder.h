////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SOUNDHOLDER_H
#define SOUNDHOLDER_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Audio/SoundBuffer.hpp>
//JSON
#include <json/json.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
    class SoundHolder
    {
		public: //utility
			typedef std::shared_ptr<SoundHolder>    Ptr;

        public:
                    SoundHolder();
            void    load();

                    sf::SoundBuffer& getSoundBuffer(std::string name);
            const   sf::SoundBuffer& getSoundBuffer(std::string name)   const;

            const std::vector<std::string>& getSoundBufferTable()       const;

        private:
            void    addSoundBuffer(std::string name, std::unique_ptr<sf::SoundBuffer> soundBuffer);

        private:
            std::map<std::string, std::unique_ptr<sf::SoundBuffer>>     m_SoundBufferMap;
            std::vector<std::string>                                    m_SoundBufferTable;
            nlohmann::json                                              m_Configuration;
    };
}
#endif // SOUNDHOLDER_H
