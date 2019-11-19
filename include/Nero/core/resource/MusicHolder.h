////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MUSICHOLDER_H
#define MUSICHOLDER_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/resource/ResourceHolder.h>
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
	class MusicHolder : public ResourceHolder
    {
		public: //utility
			typedef std::shared_ptr<MusicHolder>    Ptr;
			using ResourceHolder::loadDirectory;


        public:
                                                MusicHolder();
												MusicHolder(const Setting& setting);
			virtual							   ~MusicHolder()						override;

			virtual void						loadDirectory()						override;
			virtual void						loadFile(const std::string& file)	override;
			virtual void						destroy()							override;


            void                                load();
            sf::Music&                          getMusic(std::string name);
            void                                addMusic(const std::string& name, std::unique_ptr<sf::Music> music);
            const std::vector<std::string>&     getMusicTable() const;

        private:
            std::map<std::string, std::unique_ptr<sf::Music>>   m_MusicMap;
            std::vector<std::string>                            m_MusicTable;
    };
}
#endif // MUSICHOLDER_H
