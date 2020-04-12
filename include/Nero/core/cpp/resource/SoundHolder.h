////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SOUNDHOLDER_H
#define SOUNDHOLDER_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
//SFML
#include <SFML/Audio/SoundBuffer.hpp>
//JSON
#include <json/json.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
	class SoundHolder : public ResourceHolder
    {
		public: //utility
			typedef std::shared_ptr<SoundHolder> Ptr;
			using	ResourceHolder::loadDirectory;
			using	ResourceHolder::loadFile;

        public:
												SoundHolder();
												SoundHolder(const Setting& setting);
			virtual							   ~SoundHolder()						override;

			virtual void						loadDirectory()						override;
			virtual bool						loadFile(const std::string& file)	override;
			virtual void						destroy()							override;
			virtual void						clear() override;

				  sf::SoundBuffer&				getSoundBuffer(std::string name);
			const sf::SoundBuffer&				getSoundBuffer(std::string name)	const;
			const std::vector<std::string>&		getSoundBufferTable()				const;

        private:
			bool								addSoundBuffer(std::string name, std::unique_ptr<sf::SoundBuffer> soundBuffer);

        private:
            std::map<std::string, std::unique_ptr<sf::SoundBuffer>>     m_SoundBufferMap;
            std::vector<std::string>                                    m_SoundBufferTable;
    };
}
#endif // SOUNDHOLDER_H
