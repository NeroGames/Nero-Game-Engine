////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/editor/icon/LightIcon.h>
////////////////////////////////////////////////////////////
namespace nero
{
    LightIcon::LightIcon()
        : Object()
        , m_LightmapName("")
        , m_ParentLastPosition(sf::Vector2f(0.f, 0.f))
    {
        setFirstType(Object::Light_Object);
        setSecondType(Object::Light_Object);
        setIsUpdateable(false);
    }

    void LightIcon::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = sf::Transform::Identity;
        states.transform.translate(getPosition());
        target.draw(m_Sprite, states);
    }

    void LightIcon::setSprite(const sf::Sprite& sprite)
    {
        m_Sprite = sprite;
    }

    const sf::Sprite& LightIcon::getSprite() const
    {
        return m_Sprite;
    }

    void LightIcon::setLightmapName(const sf::String& texture)
    {
        m_LightmapName = texture;
    }

    std::string LightIcon::getLightmapName() const
    {
        return m_LightmapName;
    }

    sf::FloatRect LightIcon::getGlobalBounds() const
    {
        sf::FloatRect bound;
        bound.height     = m_Sprite.getTextureRect().height;
        bound.width      = m_Sprite.getTextureRect().width;
        sf::Vector2f pos = getWorldPosition();
        bound.left       = pos.x - bound.width / 2.f;
        bound.top        = pos.y - bound.height / 2.f;

        return bound;
    }

    Object::Ptr LightIcon::clone(sf::Vector2f& position) const
    {
        LightIcon::Ptr sprite_object = Cast(clone());
        sprite_object->move(position);
        sprite_object->setId(-1);

        return sprite_object;
    }

    Object::Ptr LightIcon::clone() const
    {
        LightIcon::Ptr light_icon(new LightIcon());
        Object::clone<LightIcon::Ptr>(light_icon);
        sf::Sprite sprite = m_Sprite;

        light_icon->setSprite(sprite);
        light_icon->setLightmapName(getLightmapName());
        light_icon->setPosition(getPosition());
        light_icon->setRotation(getRotation());
        light_icon->setScale(getScale());

        return light_icon;
    }

    nlohmann::json LightIcon::toJson() const
    {
        nlohmann::json iconJson;

        iconJson                  = Object::toJson();

        iconJson["lightmap_name"] = getLightmapName();
        iconJson["rotation"]      = getRotation();
        iconJson["position"]      = {
            {"x", getPosition().x},
            {"y", getPosition().y}
        };
        iconJson["scale"] = {
            {"x", getScale().x},
            {"y", getScale().y}
        };
        iconJson["color"] = {
            {"r", getColor().r},
            {"g", getColor().g},
            {"b", getColor().b},
            {"a", getColor().a}
        };

        return iconJson;
    }

    LightIcon::Ptr LightIcon::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Light_Object)
            return nullptr;

        return std::static_pointer_cast<LightIcon>(object);
    }

    void LightIcon::scale(const sf::Vector2f& factor)
    {
    }

    void LightIcon::scale(float factorX, float factorY)
    {
    }

    void LightIcon::setScale(float factorX, float factorY)
    {
    }

    void LightIcon::setScale(const sf::Vector2f& factors)
    {
    }

    void LightIcon::rotate(float angle)
    {
    }

    void LightIcon::setRotation(float angle)
    {
    }

} // namespace nero
