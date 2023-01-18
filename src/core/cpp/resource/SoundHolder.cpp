////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// NERO
#include <Nero/core/cpp/resource/SoundHolder.h>
#include <Nero/core/cpp/utility/Utility.h>
// BOOST
#include <experimental/filesystem>
////////////////////////////////////////////////////////////
namespace nero
{
    SoundHolder::SoundHolder()
    {
    }

    SoundHolder::SoundHolder(const Setting& setting)
        : ResourceHolder(setting)
    {
    }

    SoundHolder::~SoundHolder()
    {
        destroy();
    };

    void SoundHolder::destroy()
    {
    }

    bool SoundHolder::addSoundBuffer(std::string name, std::unique_ptr<sf::SoundBuffer> soundBuffer)
    {
        auto inserted = m_SoundBufferMap.insert(std::make_pair(name, std::move(soundBuffer)));

        if(!inserted.second)
        {
            nero_log("failed to store sound " + name);
            return false;
        }

        m_SoundBufferTable.push_back(name);

        return true;
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

    bool SoundHolder::loadFile(const std::string& file)
    {
        std::experimental::filesystem::path filePath(file);

        std::unique_ptr<sf::SoundBuffer>    soundBuffer = std::make_unique<sf::SoundBuffer>();

        const std::string                   name        = filePath.filename().stem().string();

        if(!soundBuffer->loadFromFile(filePath.string()))
        {
            nero_log("unable to load sound : " + name);
            return false;
        }

        addSoundBuffer(name, std::move(soundBuffer));

        nero_log("loaded : " + name);

        return true;
    }

    void SoundHolder::loadDirectory()
    {
        if(m_SelectedDirectory == StringPool.BLANK)
        {
            nero_log("failed to load directory");
            return;
        }

        nero_log("resource path : " + m_SelectedDirectory);

        std::experimental::filesystem::path folderPath(m_SelectedDirectory);

        if(!file::directoryExist(m_SelectedDirectory))
        {
            nero_log("unable to load sound resource");
            nero_log("folder not found : " + m_SelectedDirectory);
            assert(false);
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

    void SoundHolder::clear()
    {
        // clear parent
        ResourceHolder::clear();

        // clear current
        m_SoundBufferMap.clear();
        m_SoundBufferTable.clear();
    }
} // namespace nero
