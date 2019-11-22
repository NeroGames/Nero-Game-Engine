////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/object/Object.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class SpriteObject : public Object
    {
        public:
            typedef std::shared_ptr<SpriteObject>   Ptr;
            static Ptr                              Cast(Object::Ptr object);

        public:
                                            SpriteObject();

            void                            setSprite(const sf::Sprite& sprite);
            const sf::Sprite&               getSprite()                     const;
            virtual sf::FloatRect           getGlobalBounds()               const;

            void                            setTextureName(const sf::String& texture);
            sf::String                      getTextureName()                const;

            virtual void                    setColor(const sf::Color& color);
            virtual const sf::Color&        getColor()                      const;

            virtual Object::Ptr             clone(sf::Vector2f& position)   const;
            virtual Object::Ptr             clone()                         const;

            virtual nlohmann::json          toJson()                        const;

        private:
            virtual void	                drawObject(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            sf::Sprite                      m_Sprite;
            sf::String                      m_TextureName;
            sf::Vector2f                    m_ParentLastPosition;
    };
}
#endif // SPRITEOBJECT_H
