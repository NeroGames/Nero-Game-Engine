////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
////////////////////////////////////////////////////////////

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/resource/TextureHolder.h>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    void TextureHolder::load(const std::string& folder)
    {
        using namespace boost::filesystem;
        path p(folder);
        m_logMessage = "\nTexture_Holder-->Loading textures from \"" + p.string() +"\"";

        //Iterate over files in the folder
        directory_iterator it{p};
        while (it != directory_iterator{})
        {
            //When we found a Texture
            if(it->path().extension().string() != ".txt")
            {
                //Load the Texture
                std::unique_ptr<sf::Texture> texture(new sf::Texture());

                if (!texture->loadFromFile(it->path().string()))
                    throw std::runtime_error("TextureHolder::load - Failed to load " + it->path().filename().string());

                //get the texture in m_TextureMap
                std::string textureId = it->path().filename().stem().string();
                m_logMessage += "\nTexture_Holder-->Texture loaded : \"" + textureId + "\"";

                //Read the .txt file to retrieve sprite rectangles
                std::string file = replaceFileExtension(it->path().string(), "txt");

                //if the file existe, the texture get several sprites
                if (exists(file))
                {
                    std::ifstream inStream(replaceFileExtension(it->path().string(), "txt"));

                    //Split each line to retrieve the necessary information
                    std::string line;
                    while (std::getline(inStream, line))
                    {
                        std::vector<std::string> sprite = splitString(line, '-');

                        std::string spriteId    = sprite[0];
                        int rectLeft            = std::stoi(sprite[1]);
                        int rectTop             = std::stoi(sprite[2]);
                        int rectWidth           = std::stoi(sprite[3]);
                        int rectHeith           = std::stoi(sprite[4]);

                        sf::IntRect spriteRect = sf::IntRect(rectLeft, rectTop, rectWidth, rectHeith);

                        insertSpriteRect(textureId, spriteId, spriteRect);
                        m_SpriteTab.push_back(spriteId);

                        m_logMessage += "\nTexture_Holder-->Sprite Detected : \"" + spriteId + "\" in texture \"" + textureId + "\"";

                    }
                }
                else //the texture is the sprite
                {
                    sf::IntRect spriteRect = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);

                    insertSpriteRect(textureId, textureId, spriteRect);
                    m_SpriteTab.push_back(textureId);

                    m_logMessage += "\nTexture_Holder-->Sprite Detected : \"" + textureId + "\" in texture \"" + textureId + "\"";
                }

                //Insert the texture in m_TextureMap
                insertTexture(textureId, std::move(texture));
            }

            it++;
        }
    }

    void TextureHolder::insertTexture(std::string textureId, std::unique_ptr<sf::Texture> texture)
    {
        auto inserted = m_TextureMap.insert(std::make_pair(textureId, std::move(texture)));
        assert(inserted.second);
    }

    void TextureHolder::insertSpriteRect(std::string textureId, std::string spriteId, sf::IntRect spriteRect)
    {
        m_SpriteRectMap[textureId][spriteId] = spriteRect;
    }

    sf::Texture& TextureHolder::getTexture(std::string textureId)
    {
        auto found = m_TextureMap.find(textureId);
        assert(found != m_TextureMap.end());

            return *found->second;
    }

    const sf::Texture& TextureHolder::getTexture(std::string textureId) const
    {
        auto found = m_TextureMap.find(textureId);
        assert(found != m_TextureMap.end());

            return *found->second;
    }

    const sf::Texture& TextureHolder::getSpriteTexture(std::string spriteId) const
    {
        std::string textureId;

        for(auto t_it = m_SpriteRectMap.begin(); t_it != m_SpriteRectMap.end(); t_it++)
        {
            for(auto r_it = t_it->second.begin(); r_it != t_it->second.end(); r_it++)
            {
               if(r_it->first == spriteId)
                    return getTexture(t_it->first);
            }

        }
    }

    sf::IntRect TextureHolder::getSpriteRect(std::string spriteId) const
    {
        for(auto t_it = m_SpriteRectMap.begin(); t_it != m_SpriteRectMap.end(); t_it++)
        {
            for(auto r_it = t_it->second.begin(); r_it != t_it->second.end(); r_it++)
            {
               if(r_it->first == spriteId)
                    return r_it->second;
            }

        }
    }

    const std::vector<std::string>& TextureHolder::getSpriteTab() const
    {
        return m_SpriteTab;
    }

    int TextureHolder::getSpriteCount() const
    {
        return m_SpriteTab.size();
    }

    std::string TextureHolder::getLog() const
    {
        return m_logMessage;
    }

}
