////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/TextureHolder.h>
#include <Nero/utility/Utility.h>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    TextureHolder::TextureHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["texture"];
    }

    void TextureHolder::load()
    {
        const std::string folder_name = m_Configuration["folder"].get<std::string>();

        nero_log("Resource path : " + folder_name);

        using namespace boost::filesystem;
        path folder_path(folder_name);

        if(!exists(folder_name))
        {
            nero_log("failed to load texture resource");
            nero_log("folder not found : " + folder_name);
            return;
        }

        //Iterate over files in the folder
        directory_iterator it{folder_path};
        while (it != directory_iterator{})
        {
            //When we found a Texture
            if(checkExtention(it->path().extension().string(), m_Configuration["extension"]))
            {
                //Load the Texture

                std::unique_ptr<sf::Texture> texture = make_unique<sf::Texture>();

                const std::string textureName = it->path().filename().stem().string();

                if (!texture->loadFromFile(it->path().string()))
                {
                    nero_log("failed to load texture : " + textureName);
                    it++;
                    continue;
                }

                //Read the .txt file to retrieve sprite bound
                std::string txtHelper   = replaceFileExtension(it->path().string(), "txt");
                std::string jsonHelper  = replaceFileExtension(it->path().string(), "json");

                if (exists(txtHelper))// .txt helper file exist, the texture get several sprites
                {
                    std::ifstream inStream(txtHelper);
                    //Split each line to retrieve the necessary information
                    const std::string separatorString = m_Configuration["separator"].get<std::string>();
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
                else if(exists(jsonHelper))
                {
                    nlohmann::json helper = loadJson(jsonHelper);

                    nlohmann::json frame_table = helper["frames"];

                    for (auto& frame : frame_table)
                    {
                        std::string spriteName      = removeFileExtension(frame["filename"].get<std::string>());
                        int rectLeft                = frame["x"];
                        int rectTop                 = frame["y"];
                        int rectWidth               = frame["w"];
                        int rectHeight              = frame["h"];

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
