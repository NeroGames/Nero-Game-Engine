////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
// SFML
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
        typedef std::shared_ptr<SpriteObject> Ptr;
        static Ptr                            Cast(Object::Ptr object);

      public:
        SpriteObject();

        void                     setSprite(const sf::Sprite& sprite);
        const sf::Sprite&        getSprite() const;
        virtual sf::FloatRect    getGlobalBounds() const;

        void                     setTextureName(const sf::String& texture);
        std::string              getTextureName() const;

        virtual void             setColor(const sf::Color& color);
        virtual const sf::Color& getColor() const;

        virtual Object::Ptr      clone(sf::Vector2f& position) const;
        virtual Object::Ptr      clone() const;

        virtual nlohmann::json   toJson() const;

      private:
        virtual void drawObject(sf::RenderTarget& target, sf::RenderStates states) const;

      private:
        sf::Sprite m_Sprite;
        sf::String m_TextureName;
    };
} // namespace nero
#endif // SPRITEOBJECT_H
