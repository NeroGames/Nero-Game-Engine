////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2018 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef MUSICHOLDER_H
#define MUSICHOLDER_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Audio/Music.hpp>
//JSON
#include <json/json.hpp>
//STD
#include <string>
#include <map>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class MusicHolder
    {
        public:
                                                MusicHolder();

            void                                load();
            sf::Music&                          getMusic(std::string name);
            void                                addMusic(const std::string& name, std::unique_ptr<sf::Music> music);
            const std::vector<std::string>&     getMusicTable() const;

        private:
            std::map<std::string, std::unique_ptr<sf::Music>>   m_MusicMap;
            std::vector<std::string>                            m_MusicTable;
            nlohmann::json                                      m_Configuration;
    };
}

#endif // MUSICHOLDER_H
