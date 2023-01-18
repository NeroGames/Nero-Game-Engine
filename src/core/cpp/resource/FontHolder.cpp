////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/resource/FontHolder.h>
#include <Nero/core/cpp/utility/Utility.h>
// BOOST
#include <experimental/filesystem>

////////////////////////////////////////////////////////////
namespace nero
{
    FontHolder::FontHolder()
    {
    }

    FontHolder::FontHolder(const Setting& setting)
        : ResourceHolder(setting)
    {
    }

    FontHolder::~FontHolder()
    {
        destroy();
    }

    void FontHolder::destroy()
    {
    }

    bool FontHolder::addFont(std::string name, std::unique_ptr<sf::Font> font)
    {
        auto inserted = m_FontMap.insert(std::make_pair(name, std::move(font)));

        if(!inserted.second)
        {
            nero_log("failed to store font " + name);
            return false;
        }

        m_FontTable.push_back(name);

        return true;
    }

    sf::Font& FontHolder::getFont(std::string name)
    {
        auto found = m_FontMap.find(name);
        assert(found != m_FontMap.end());

        return *found->second;
    }

    const sf::Font& FontHolder::getFont(std::string name) const
    {
        auto found = m_FontMap.find(name);
        assert(found != m_FontMap.end());

        return *found->second;
    }

    const sf::Font& FontHolder::getDefaultFont() const
    {
        nero_log(m_Setting.getString("default"));
        return getFont(m_Setting.getString("default"));
    }

    const std::vector<std::string>& FontHolder::getFontTable() const
    {
        return m_FontTable;
    }

    bool FontHolder::loadFile(const std::string& file)
    {
        std::experimental::filesystem::path filePath(file);

        std::unique_ptr<sf::Font>           font = std::make_unique<sf::Font>();

        const std::string                   name = filePath.filename().stem().string();

        if(!font->loadFromFile(filePath.string()))
        {
            nero_log("failed to load font : " + name);
            return false;
        }

        addFont(name, std::move(font));

        nero_log("loaded : " + name);

        return true;
    }

    void FontHolder::loadDirectory()
    {
        if(m_SelectedDirectory == StringPool.BLANK)
        {
            nero_log("failed to load directory");
            return;
        }

        nero_log("Resource path : " + m_SelectedDirectory);

        std::experimental::filesystem::path folderPath(m_SelectedDirectory);

        if(!file::directoryExist(m_SelectedDirectory))
        {
            nero_log("unable to load font resource");
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

    void FontHolder::clear()
    {
        // clear parent
        ResourceHolder::clear();

        // clear current
        m_FontMap.clear();
        m_FontTable.clear();
    }
} // namespace nero
