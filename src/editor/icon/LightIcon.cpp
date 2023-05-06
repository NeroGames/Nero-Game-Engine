////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/icon/LightIcon.h>
////////////////////////////////////////////////////////////
namespace nero
{
    LightIcon::LightIcon()
        : Object()
        , m_LightmapName("")
        , m_LightColor(sf::Color::White)
        , m_LightEnabled(true)
        , m_LightScale(1.f, 1.f)
    {
        setFirstType(Object::Light_Object);
        setSecondType(Object::Light_Object);
        setIsUpdateable(true);
    }

    void LightIcon::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = sf::Transform::Identity;
        states.transform.translate(getPosition());
        target.draw(m_Sprite, states);
        target.draw(m_LightRing, states);
    }

    void LightIcon::updateObject(sf::Time)
    {
        const auto scale      = getScale();
        bool       updateRing = false;
        // bring x and y to same value
        if(m_LightScale.x != scale.x)
        {
            setScale(scale.x, scale.x);
            m_LightScale = scale;
            updateRing   = true;
        }
        else if(m_LightScale.y != scale.y)
        {
            setScale(scale.y, scale.y);
            m_LightScale = scale;
            updateRing   = true;
        }

        if(updateRing)
        {
            const auto currentScale = getScale().x;
            float      scaleFactor  = 1.f;

            if(currentScale < 1.f)
            {
                scaleFactor -= currentScale / 10.f;
            }
            else
            {
                scaleFactor += currentScale / 10.f;
            }
            m_LightRing.setScale(scaleFactor, scaleFactor);
        }
    }

    void LightIcon::setSprite(const sf::Sprite& sprite)
    {
        m_Sprite           = sprite;
        const float radius = m_Sprite.getLocalBounds().height;
        m_LightRing.setRadius(radius);
        m_LightRing.setFillColor(sf::Color::Transparent);
        m_LightRing.setOutlineThickness(4.f);
        m_LightRing.setOutlineColor(m_LightColor);
        m_LightRing.setOrigin(radius, radius);
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
        bound.height   = m_Sprite.getTextureRect().height;
        bound.width    = m_Sprite.getTextureRect().width;
        const auto pos = getWorldPosition();
        bound.left     = pos.x - bound.width / 2.f;
        bound.top      = pos.y - bound.height / 2.f;

        return bound;
    }

    Object::Ptr LightIcon::clone(sf::Vector2f& position) const
    {
        LightIcon::Ptr lightProb = Cast(clone());
        lightProb->move(position);
        lightProb->setId(-1);

        return lightProb;
    }

    Object::Ptr LightIcon::clone() const
    {
        LightIcon::Ptr lightProb(new LightIcon());
        Object::clone<LightIcon::Ptr>(lightProb);
        sf::Sprite sprite = m_Sprite;

        lightProb->setSprite(sprite);
        lightProb->setLightmapName(getLightmapName());
        lightProb->setPosition(getPosition());
        lightProb->setRotation(getRotation());
        lightProb->setScale(getScale());

        return lightProb;
    }

    void LightIcon::setColor(const sf::Color& color)
    {
        m_LightColor = color;
        m_LightRing.setOutlineColor(m_LightColor);
    }

    const sf::Color& LightIcon::getColor() const
    {
        return m_LightColor;
    }

    bool LightIcon::getLightEnabled() const
    {
        return m_LightEnabled;
    }

    void LightIcon::setLightEnabled(bool enabled)
    {
        m_LightEnabled = enabled;

        if(m_LightEnabled)
            m_Sprite.setColor(sf::Color::White);
        else
            m_Sprite.setColor(sf::Color::Red);
    }

    nlohmann::json LightIcon::toJson() const
    {
        nlohmann::json iconJson;

        iconJson                  = Object::toJson();

        iconJson["lightmap_name"] = getLightmapName();
        iconJson["enabled"]       = getLightEnabled();
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
} // namespace nero
