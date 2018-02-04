////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
////////////////////////////////////////////////////////////
#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/Utility.h>
//SFML
#include <SFML/Graphics/Texture.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    class TextureHolder
    {
        public:
            void						        load(const std::string& folder);

            sf::Texture&				        getTexture(std::string textureId);
            const sf::Texture&			        getTexture(std::string textureId) const;
            const sf::Texture&			        getSpriteTexture(std::string spriteId) const;
            sf::IntRect                         getSpriteRect(std::string spriteId) const;
            const std::vector<std::string>&     getSpriteTab() const;
            int                                 getSpriteCount() const;
            std::string                         getLog() const;


        private:
            void						insertTexture(std::string textureId, std::unique_ptr<sf::Texture> texture);
            void						insertSpriteRect(std::string textureId, std::string spriteId, sf::IntRect spriteRect);


        private:
            std::map<std::string, std::unique_ptr<sf::Texture>>	            m_TextureMap;
            std::map<std::string, std::map<std::string, sf::IntRect>>       m_SpriteRectMap;
            std::vector<std::string>                                        m_SpriteTab;

            std::string                                                     m_logMessage;
    };

}

#endif // TEXTUREHOLDER_H
