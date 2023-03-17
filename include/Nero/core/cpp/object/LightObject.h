////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
// Lighting
#include <ltbl/LightPointEmission.hpp>
// STD
#include <functional>
/////////////////////////////////////////////////////////////
namespace nero
{
    class LightObject : public Object
    {
      public:
        typedef std::shared_ptr<LightObject> Ptr;
        static Ptr                           Cast(Object::Ptr object);

      public:
        LightObject();
        virtual ~LightObject();

        const std::string   getLightmap() const;
        void                setLightmap(const std::string& lightmap);
        void                setLight(ltbl::LightPointEmission* light);
        void                setCloneCallback(std::function<ltbl::LightPointEmission*()> callback);

        virtual Object::Ptr clone(sf::Vector2f& position) const;
        virtual Object::Ptr clone() const;

        virtual void        setColor(const sf::Color& color);
        virtual const sf::Color&  getColor() const;

        void                      setSprite(const sf::Sprite& sprite);
        virtual sf::FloatRect     getGlobalBounds() const;
        virtual void              updateObject(sf::Time time_step) override;
        ltbl::LightPointEmission* getLight();
        bool                      getLightEnabled() const;
        void                      setLightEnabled(bool enabled);

      private:
        virtual void drawObject(sf::RenderTarget& target, sf::RenderStates states) const;

      private:
        std::string                                m_Lightmap;
        ltbl::LightPointEmission*                  m_Light;
        std::function<ltbl::LightPointEmission*()> m_CloneLight;
        sf::Color                                  m_Color;
        sf::Sprite                                 m_Sprite;
    };
} // namespace nero

#endif // LIGHTOBJECT_H
