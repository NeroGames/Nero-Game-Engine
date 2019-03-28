////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/resource/SoundHolder.h>
#include <Nero/utility/Utility.h>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    SoundHolder::SoundHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["sound"];
    }

    void SoundHolder::addSoundBuffer(std::string name, std::unique_ptr<sf::SoundBuffer> soundBuffer)
    {
        auto newSoundBuffer = m_SoundBufferMap.insert(std::make_pair(name, std::move(soundBuffer)));
        assert(newSoundBuffer.second);

        m_SoundBufferTable.push_back(name);
    }

    sf::SoundBuffer& SoundHolder::getSoundBuffer(std::string name)
    {
        auto found = m_SoundBufferMap.find(name);
        assert(found != m_SoundBufferMap.end());

        return *found->second;
    }

    const sf::SoundBuffer& SoundHolder::getSoundBuffer(std::string name) const
    {
        auto found = m_SoundBufferMap.find(name);
        assert(found != m_SoundBufferMap.end());

        return *found->second;
    }

    const std::vector<std::string>& SoundHolder::getSoundBufferTable() const
    {
        return m_SoundBufferTable;
    }

    void SoundHolder::load()
    {
        const std::string folder_name = m_Configuration["folder"].get<std::string>();

        nero_log("resource path : " + folder_name);

        using namespace boost::filesystem;
        path folder_path(folder_name);

        if(!exists(folder_name))
        {
            nero_log("unable to load sound resource");
            nero_log("folder not found : " + folder_name);
            assert(false);
        }

        directory_iterator it{folder_path};
        while(it != directory_iterator{})
        {
            if(checkExtention(it->path().extension().string(), m_Configuration["extension"]))
            {

                std::unique_ptr<sf::SoundBuffer> soundBuffer = make_unique<sf::SoundBuffer>();

                const std::string name = it->path().filename().stem().string();

                if (!soundBuffer->loadFromFile(it->path().string()))
                {
                    nero_log("unable to load sound : " + name);
                    assert(false);
                }

                addSoundBuffer(name, std::move(soundBuffer));

                nero_log("loaded : " + name);
            }

            it++;
        }
    }
}
