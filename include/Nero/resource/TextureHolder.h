////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2018 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Graphics/Texture.hpp>
//JSON
#include <json/json.hpp>
//STD
#include <memory>
#include <map>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class TextureHolder
    {
        public:
                                                TextureHolder();
            void						        load();

            sf::Texture&				        getTexture(std::string textureName);
            const sf::Texture&			        getTexture(std::string textureName) const;
            const sf::Texture&			        getSpriteTexture(std::string spriteName) const;
            sf::IntRect                         getSpriteBound(std::string spriteName) const;
            const std::vector<std::string>&     getSpriteTable() const;
            int                                 getSpriteCount() const;
            void                                printSpriteTable() const;

        private:
            void        addTexture(std::string textureId, std::unique_ptr<sf::Texture> texture);
            void        addSpriteBound(std::string textureId, std::string spriteId, sf::IntRect spriteRect);


        private:
            std::map<std::string, std::unique_ptr<sf::Texture>>	            m_TextureMap;
            std::map<std::string, std::map<std::string, sf::IntRect>>       m_SpriteMap;
            std::vector<std::string>                                        m_SpriteTable;
            nlohmann::json                                                  m_Configuration;
    };

}

#endif // TEXTUREHOLDER_H
