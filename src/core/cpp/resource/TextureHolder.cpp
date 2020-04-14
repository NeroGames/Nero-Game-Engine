////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/resource/TextureHolder.h>
#include <Nero/core/cpp/utility/Utility.h>
//BOOST
#include <experimental/filesystem>
////////////////////////////////////////////////////////////
namespace nero
{
	TextureHolder::TextureHolder()
    {

    }

	TextureHolder::TextureHolder(const Setting& setting) : ResourceHolder(setting)
	{

	}


	TextureHolder::~TextureHolder()
	{
		destroy();
	}

	void TextureHolder::destroy()
	{

	}

	bool TextureHolder::loadFile(const std::string& file)
	{
		std::experimental::filesystem::path filePath(file);

		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

		const std::string textureName = filePath.filename().stem().string();

		if (!texture->loadFromFile(filePath.string()))
		{
			nero_log("failed to load texture : " + textureName);

			return false;
		}

		//Read the .txt file to retrieve sprite bound
		std::string txtHelper   = file::replaceExtension(filePath.string(), "txt");
		std::string jsonHelper  = file::replaceExtension(filePath.string(), "json");

		std::map<std::string, std::map<std::string, sf::IntRect>> spriteMap;
		std::vector<std::string>                                  spriteTable;

		if(file::fileExist(jsonHelper))
		{
			nlohmann::json helper = file::loadJson(jsonHelper, true);

			nlohmann::json frame_table = helper["frames"];

			for (auto& frame : frame_table)
			{
				std::string spriteName      = file::removeFileExtension(frame["filename"].get<std::string>());
				int rectLeft                = frame["frame"]["x"];
				int rectTop                 = frame["frame"]["y"];
				int rectWidth               = frame["frame"]["w"];
				int rectHeight              = frame["frame"]["h"];

				sf::IntRect spriteBound = sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight);

				spriteMap[textureName][spriteName] = spriteBound;
				spriteTable.push_back(spriteName);
				//addSpriteBound(textureName, spriteName, spriteBound);
				//m_SpriteTable.push_back(spriteName);

				nero_log("loaded : " + spriteName);
			}
		}
		else if (file::fileExist(txtHelper))// .txt helper file exist, the texture get several sprites
		{
			/*std::ifstream inStream(txtHelper);
			//Split each line to retrieve the necessary information
			const std::string separatorString = m_Setting.getString("separator");
			char separator = separatorString[0];
			std::string line;

			while (std::getline(inStream, line))
			{
				std::vector<std::string> sprite = splitString(line, separator);

				std::string spriteName      = sprite[0];
				int rectLeft                = std::stoi(sprite[1]);
				int rectTop                 = std::stoi(sprite[2]);
				int rectWidth               = std::stoi(sprite[3]);
				int rectHeight              = std::stoi(sprite[4]);

				sf::IntRect spriteBound = sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight);

				addSpriteBound(textureName, spriteName, spriteBound);
				m_SpriteTable.push_back(spriteName);

				nero_log("loaded : " + spriteName);
			}*/
		}
		else //helper file not exist, texture is the sprite
		{
			sf::IntRect spriteBound = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);

			spriteMap[textureName][textureName] = spriteBound;
			spriteTable.push_back(textureName);

			//addSpriteBound(textureName, textureName, spriteBound);
			//m_SpriteTable.push_back(textureName);

			nero_log("loaded : " + textureName);
		}

		//Insert the texture in m_TextureMap
		bool added =  addTexture(textureName, std::move(texture));

		if(!added)
			return false;

		addSpriteBound(spriteMap);
		addSprite(spriteTable);

		return true;
	}

	void TextureHolder::addSpriteBound(std::map<std::string, std::map<std::string, sf::IntRect>> spriteMap)
	{
		for (auto const& entry : spriteMap)
		{
			m_SpriteMap[entry.first] = entry.second;
			/*for (auto const& bound : entry.second)
			{
				m_SpriteMap[entry.first][bound.first] = bound.second;
			}*/
		}
	}

	void TextureHolder::addSprite(std::vector<std::string> spriteTable)
	{
		for(const std::string sprite : spriteTable)
		{
			m_SpriteTable.push_back(sprite);
		}
	}

	void TextureHolder::loadDirectory()
    {
		if(m_SelectedDirectory == StringPool.BLANK)
		{
			nero_log("failed to load directory");
			return;
		}

		nero_log("loading all texture in directory : " + m_SelectedDirectory);

		std::experimental::filesystem::path directoryPath(m_SelectedDirectory);

		if(!file::directoryExist(m_SelectedDirectory))
        {
			nero_log("failed to load texture resource");
			nero_log("directory not found : " + m_SelectedDirectory);
            return;
        }

        //Iterate over files in the folder
		std::experimental::filesystem::directory_iterator it{directoryPath};
		while (it != std::experimental::filesystem::directory_iterator{})
        {
            //When we found a Texture
			if(file::checkExtention(it->path().extension().string(), m_Setting.getStringTable("extension")))
            {
                //Load the Texture
				loadFile(it->path().string());
            }

            it++;
        }
    }

	bool TextureHolder::addTexture(std::string textureName, std::unique_ptr<sf::Texture> texture)
    {
        auto inserted = m_TextureMap.insert(std::make_pair(textureName, std::move(texture)));

        if(!inserted.second)
        {
            nero_log("failed to store texture " + textureName);
			return false;
        }

		return true;
    }

    void TextureHolder::addSpriteBound(std::string textureName, std::string spriteName, sf::IntRect spriteBound)
    {
        m_SpriteMap[textureName][spriteName] = spriteBound;
    }

    sf::Texture& TextureHolder::getTexture(std::string textureName)
    {
        auto found = m_TextureMap.find(textureName);
        assert(found != m_TextureMap.end());

        return *found->second;
    }

    const sf::Texture& TextureHolder::getTexture(std::string textureName) const
    {
        auto found = m_TextureMap.find(textureName);
        assert(found != m_TextureMap.end());

        return *found->second;
    }

    const sf::Texture& TextureHolder::getSpriteTexture(std::string spriteName) const
    {
        std::string textureName;

        for(auto t_it = m_SpriteMap.begin(); t_it != m_SpriteMap.end(); t_it++)
        {
            for(auto r_it = t_it->second.begin(); r_it != t_it->second.end(); r_it++)
            {
               if(r_it->first == spriteName)
                    return getTexture(t_it->first);
            }

        }
    }

    sf::IntRect TextureHolder::getSpriteBound(std::string spriteName) const
    {
        for(auto t_it = m_SpriteMap.begin(); t_it != m_SpriteMap.end(); t_it++)
        {
            for(auto r_it = t_it->second.begin(); r_it != t_it->second.end(); r_it++)
            {
               if(r_it->first == spriteName)
                    return r_it->second;
            }

        }
    }

    const std::vector<std::string>& TextureHolder::getSpriteTable() const
    {
        return m_SpriteTable;
    }

    int TextureHolder::getSpriteCount() const
    {
        return m_SpriteTable.size();
    }

    void TextureHolder::printSpriteTable() const
    {
		nero_log("TextureHolder : List of Sprite (" + nero_ss(m_SpriteTable.size()) + ")");

        for(std::string sprite : m_SpriteTable)
            nero_log(sprite);
    }

	void TextureHolder::smoothAllTexture()
	{
		for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
		{
			it->second->setSmooth(true);
		}
	}

	void TextureHolder::clear()
	{
		//clear parent
		ResourceHolder::clear();

		//clear current
		m_TextureMap.clear();
		m_SpriteMap.clear();
		m_SpriteTable.clear();
	}

}
