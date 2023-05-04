////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef LIGHTICON_H
#define LIGHTICON_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
// SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
//
#include <SFML/Graphics/CircleShape.hpp>
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

        void                     setSprite(const sf::Sprite& sprite);
        const sf::Sprite&        getSprite() const;
        virtual sf::FloatRect    getGlobalBounds() const override;
        void                     setLightmapName(const sf::String& lightmap);
        std::string              getLightmapName() const;
        virtual Object::Ptr      clone(sf::Vector2f& position) const override;
        virtual Object::Ptr      clone() const override;
        virtual nlohmann::json   toJson() const override;
        virtual void             setColor(const sf::Color& color) override;
        virtual const sf::Color& getColor() const override;
        bool                     getLightEnabled() const;
        void                     setLightEnabled(bool enabled);

      private:
        virtual void drawObject(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateObject(sf::Time time_step) override;

      private:
        sf::Sprite      m_Sprite;
        sf::String      m_LightmapName;
        sf::Color       m_LightColor;
        bool            m_LightEnabled;
        sf::CircleShape m_LightRing;
        sf::Vector2f    m_LightScale;
    };
} // namespace nero
#endif // LIGHTICON_H
