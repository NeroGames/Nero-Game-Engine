#include <Nero/resource/FontHolder.h>
#include <Nero/utility/Utility.h>
#include <boost/filesystem.hpp>

namespace nero
{
    FontHolder::FontHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["font"];
    }

    void FontHolder::addFont(std::string name, std::unique_ptr<sf::Font> font)
    {
        auto newFont = m_FontMap.insert(std::make_pair(name, std::move(font)));
        assert(newFont.second);

        m_FontTable.push_back(name);
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

    const   sf::Font& FontHolder::getDefaultFont()  const
    {
        return getFont(m_Configuration["default"].get<std::string>());
    }

    const std::vector<std::string>& FontHolder::getFontTable() const
    {
        return m_FontTable;
    }

    void FontHolder::load()
    {
        const std::string folder_name = m_Configuration["folder"].get<std::string>();

        nero_log("Resource path : " + folder_name);

        using namespace boost::filesystem;
        path folder_path(folder_name);

        if(!exists(folder_name))
        {
            nero_log("unable to load font resource");
            nero_log("folder not found : " + folder_name);
            assert(false);
        }

        directory_iterator it{folder_path};
        while(it != directory_iterator{})
        {
            if(checkExtention(it->path().extension().string(), m_Configuration["extension"]))
            {
                std::unique_ptr<sf::Font> font = make_unique<sf::Font>();

                const std::string name = it->path().filename().stem().string();

                if (!font->loadFromFile(it->path().string()))
                {
                    nero_log("failed to load font : " + name);
                    assert(false);
                }

                addFont(name, std::move(font));

                nero_log("loaded : " + name);
            }

            it++;
        }
    }
}
