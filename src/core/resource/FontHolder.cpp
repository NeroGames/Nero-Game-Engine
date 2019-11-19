////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/resource/FontHolder.h>
#include <Nero/core/utility/Utility.h>
//BOOST
#include <experimental/filesystem>

////////////////////////////////////////////////////////////
namespace nero
{
    FontHolder::FontHolder()
    {

    }

	FontHolder::FontHolder(const Setting& setting) : ResourceHolder (setting)
	{

	}


	FontHolder::~FontHolder()
	{
		destroy();
	}

	void FontHolder::destroy()
	{

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

	const sf::Font& FontHolder::getDefaultFont()  const
    {
		return getFont(m_Setting.getString("default"));
    }

    const std::vector<std::string>& FontHolder::getFontTable() const
    {
        return m_FontTable;
    }

	void FontHolder::loadFile(const std::string &file)
	{
		std::experimental::filesystem::path filePath(file);

		std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();

		const std::string name = filePath.filename().stem().string();

		if (!font->loadFromFile(filePath.string()))
		{
			nero_log("failed to load font : " + name);
			assert(false);
		}

		addFont(name, std::move(font));

		nero_log("loaded : " + name);
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

		if(!directoryExist(m_SelectedDirectory))
        {
            nero_log("unable to load font resource");
			nero_log("folder not found : " + m_SelectedDirectory);
            assert(false);
        }

		std::experimental::filesystem::directory_iterator it{folderPath};
		while(it != std::experimental::filesystem::directory_iterator{})
        {
			if(checkExtention(it->path().extension().string(), m_Setting.getStringTable("extension")))
            {
				loadFile(it->path().string());
            }

            it++;
        }
    }
}
