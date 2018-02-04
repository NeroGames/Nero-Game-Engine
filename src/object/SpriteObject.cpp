////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/object/SpriteObject.h>
#include <NERO/object/MeshObject.h>
#include <NERO/object/PhysicObject.h>
//BOX2D
#include <Box2D/Dynamics/b2Body.h>
////////////////////////////////////////////////////////////

namespace nero
{
    SpriteObject::SpriteObject():
        Object(),
        m_TextureName(""),
        m_ParentLastPosition(sf::Vector2f(0.f,0.f))
    {
        setFirstType(Object::Sprite_Object);
        setSecondType(Object::Sprite_Object);
        setIsUpdateable(false);
    }

    void SpriteObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_Sprite, states);
    }

    void SpriteObject::setSprite(const sf::Sprite& sprite)
    {
        m_Sprite = sprite;
    }

    const sf::Sprite& SpriteObject::getSprite() const
    {
        return m_Sprite;
    }

    void SpriteObject::setTextureName(const sf::String& texture)
    {
        m_TextureName = texture;
    }

    sf::String  SpriteObject::getTextureName() const
    {
        return m_TextureName;
    }

    sf::FloatRect SpriteObject::getGlobalBounds() const
    {
        return getTransform().transformRect(m_Sprite.getGlobalBounds());
    }

    Object::Ptr SpriteObject::clone(sf::Vector2f& position) const
    {
        SpriteObject::Ptr sprite_object = Cast(clone());
        sprite_object->move(position);
        sprite_object->setId(-1);

        return sprite_object;
    }

    Object::Ptr SpriteObject::clone() const
    {
        SpriteObject::Ptr sprite_object(new SpriteObject());
        Object::clone<SpriteObject::Ptr>(sprite_object);
        sf::Sprite sprite = m_Sprite;

        sprite_object->setSprite(sprite);
        sprite_object->setTextureName(getTextureName());
        sprite_object->setPosition(getPosition());
        sprite_object->setRotation(getRotation());
        sprite_object->setScale(getScale());

        return sprite_object;
    }

    void SpriteObject::setColor(const sf::Color& color)
    {
        m_Sprite.setColor(color);
    }

    const sf::Color& SpriteObject::getColor() const
    {
        return m_Sprite.getColor();
    }

    nlohmann::json SpriteObject::toJson() const
    {
        nlohmann::json spriteJson;

        spriteJson = Object::toJson();

        spriteJson["sprite"]    = getTextureName();
        spriteJson["rotation"]  = getRotation();
        spriteJson["position"]  = {{"x", getPosition().x}, {"y", getPosition().y}};
        spriteJson["scale"]     = {{"x", getScale().x}, {"y", getScale().y}};
        spriteJson["color"]     = {{"r", getColor().r}, {"g", getColor().g}, {"b", getColor().b}, {"a", getColor().a}};

        return spriteJson;
    }

    SpriteObject::Ptr SpriteObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Sprite_Object)
            return nullptr;

        return  std::static_pointer_cast<SpriteObject>(object);
    }
}
