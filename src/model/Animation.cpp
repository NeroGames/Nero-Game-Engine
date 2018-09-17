#include <Nero/model/Animation.h>

#include <easyloggingpp/easylogging++.h>
#include <Nero/utility/LogUtility.h>
namespace nero
{
    Animation::Animation():
        m_ElapsedTime(sf::seconds(0.f))
    {
        //ctor
    }

    void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(m_Sprite, states);
    }

    void Animation::setSprite(sf::Sprite sprite)
    {
        m_Sprite = sprite;
    }

    void Animation::setTexture(std::string texture)
    {
        m_Texture = texture;
    }

    void Animation::addSequence(std::string name, AnimationSequence sequence)
    {
        m_SequenceMap[name] = sequence;
    }

    sf::FloatRect Animation::getGlobalBounds() const
    {
        return m_Sprite.getGlobalBounds();
    }

    void Animation::update(sf::Time timeStep)
    {
        m_ElapsedTime += timeStep;

        if(m_ElapsedTime >= sf::seconds(m_SequenceMap[m_CurrentSequence].getFrameRate()))
        {
            m_Sprite.setTextureRect(m_SequenceMap[m_CurrentSequence].getNextFrame());
            m_ElapsedTime -= sf::seconds(m_SequenceMap[m_CurrentSequence].getFrameRate());
        }
    }

    void Animation::setSequence(std::string name)
    {
        m_CurrentSequence = name;
    }

    std::string Animation::getTexture() const
    {
        return m_Texture;
    }

    void Animation::setColor(const sf::Color& color)
    {
        m_Sprite.setColor(color);
    }

    const sf::Color& Animation::getColor() const
    {
        return m_Sprite.getColor();
    }

    const Animation::SequenceMap& Animation::getSequenceMap() const
    {
        return m_SequenceMap;
    }

    void Animation::setFrameRate(float frameRate)
    {
        m_SequenceMap[m_CurrentSequence].setFrameRate(frameRate);
    }

    void Animation::setLoop(bool flag)
    {
        m_SequenceMap[m_CurrentSequence].setLoop(flag);
    }







}


