////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include <Nero/core/cpp/object/LightObject.h>
#include <Nero/core/cpp/utility/String.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    LightObject::LightObject()
        : m_Lightmap(StringPool.BLANK)
        , m_Light(nullptr)
        , m_Color(sf::Color::White)
    {
        setFirstType(Object::Light_Object);
        setSecondType(Object::Light_Object);
        setIsUpdateable(true);
    }

    LightObject::~LightObject()
    {
        m_Light = nullptr;
    }

    LightObject::Ptr LightObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Light_Object)
            return nullptr;

        return std::static_pointer_cast<LightObject>(object);
    }

    const std::string LightObject::getLightmap() const
    {
        return m_Lightmap;
    }

    void LightObject::setLightmap(const std::string& lightmap)
    {
        m_Lightmap = lightmap;
    }

    void LightObject::setLight(ltbl::LightPointEmission* light)
    {
        m_Light = light;
    }

    Object::Ptr LightObject::clone(sf::Vector2f& position) const
    {
        LightObject::Ptr light_object = Cast(clone());
        light_object->move(position);
        light_object->setId(-1);

        return light_object;
    }

    void LightObject::setCloneCallback(std::function<ltbl::LightPointEmission*()> callback)
    {
        m_CloneLight = callback;
    }

    Object::Ptr LightObject::clone() const
    {
        LightObject::Ptr light_object(new LightObject());
        Object::clone<LightObject::Ptr>(light_object);

        if(m_Light && m_CloneLight)
        {
            ltbl::LightPointEmission* light = m_CloneLight();
            light_object->setLight(light);
            light = nullptr;
        }

        light_object->setLightmap(m_Lightmap);
        light_object->setPosition(getPosition());
        light_object->setRotation(getRotation());
        light_object->setScale(getScale());
        light_object->setColor(getColor());

        return light_object;
    }

    void LightObject::setColor(const sf::Color& color)
    {
        m_Color = color;
    }

    const sf::Color& LightObject::getColor() const
    {
        return m_Color;
    }

    void LightObject::updateObject(sf::Time time_step)
    {
        m_Sprite.setScale(1.f, 1.f);
    }

    void LightObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_Sprite, states);
    }

    void LightObject::setSprite(const sf::Sprite& sprite)
    {
        m_Sprite = sprite;
    }

    sf::FloatRect LightObject::getGlobalBounds() const
    {
        return getTransform().transformRect(m_Sprite.getGlobalBounds());
    }

    ltbl::LightPointEmission* LightObject::getLight()
    {
        return m_Light;
    }

    bool LightObject::getLightEnabled() const
    {
        return m_Light->isTurnedOn();
    }

    void LightObject::setLightEnabled(bool enabled)
    {
        m_Light->setTurnedOn(enabled);
    }

} // namespace nero
