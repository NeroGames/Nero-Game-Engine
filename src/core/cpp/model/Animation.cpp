////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/model/Animation.h>

// EASYLOG
// #include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    Animation::Animation()
        : m_ElapsedTime(sf::seconds(0.f))
        , m_pause(false)
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(m_Sprite, states);
    }

    ////////////////////////////////////////////////////////////
    void Animation::setSprite(sf::Sprite sprite)
    {
        m_Sprite = sprite;
    }

    ////////////////////////////////////////////////////////////
    void Animation::setTexture(std::string texture)
    {
        m_Texture = texture;
    }

    ////////////////////////////////////////////////////////////
    void Animation::addSequence(std::string name, AnimationSequence sequence)
    {
        m_SequenceMap[name] = sequence;
    }

    ////////////////////////////////////////////////////////////
    sf::FloatRect Animation::getGlobalBounds() const
    {
        return m_Sprite.getGlobalBounds();
    }

    ////////////////////////////////////////////////////////////
    void Animation::setSequence(std::string name)
    {
        if(m_CurrentSequence != name)
        {
            m_SequenceMap[name].reset();
            m_CurrentSequence = name;
        }
    }

    ////////////////////////////////////////////////////////////
    std::string Animation::getTexture() const
    {
        return m_Texture;
    }

    ////////////////////////////////////////////////////////////
    void Animation::setColor(const sf::Color& color)
    {
        m_Sprite.setColor(color);
    }

    ////////////////////////////////////////////////////////////
    const sf::Color& Animation::getColor() const
    {
        return m_Sprite.getColor();
    }

    ////////////////////////////////////////////////////////////
    const Animation::SequenceMap& Animation::getSequenceMap() const
    {
        return m_SequenceMap;
    }

    ////////////////////////////////////////////////////////////
    void Animation::setFrameRate(float frameRate)
    {
        m_SequenceMap[m_CurrentSequence].setFrameRate(frameRate);
    }

    ////////////////////////////////////////////////////////////
    void Animation::setLoop(bool flag)
    {
        m_SequenceMap[m_CurrentSequence].setLoop(flag);
    }

    ////////////////////////////////////////////////////////////
    void Animation::verticalFlip()
    {
        m_Sprite.setScale(-m_Sprite.getScale().x, 1.f);
    }

    ////////////////////////////////////////////////////////////
    void Animation::horizontalFlip()
    {
        m_Sprite.setScale(1.f, -m_Sprite.getScale().y);
    }

    ////////////////////////////////////////////////////////////
    void Animation::play()
    {
        m_pause = false;
    }

    ////////////////////////////////////////////////////////////
    void Animation::pause()
    {
        m_pause = true;
    }

    ////////////////////////////////////////////////////////////
    void Animation::update(sf::Time timeStep)
    {
        if(m_pause)
            return;

        m_ElapsedTime += timeStep;
        sf::Time framerate = sf::seconds(m_SequenceMap[m_CurrentSequence].getFrameRate());

        if(m_ElapsedTime >= framerate)
        {
            m_Sprite.setTextureRect(m_SequenceMap[m_CurrentSequence].getNextFrame());

            m_ElapsedTime -= framerate;
        }
    }
} // namespace nero
////////////////////////////////////////////////////////////
