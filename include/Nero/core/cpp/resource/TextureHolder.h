////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
//SFML
#include <SFML/Graphics/Texture.hpp>
//JSON
#include <json/json.hpp>
//STD
#include <memory>
#include <vector>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
	class TextureHolder : public ResourceHolder
    {
		public: //utility
			typedef std::shared_ptr<TextureHolder>    Ptr;
			using	ResourceHolder::loadDirectory;
			using	ResourceHolder::loadFile;

        public:
                                                TextureHolder();
												TextureHolder(const Setting& setting);
			virtual							   ~TextureHolder() override;

			virtual void						loadDirectory() override;
			virtual bool						loadFile(const std::string& file) override;
			virtual void						destroy() override;
			virtual void						clear() override;

            sf::Texture&				        getTexture(std::string textureName);
            const sf::Texture&			        getTexture(std::string textureName)         const;
            const sf::Texture&			        getSpriteTexture(std::string spriteName)    const;
            sf::IntRect                         getSpriteBound(std::string spriteName)      const;
            const std::vector<std::string>&     getSpriteTable()                            const;
            int                                 getSpriteCount()                            const;
            void                                printSpriteTable()                          const;

			void								setResourceDirectory(const std::string& resourceDictory);
			void								smoothAllTexture();


        private:
			bool                                addTexture(std::string textureId, std::unique_ptr<sf::Texture> texture);
			void                                addSpriteBound(std::string textureId, std::string spriteId, sf::IntRect spriteRect);
			void                                addSpriteBound(std::map<std::string, std::map<std::string, sf::IntRect>> spriteMap);
			void								addSprite(std::vector<std::string> spriteTable);

        private:
            std::map<std::string, std::unique_ptr<sf::Texture>>	            m_TextureMap;
            std::map<std::string, std::map<std::string, sf::IntRect>>       m_SpriteMap;
            std::vector<std::string>                                        m_SpriteTable;
    };
}
#endif // TEXTUREHOLDER_H
