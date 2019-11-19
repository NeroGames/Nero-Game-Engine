////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/resource/TextureHolder.h>
#include <Nero/core/utility/Utility.h>
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

	void TextureHolder::loadFile(const std::string& file)
	{
		std::experimental::filesystem::path filePath(file);

		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

		const std::string textureName = filePath.filename().stem().string();

		if (!texture->loadFromFile(filePath.string()))
		{
			nero_log("failed to load texture : " + textureName);

			return;
		}

		//Read the .txt file to retrieve sprite bound
		std::string txtHelper   = replaceFileExtension(filePath.string(), "txt");
		std::string jsonHelper  = replaceFileExtension(filePath.string(), "json");

		if(fileExist(jsonHelper))
		{
			nlohmann::json helper = loadJson(jsonHelper, true);

			nlohmann::json frame_table = helper["frames"];

			for (auto& frame : frame_table)
			{
				std::string spriteName      = removeFileExtension(frame["filename"].get<std::string>());
				int rectLeft                = frame["frame"]["x"];
				int rectTop                 = frame["frame"]["y"];
				int rectWidth               = frame["frame"]["w"];
				int rectHeight              = frame["frame"]["h"];

				sf::IntRect spriteBound = sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight);

				addSpriteBound(textureName, spriteName, spriteBound);
				m_SpriteTable.push_back(spriteName);

				nero_log("loaded : " + spriteName);
			}
		}
		else if (fileExist(txtHelper))// .txt helper file exist, the texture get several sprites
		{
			std::ifstream inStream(txtHelper);
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
			}
		}
		else //helper file not exist, texture is the sprite
		{
			sf::IntRect spriteBound = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);

			addSpriteBound(textureName, textureName, spriteBound);
			m_SpriteTable.push_back(textureName);

			nero_log("loaded : " + textureName);
		}

		//Insert the texture in m_TextureMap
		addTexture(textureName, std::move(texture));
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

		if(!directoryExist(m_SelectedDirectory))
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
			if(checkExtention(it->path().extension().string(), m_Setting.getStringTable("extension")))
            {
                //Load the Texture
				loadFile(it->path().string());
            }

            it++;
        }
    }

    void TextureHolder::addTexture(std::string textureName, std::unique_ptr<sf::Texture> texture)
    {
        auto inserted = m_TextureMap.insert(std::make_pair(textureName, std::move(texture)));

        if(!inserted.second)
        {
            nero_log("failed to store texture " + textureName);
        }
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
        nero_log("TextureHolder : List of Sprite (" + _s(m_SpriteTable.size()) + ")");

        for(std::string sprite : m_SpriteTable)
            nero_log(sprite);
    }
}
