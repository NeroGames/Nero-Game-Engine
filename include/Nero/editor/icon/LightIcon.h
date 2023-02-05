////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef LIGHTICON_H
#define LIGHTICON_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/Object.h>
// SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class LightIcon : public Object
    {
      public:
        typedef std::shared_ptr<LightIcon> Ptr;
        static Ptr                         Cast(Object::Ptr object);

      public:
        LightIcon();

        void                   setSprite(const sf::Sprite& sprite);
        const sf::Sprite&      getSprite() const;
        virtual sf::FloatRect  getGlobalBounds() const;

        void                   setLightmapName(const sf::String& lightmap);
        std::string            getLightmapName() const;

        virtual Object::Ptr    clone(sf::Vector2f& position) const;
        virtual Object::Ptr    clone() const;

        virtual nlohmann::json toJson() const;

        virtual void           scale(const sf::Vector2f& factor);
        virtual void           scale(float factorX, float factorY);
        virtual void           setScale(float factorX, float factorY);
        virtual void           setScale(const sf::Vector2f& factors);
        virtual void           rotate(float angle);
        virtual void           setRotation(float angle);

      private:
        virtual void drawObject(sf::RenderTarget& target, sf::RenderStates states) const;

      private:
        sf::Sprite   m_Sprite;
        sf::String   m_LightmapName;
        sf::Vector2f m_ParentLastPosition;
    };
} // namespace nero
#endif // LIGHTICON_H
