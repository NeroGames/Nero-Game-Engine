////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/resource/MusicHolder.h>
#include <Nero/core/cpp/utility/Utility.h>
// BOOST
#include <experimental/filesystem>

////////////////////////////////////////////////////////////
namespace nero
{
    MusicHolder::MusicHolder()
    {
    }

    MusicHolder::MusicHolder(const Setting& setting)
        : ResourceHolder(setting)
    {
    }

    MusicHolder::~MusicHolder()
    {
        destroy();
    }

    void MusicHolder::destroy()
    {
    }

    bool MusicHolder::loadFile(const std::string& file)
    {
        std::experimental::filesystem::path filePath(file);

        std::unique_ptr<sf::Music>          music     = std::make_unique<sf::Music>();

        const std::string                   musicName = filePath.filename().stem().string();

        if(!music->openFromFile(filePath.string()))
        {
            nero_log("failed to load music : " + musicName);
            return false;
        }

        music->setLoop(true);

        bool added = addMusic(musicName, std::move(music));

        if(!added)
            return false;

        m_MusicTable.push_back(musicName);

        nero_log("loaded : " + musicName);

        return true;
    }

    void MusicHolder::loadDirectory()
    {
        if(m_SelectedDirectory == StringPool.BLANK)
        {
            nero_log("failed to load directory");
            return;
        }

        nero_log("Resource path : " + m_SelectedDirectory);

        std::experimental::filesystem::path folderPath(m_SelectedDirectory);

        if(!file::fileExist(m_SelectedDirectory))
        {
            nero_log("unable to load music resource");
            nero_log("folder not found : " + m_SelectedDirectory);
            return;
        }

        std::experimental::filesystem::directory_iterator it{folderPath};
        while(it != std::experimental::filesystem::directory_iterator{})
        {
            if(file::checkExtention(it->path().extension().string(), m_Setting.getStringTable("extension")))
            {
                loadFile(it->path().string());
            }

            it++;
        }
    }

    bool MusicHolder::addMusic(const std::string& musicName, std::unique_ptr<sf::Music> music)
    {
        auto inserted = m_MusicMap.insert(std::make_pair(musicName, std::move(music)));

        if(!inserted.second)
        {
            nero_log("failed to store music " + musicName);
            return false;
        }

        return true;
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

    void MusicHolder::clear()
    {
        // clear parent
        ResourceHolder::clear();

        // clear current
        m_MusicMap.clear();
        m_MusicTable.clear();
    }

} // namespace nero
