////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2018 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/MusicHolder.h>
#include <Nero/utility/Utility.h>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    MusicHolder::MusicHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["music"];
    }

    void MusicHolder::load()
    {
        const std::string folder_name = m_Configuration["folder"].get<std::string>();

        nero_log("Resource path : " + folder_name);

        using namespace boost::filesystem;
        path folder_path(folder_name);

        if(!exists(folder_name))
        {
            nero_log("unable to load music resource");
            nero_log("folder not found : " + folder_name);
            assert(false);
        }

        directory_iterator it{folder_path};
        while(it != directory_iterator{})
        {
            if(checkExtention(it->path().extension().string(), m_Configuration["extension"]))
            {
                std::unique_ptr<sf::Music> music = make_unique<sf::Music>();

                const std::string musicName = it->path().filename().stem().string();

                if (!music->openFromFile(it->path().string()))
                {
                    nero_log("failed to load music : " + musicName);
                    continue;
                }

                music->setLoop(true);

                addMusic(musicName, std::move(music));
                m_MusicTable.push_back(musicName);

                nero_log("loaded : " + musicName);
            }

            it++;
        }
    }

    void MusicHolder::addMusic(const std::string& musicName, std::unique_ptr<sf::Music> music)
    {
        auto inserted = m_MusicMap.insert(std::make_pair(musicName, std::move(music)));

        if(!inserted.second)
        {
            nero_log("failed to store music " + musicName);
        }
    }

    sf::Music& MusicHolder::getMusic(std::string name)
    {
        auto found = m_MusicMap.find(name);
        assert(found != m_MusicMap.end());

        return *found->second;
    }

    const std::vector<std::string>& MusicHolder::getMusicTable() const
    {
        return m_MusicTable;
    }

}
