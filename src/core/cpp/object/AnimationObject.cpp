////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/object/AnimationObject.h>
////////////////////////////////////////////////////////////
namespace nero
{
    AnimationObject::AnimationObject()
    {
        setFirstType(Object::Animation_Object);
        setSecondType(Object::Animation_Object);
    }

    void AnimationObject::setAnimation(Animation animation)
    {
        m_Animation = animation;
    }

    void AnimationObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_Animation, states);
    }

    sf::FloatRect AnimationObject::getGlobalBounds() const
    {
        return getTransform().transformRect(m_Animation.getGlobalBounds());
    }

    AnimationObject::Ptr AnimationObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Animation_Object)
            return nullptr;

        return  std::static_pointer_cast<AnimationObject>(object);
    }

    Object::Ptr AnimationObject::clone(sf::Vector2f& position) const
    {
        AnimationObject::Ptr animation_object = Cast(clone());
        animation_object->move(position);
        animation_object->setId(-1);

        return animation_object;
    }

    Object::Ptr AnimationObject::clone() const
    {
        AnimationObject::Ptr animation_object(new AnimationObject());
        Object::clone<AnimationObject::Ptr>(animation_object);
        Animation animation = m_Animation;

        animation_object->setAnimation(animation);
        animation_object->setPosition(getPosition());
        animation_object->setRotation(getRotation());
        animation_object->setScale(getScale());

        return animation_object;
    }

    void AnimationObject::updateObject(sf::Time timeStep)
    {
        m_Animation.update(timeStep);
    }

    nlohmann::json AnimationObject::toJson() const
    {
        nlohmann::json animationJson;

        animationJson = Object::toJson();

        animationJson["animation"]    = m_Animation.getTexture();
        animationJson["rotation"]  = getRotation();
        animationJson["position"]  = {{"x", getPosition().x}, {"y", getPosition().y}};
        animationJson["scale"]     = {{"x", getScale().x}, {"y", getScale().y}};
        animationJson["color"]     = {{"r", getColor().r}, {"g", getColor().g}, {"b", getColor().b}, {"a", getColor().a}};

        nlohmann::json sequenceTableJson;
        const auto& sequenceMap = m_Animation.getSequenceMap();
        for(auto it=sequenceMap.begin(); it!=sequenceMap.end(); it++)
        {
            nlohmann::json sequenceJson;
            sequenceJson["frame_rate"] = it->second.getFrameRate();
            sequenceJson["loop"] = it->second.getLoop();
            sequenceTableJson[it->first] = sequenceJson;
        }

        animationJson["sequence_table"] = sequenceTableJson;

        return animationJson;
    }

    void AnimationObject::setColor(const sf::Color& color)
    {
        m_Animation.setColor(color);
    }

    const sf::Color& AnimationObject::getColor() const
    {
        return m_Animation.getColor();
    }

    const Animation::SequenceMap& AnimationObject::getSequenceMap() const
    {
        return m_Animation.getSequenceMap();
    }

    void AnimationObject::setSequence(std::string name)
    {
        m_Animation.setSequence(name);
    }

    void AnimationObject::setFrameRate(float frameRate)
    {
        m_Animation.setFrameRate(frameRate);
    }

    void AnimationObject::setLoop(bool flag)
    {
        m_Animation.setLoop(flag);
    }

    void AnimationObject::verticalFlip()
    {
        m_Animation.verticalFlip();
    }

    void AnimationObject::horizontalFlip()
    {
        m_Animation.horizontalFlip();
    }
}
